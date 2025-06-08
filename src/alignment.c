#include "alignment.h"

Unsigned_t AlignInteger(Unsigned_t offset, Unsigned_t alignment)
{
    if (offset % alignment == 0)
    {
        return offset;
    }

    return offset + alignment - (offset % alignment);
}

void *AlignPointer(void *ptr, Unsigned_t alignment)
{
    return (void *)AlignInteger((Unsigned_t)ptr, alignment);
}