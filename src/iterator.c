#include "iterator.h"

void IteratorNext(Iterator_t *it)
{
    it->next(it->opaque_data);
}

void IteratorPrevious(Iterator_t *it)
{
    it->prev(it->opaque_data);
}

bool IteratorDone(Iterator_t *it)
{
    return it->done(it->opaque_data);
}

void *IteratorItem(Iterator_t *it)
{
    return it->item(it->opaque_data);
}