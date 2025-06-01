#include "linked_list.h"

typedef struct _list_iterator_opaque_s
{
    ListNode_t *first_node;
    ListNode_t *cur_node;
    int64_t count;
} ListItOpaque_t;

_Static_assert(sizeof(ListItOpaque_t) <= IT_OPAQUE_DATA_SIZE, "List iterator opaque size too large");

void ListIteratorNext(ListItOpaque_t *opaque)
{
    opaque->cur_node = opaque->cur_node->next;
    opaque->count++;
}

void ListIteratorPrev(ListItOpaque_t *opaque)
{
    opaque->cur_node = opaque->cur_node->previous;
    opaque->count--;
}

bool ListIteratorDone(ListItOpaque_t *opaque)
{
    return opaque->count != 0 && opaque->cur_node == opaque->first_node;
}

void *ListIteratorItem(ListItOpaque_t *opaque)
{
    return opaque->cur_node->data;
}

Iterator_t NewListIterator(List_t *list)
{
    Iterator_t it = {
        (IteratorMove_t) ListIteratorNext,
        (IteratorMove_t) ListIteratorPrev,
        (IteratorDone_t) ListIteratorDone,
        (IteratorItem_t) ListIteratorItem,
    };
    
    ListItOpaque_t *opaque = (ListItOpaque_t*) it.opaque_data;
    opaque->cur_node = list->first_element;
    opaque->first_node = list->first_element;
    opaque->count = 0;

    return it;
}
