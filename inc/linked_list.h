#ifndef __LIB_FUNDEMENTAL_LIST_H__
#define __LIB_FUNDEMENTAL_LIST_H__

/**
 * @file linked_list.h
 * A circular, doubley-linked list implementation.
 * All linked list nodes are allocated against a
 * given arena
 */

#include "arena.h"
#include "iterator.h"

/**
 * @class ListNode_t
 * @brief A linked list element
 */
typedef struct _list_node_s
{
    /**
     * @memberof ListNode_t
     * @brief The next item in the linked list
     */
    struct _list_node_s *next;

    /**
     * @memberof ListNode_t
     * @brief The previous item in the linked list
     */
    struct _list_node_s *previous;

    /**
     * @memberof ListNode_t
     * @brief The length of the data element
     */
    uint64_t length;

    /**
     * @memberof ListNode_t
     * @brief The data stored in this node
     */
    uint8_t data[];
} ListNode_t;

/**
 * @class List_t
 * @brief A linked list
 */
typedef struct _list_s
{
    ListNode_t *first_element;
} List_t;

/**
 * @def LIST_LOOP
 * A macro to make writing 'for' loops over linked lists easier
 */
#define LIST_LOOP(LIST, ELEM_NAME) \
    (ListNode_t ELEM_NAME = LIST->first_element;)

/**
 * @public @memberof ListNode_t
 *
 * Create a list node of a given length, and copy the data into it.
 * The next and previous elements will be initialized to the same node
 *
 * @param arena The arena to allocate against. ARENA_NONE will simply
 * use malloc(). Calling free() on a ListNode_t is only valid if it
 * was allocated against ARENA_NONE
 *
 * @param data The data to copy into the list node. If it is NULL, the
 * node's data section will be left uninitialized
 *
 * @param length The length of the data section. The total size of the
 * node can be given by the LIST_NODE_SIZE macro
 */
ListNode_t *NewListNode(Arena_t *arena, void *data, uint64_t length);

/**
 * @public @memberof ListNode_t
 *
 * Write to the data section of the given list node
 *
 * @param ln The list node to write the given data to
 * @param data The data to write th the list node
 */
void ListNodeWriteData(ListNode_t *ln, void *data);

/**
 * @public @memberof List_t
 * Add a list element to the end of a list
 *
 * @param list The list to modify
 * @param new_node The list node to add to the list
 */
void ListInsertBack(List_t *list, ListNode_t *new_node);

/**
 * @public @memberof List_t
 * Remove an element from the end list
 *
 * @param list The list to modify
 */
ListNode_t *ListRemoveBack(List_t *list);

/**
 * @public @memberof List_t
 * Add a list element to the front of a list
 *
 * @param list The list to modify
 * @param new_node The list node to add to the list
 */
void ListInsertFront(List_t *list, ListNode_t *new_node);

/**
 * @public @memberof List_t
 * Remove an element from the front list
 *
 * @param list The list to modify
 */
ListNode_t *ListRemoveFront(List_t *list);

/**
 * @public @memberof List_t
 * Get the number of elements in a given list
 *
 * @param list The list to get the length of
 */
uint64_t ListLength(List_t *list);

/**
 * @public @memberof List_t
 * Return the ListNode_t at a given index
 *
 * @param list The list to index
 * @param idx The index
 */
ListNode_t *ListIndex(List_t *list, uint64_t idx);

/**
 * @public @memberof List_t
 * @brief Create an iterator over the given list
 * 
 * @param list The list to create the iterator for
 */
Iterator_t NewListIterator(List_t *list);


#endif