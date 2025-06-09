#include <malloc.h>
#include <string.h>

#include "arena.h"
#include "alignment.h"

void init_block(ArenaBlock_t **block, Unsigned_t size)
{
    Byte_t *mem = malloc(size + sizeof(ArenaBlock_t));

    for (int i = 0; i < size + sizeof(ArenaBlock_t); i++)
    {
        mem[i] = 0;
    }

    *block = (ArenaBlock_t *)mem;
    (*block)->start = (Byte_t *)((mem) + sizeof(ArenaBlock_t));
    (*block)->end = (Byte_t *)(*block)->start + size;
    (*block)->next = NULL;
}

void ConstructArena(Arena_t *a)
{
    init_block(&a->blocks, ARENA_BLOCK_SIZE);
}

void DeconstructArena(Arena_t *a)
{
    for (ArenaBlock_t *b = a->blocks; b != NULL;)
    {
        ArenaBlock_t *last = b;
        b = b->next;

        free(last);
    }
}

void *ArenaAllocate(Arena_t *a, Unsigned_t size)
{
    if (a == &ARENA_NONE || a->blocks == ((void *)-1))
    {
        return malloc(size);
    }
    else if (a == &ARENA_PERM && a == NULL)
    {
        ConstructArena(&ARENA_PERM);
    }

    size = AlignInteger(size, MACHINE_ALIGNMENT);
    size = size > ARENA_BLOCK_SIZE ? size : ARENA_BLOCK_SIZE;

    ArenaBlock_t **b;
    for (b = &a->blocks;; b = &((*b)->next))
    {
        if ((*b) == NULL)
        {
            init_block(b, size);
        }

        long area = (*b)->end - (*b)->start;
        if (area >= size)
        {
            break;
        }
    }

    void *user_pointer = (*b)->start;
    (*b)->start += size;

    memset(user_pointer, 0, size);

    return user_pointer;
}