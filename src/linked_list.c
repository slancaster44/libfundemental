#include "linked_list.h"

#include <string.h>
#include <assert.h>

ListNode_t *NewListNode(Arena_t *arena, void *data, uint64_t length)
{
    ListNode_t *new_node = ArenaAllocate(arena, length);

    new_node->length = length;
    new_node->previous = new_node;
    new_node->next = new_node;

    ListNodeWriteData(new_node, data);
    return new_node;
}

void ListNodeWriteData(ListNode_t *ln, void *data)
{
    if (data != NULL)
    {
        memcpy(ln->data, data, ln->length);
    }
}

void ListInsertBack(List_t *list, ListNode_t *new_node)
{
    if (list->first_element == NULL)
    {
        list->first_element = new_node;
        return;
    }

    ListNode_t *head_new = new_node;
    ListNode_t *tail_new = new_node->previous;

    ListNode_t *head_old = list->first_element;
    ListNode_t *tail_old = list->first_element->previous;

    tail_old->next = head_new;
    head_old->previous = tail_new;

    tail_new->next = head_old;
    head_new->previous = tail_old;
}

ListNode_t *ListRemoveBack(List_t *list)
{
    assert(list->first_element != NULL);
    ListNode_t *node = list->first_element->previous;

    if (node == list->first_element)
    {
        assert(node->previous == node);
        list->first_element = NULL;
        return node;
    }

    list->first_element->previous = node->previous;
    node->previous->next = list->first_element;

    return node;
}

void ListInsertFront(List_t *list, ListNode_t *new_node)
{
    ListInsertBack(list, new_node);
    list->first_element = list->first_element->previous;
}

ListNode_t *ListRemoveFront(List_t *list)
{
    assert(list->first_element != NULL);
    list->first_element = list->first_element->next;
    return ListRemoveBack(list);
}

uint64_t ListLength(List_t *list)
{
    if (list->first_element == NULL)
    {
        return 0;
    }

    ListNode_t *curr = list->first_element;
    uint64_t count = 0;

    do
    {
        count++;
        curr = curr->next;
    } while (curr != list->first_element);

    return count;
}

ListNode_t *ListIndex(List_t *list, uint64_t idx)
{
    assert(ListLength(list) > idx);

    ListNode_t *curr = list->first_element;
    uint64_t count = 0;

    while (count != idx)
    {
        count++;
        curr = curr->next;
    }

    return curr;
}