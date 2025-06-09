#ifndef __LIB_FUNDEMENTAL_CONST_POOL_H__
#define __LIB_FUNDEMENTAL_CONST_POOL_H__

/**
 * @file constant_pool.h
 *
 * A data structure for interning constant objects, so
 * that memory doesn't need to be allocated for identical
 * objects
 */

#include "linked_list.h"

/**
 * @private
 * The number of hash buckets in the
 * constant pool
 */
#define NUM_INTERN_BUCKETS 512

/**
 * @class ConstantPool_t
 *
 * @brief A pool of constant objects \n
 *
 * Interning a value against a pool will either: \n
 *  A) Add a new constant object to the pool and return a pointer to
 *     that constant object \n
 *  B) If the an identical constant object already exists in the pool,
 *     return a pointer to that constant object
 */
typedef struct _intern_pool_s
{
    /**
     * @memberof ConstantPool_t
     * @brief The arena to allocate constant objects against
     */
    Arena_t *arena;
    /** Hash buckets for allocated constant objects */
    List_t buckets[NUM_INTERN_BUCKETS];
} ConstantPool_t;

/**
 * @class ConstantObject_t
 *
 * @brief Created when interning a value against a constant
 * pool
 */
typedef struct _const_obj_s
{
    /**
     * @memberof ConstantObject_t
     * @brief A hash of the data field
     */
    Unsigned_t hash;

    /**
     * @memberof ConstantObject_t
     * @brief The length of the data field
     */
    Unsigned_t length;

    /**
     * @memberof ConstantObject_t
     * @brief The value of the constant object.
     *
     * This can be cast to the type originally interned
     */
    Byte_t value[];
} ConstantObject_t;

/**
 * @public @memberof ConstantPool_t
 * Create a new constant pool
 *
 * @param a The arena that constant objects will be
 * allocated against. If the objects will be used for
 * the length of the program, it is recommended that
 * ARENA_PERM is used
 */
ConstantPool_t *NewConstantPool(Arena_t *a);

/**
 * @public @memberof ConstantPool_t
 * Add a new value of the given length to a constant pool
 *
 * @param pool The pool to intern the value against
 * @param value A pointer to the value
 * @param length The length of the value
 */
ConstantObject_t *NewConstant(ConstantPool_t *pool, void *value, Unsigned_t length);

#endif