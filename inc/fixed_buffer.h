#ifndef __LIB_FUNDEMENTAL_BUFFER_H__
#define __LIB_FUNDEMENTAL_BUFFER_H__

/**
 * @file fixed_buffer.h
 * Fixed buffer with bounds checking
 */

#include "arena.h"
#include "iterator.h"

#include <string.h>

/**
 * @class Buffer_t
 * @brief Fixed memory buffer with bounds checking
 *
 * The length and data_width elements allow us to perform
 * bounds checking when indexing or inserting
 * elements
 */
typedef struct _fixed_buffer_s
{
    /**
     * @memberof Buffer_t
     * @brief Length, in bytes, of a single data element in the buffer
     */
    Unsigned_t data_width;

    /**
     * @memberof Buffer_t
     * @brief Number of data elements the buffer can contain
     */
    Unsigned_t length;

    /**
     * @memberof Buffer_t
     * @brief Actual buffer data
     */
    Byte_t data[];
} Buffer_t;

/**
 * @def TOTAL_BUFFER_SIZE
 * Return the total amount of memory used by a given buffer
 */
#define TOTAL_BUFFER_SIZE(BUFF) ((BUFF->length * BUFF->data_width) + sizeof(Buffer_t))

/**
 * @public @memberof Buffer_t
 * @brief Initialize guarded buffer over statically
 * or stack-allocated memory region
 * 
 * @param memory The memory to initialize the Buffer_t over
 * @param data_width The width of a single element within the new buffer
 * @param total_length The number of elements contained in the buffer
 */
Buffer_t *InitializeBuffer(Byte_t *memory, Unsigned_t data_width, Unsigned_t length);

/**
 * @public @memberof Buffer_t
 * Create a new buffer
 *
 * @param a The arena to allocate against
 * @param data_width The width of a single element in the new buffer
 * @param total_length The number of elements contained in the buffer
 */
Buffer_t *NewBuffer(Arena_t *a, Unsigned_t data_width, Unsigned_t length);

/**
 * @public @memberof Buffer_t
 * Get a pointer to a given element within a buffer
 *
 * @param b The buffer to index
 * @param idx The offset (from zero) to get the element from
 */
void *BufferIndex(Buffer_t *b, Unsigned_t idx);

/**
 * @public @memberof Buffer_t
 * Write an element to a buffer
 *
 * @param b The buffer to write to
 * @param idx The index to write the element to
 * @param data A pointer to the element
 */
void BufferInsert(Buffer_t *b, Unsigned_t idx, void *data);

/**
 * @public @memberof Buffer_t
 * Copy an element out of a buffer
 *
 * @param b The buffer to extract an element from
 * @param dest The location to copy the element to
 * @param src_idx The index to copy the element from
 */
void BufferCopyElement(Buffer_t *b, void *dest, Unsigned_t src_idx);

/**
 * @public @memberof Buffer_t
 * Clone a buffer into a new arena
 *
 * @param b The buffer to clone
 * @param a The arena to clone the buffer into
 */
Buffer_t *BufferClone(Buffer_t *b, Arena_t *a);

/**
 * @public @memberof Buffer_t
 * Hash the contents of a given buffer
 *
 * @param b The buffer to hash
 */
Unsigned_t BufferHash(Buffer_t *b);

/**
 * Hash contents of a raw buffer using the algorithm
 * used by BufferHash
 *
 * @param buff The raw buffer to hash
 * @param length The length of the raw buffer
 */
Unsigned_t RawBufferHash(Byte_t *buff, Unsigned_t length);

/**
 * @public @memberof Buffer_t
 * @brief Create an iterator over the given buffer
 * 
 * @param buffer The buffer to create the iterator for
 */
Iterator_t NewBufferIterator(Buffer_t *buffer);

#endif