#ifndef __LIB_FUNDEMENTAL_ITERATOR_H__
#define __LIB_FUNDEMENTAL_ITERATOR_H__

/**
 * @file iterator.h
 *
 * An implementation of the iterator pattern
 * for the various collection objects in this
 * library
 */

#include <stdbool.h>
#include "basic_types.h"

typedef void (*IteratorMove_t)(void *opaque);
typedef bool (*IteratorDone_t)(void *opaque);
typedef void *(*IteratorItem_t)(void *opaque);
typedef void (*IteratorClose_t)(void *opaque);

/**
 * @def IT_OPAQUE_DATA_SIZE
 * @brief The maximum size of an iterator's opaque
 * data
 */
#define IT_OPAQUE_DATA_SIZE 32

/**
 * @class Iterator_t
 *
 * @brief The iterator object. Used for looping
 * over a collection objects (eg List_t, Buffer_t).
 * 
 * It is strongly recommended that data structions not
 * be modified while they are being iterated over.
 *
 * Example usage on a buffer of integers:
 * @code
 *  for (Iterator_t it = NewBufferIterator(buffer); !IteratorDone(&it); IteratorNext(&it))
 *  {
 *      Unsigned_t *my_number_ptr = IteratorItem(&it);
 *  }
 * @endcode
 */
typedef struct _iterator_s
{
    IteratorMove_t next;
    IteratorMove_t prev;
    IteratorDone_t done;
    IteratorItem_t item;
    IteratorClose_t close;
    Byte_t opaque_data[IT_OPAQUE_DATA_SIZE];
} Iterator_t;

/**
 * @public @memberof Iterator_t
 * @brief Move the iterator to the next object
 *
 * Used at the end of an iteration to move the
 * iterator onto the next object.
 *
 * @param it The iterator to increment
 */
void IteratorNext(Iterator_t *it);

/**
 * @public @memberof Iterator_t
 * @brief Move the iterator to the previous object
 *
 * Used at the end of an iteration to move the
 * iterator onto the previous object.
 *
 * @param it The iterator to decrement
 */
void IteratorPrevious(Iterator_t *it);

/**
 * @public @memberof Iterator_t
 * @brief Test if the iterator is done.
 *
 * This method will return 'true' if the iterator
 * has passed the final object in the collection. If
 * there are still objects remaining, it will return 'false'
 *
 * @param it The iterator to test against
 */
bool IteratorDone(Iterator_t *it);

/**
 * @public @memberof Iterator_t
 * @brief Returns a void* to the current item in the collection
 *
 * @param it The iterator to pull an item from
 */
void *IteratorItem(Iterator_t *it);

/**
 * @public @memberof Iterator_t
 * @brief Cleanup iterator
 * 
 * @param it The iterator to cleanup
 */
void IteratorClose(Iterator_t *it);

#endif