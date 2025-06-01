#include "alignment.h"

uint64_t AlignInteger(uint64_t offset, uint64_t alignment)
{
    if (offset % alignment == 0)
    {
        return offset;
    }

    return offset + alignment - (offset % alignment);
}

void *AlignPointer(void *ptr, uint64_t alignment)
{
    return (void *)AlignInteger((uint64_t)ptr, alignment);
}