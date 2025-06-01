#include "fixed_buffer.h"

typedef struct _fixed_buffer_it_s
{
    Buffer_t *buffer;
    uint64_t cur_idx;
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
    };

    BufferItOpaque_t *opaque = (BufferItOpaque_t*) it.opaque_data;
    opaque->cur_idx = 0;
    opaque->buffer = buffer;

    return it;
}