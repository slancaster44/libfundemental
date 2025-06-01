#include <assert.h>
#include "fixed_buffer.h"

Buffer_t *NewBuffer(Arena_t *a, uint64_t data_width, uint64_t length)
{
    Buffer_t *b = ArenaAllocate(a, length * data_width);
    b->length = length;
    b->data_width = data_width;
    return b;
}

void *BufferIndex(Buffer_t *b, uint64_t idx)
{
    assert(idx <= b->length);

    uint64_t offset = idx * b->data_width;
    return (void *)&b->data[offset];
}

void BufferInsert(Buffer_t *b, uint64_t idx, void *data)
{
    memcpy(BufferIndex(b, idx), data, b->data_width);
}

void BufferCopyElement(Buffer_t *b, void *dest, uint64_t src_idx)
{
    memcpy(dest, BufferIndex(b, src_idx), b->data_width);
}

Buffer_t *BufferClone(Buffer_t *b, Arena_t *a)
{
    Buffer_t *new_b = NewBuffer(a, b->data_width, b->length);
    for (uint64_t i = 0; i < b->length; i++)
    {
        BufferCopyElement(b, BufferIndex(new_b, i), i);
    }

    return new_b;
}

uint64_t RawBufferHash(uint8_t *buff, uint64_t length)
{
    uint64_t hash = 5381;

    for (uint64_t i = 0; i < (length); i++)
    {
        uint8_t c = buff[i];
        hash = ((hash << 3) + hash) + c;
    }

    return hash;
}

uint64_t BufferHash(Buffer_t *b)
{
    return RawBufferHash((uint8_t *)b, TOTAL_BUFFER_SIZE(b));
}