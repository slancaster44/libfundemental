#include <assert.h>
#include "fixed_buffer.h"

Buffer_t *NewBuffer(Arena_t *a, Unsigned_t data_width, Unsigned_t length)
{
    Buffer_t *b = ArenaAllocate(a, length * data_width);
    b->length = length;
    b->data_width = data_width;
    return b;
}

Buffer_t *InitializeBuffer(Byte_t *memory, Unsigned_t data_width, Unsigned_t length)
{
    Buffer_t *b = (Buffer_t*) memory;
    b->length = length;
    b->data_width = data_width;
    return b;
}

void *BufferIndex(Buffer_t *b, Unsigned_t idx)
{
    assert(idx <= b->length);

    Unsigned_t offset = idx * b->data_width;
    return (void *)&b->data[offset];
}

void BufferInsert(Buffer_t *b, Unsigned_t idx, void *data)
{
    memcpy(BufferIndex(b, idx), data, b->data_width);
}

void BufferCopyElement(Buffer_t *b, void *dest, Unsigned_t src_idx)
{
    memcpy(dest, BufferIndex(b, src_idx), b->data_width);
}

Buffer_t *BufferClone(Buffer_t *b, Arena_t *a)
{
    Buffer_t *new_b = NewBuffer(a, b->data_width, b->length);
    for (Unsigned_t i = 0; i < b->length; i++)
    {
        BufferCopyElement(b, BufferIndex(new_b, i), i);
    }

    return new_b;
}

Unsigned_t RawBufferHash(Byte_t *buff, Unsigned_t length)
{
    Unsigned_t hash = 5381;

    for (Unsigned_t i = 0; i < (length); i++)
    {
        Byte_t c = buff[i];
        hash = ((hash << 3) + hash) + c;
    }

    return hash;
}

Unsigned_t BufferHash(Buffer_t *b)
{
    return RawBufferHash((Byte_t *)b, TOTAL_BUFFER_SIZE(b));
}

typedef struct _fixed_buffer_it_s
{
    Buffer_t *buffer;
    Unsigned_t cur_idx;
} BufferItOpaque_t;

_Static_assert(sizeof(BufferItOpaque_t) <= IT_OPAQUE_DATA_SIZE, "Buffer iterator opaque size too large");

void BufferIteratorNext(BufferItOpaque_t *opaque)
{
    opaque->cur_idx++;
}

void BufferIteratorPrev(BufferItOpaque_t *opaque)
{
    opaque->cur_idx--;
}

bool BufferIteratorDone(BufferItOpaque_t *opaque)
{
    return opaque->cur_idx > opaque->buffer->length;
}

void *BufferIteratorItem(BufferItOpaque_t *opaque)
{
    return BufferIndex(opaque->buffer, opaque->cur_idx);
}

Iterator_t NewBufferIterator(Buffer_t *buffer)
{
    Iterator_t it = {
        (IteratorMove_t) BufferIteratorNext,
        (IteratorMove_t) BufferIteratorPrev,
        (IteratorDone_t) BufferIteratorDone,
        (IteratorItem_t) BufferIteratorItem,
        NULL,
    };

    BufferItOpaque_t *opaque = (BufferItOpaque_t*) it.opaque_data;
    opaque->cur_idx = 0;
    opaque->buffer = buffer;

    return it;
}