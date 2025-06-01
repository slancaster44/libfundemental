#include <malloc.h>
#include <string.h>

#include "arena.h"
#include "alignment.h"

void init_block(ArenaBlock_t **block, uint64_t size)
{
    uint8_t *mem = malloc(size + sizeof(ArenaBlock_t));

    for (int i = 0; i < size + sizeof(ArenaBlock_t); i++)
    {
        mem[i] = 0;
    }

    *block = (ArenaBlock_t *)mem;
    (*block)->start = (uint8_t *)((mem) + sizeof(ArenaBlock_t));
    (*block)->end = (uint8_t *)(*block)->start + size;
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

void *ArenaAllocate(Arena_t *a, uint64_t size)
{
    if (a == &ARENA_NONE || a->blocks == ((void *)-1))
    {
        return malloc(size);
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

__attribute__((constructor)) void construct_perm()
{
    ConstructArena(&ARENA_PERM);
}

__attribute__((destructor)) void deconstruct_perm()
{
    DeconstructArena(&ARENA_PERM);
}