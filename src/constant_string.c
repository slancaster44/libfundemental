#include <string.h>
#include "constant_string.h"

String_t *NewString(char *str)
{
    if (POOL_STRINGS == NULL)
    {
        POOL_STRINGS = NewConstantPool(&ARENA_PERM);
    }

    return NewConstant(POOL_STRINGS, str, strlen(str));
}

typedef struct _string_it_s
{
    String_t *str;
    Unsigned_t idx;
} StringIterator_t;

_Static_assert(sizeof(StringIterator_t) <= IT_OPAQUE_DATA_SIZE, "String it size too large");

void StringIteratorNext(StringIterator_t *it)
{
    it->idx++;
}

void StringIteratorPrev(StringIterator_t *it)
{
    it->idx--;
}

bool StringIteratorDone(StringIterator_t *it)
{
    return it->idx >= it->str->length;
}

void *StringIteratorItem(StringIterator_t *it)
{
    return &it->str->value[it->idx];
}

Iterator_t NewStringIterator(String_t *s)
{
    Iterator_t it = {
        (IteratorMove_t)StringIteratorNext,
        (IteratorMove_t)StringIteratorPrev,
        (IteratorDone_t)StringIteratorDone,
        (IteratorItem_t)StringIteratorItem,
        NULL,
    };

    StringIterator_t *opaque = (StringIterator_t*) it.opaque_data;
    opaque->idx = 0;
    opaque->str = s;

    return it;
}