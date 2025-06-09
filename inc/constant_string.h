#ifndef __LIB_FUNDEMENTAL_CONST_STR_H__
#define __LIB_FUNDEMENTAL_CONST_STR_H__

/**
 * @file constant_string.h
 * @brief Pool of interned strings
 */

#include "constant_pool.h"

/**
 * @class String_t
 * A constant string. Creating two identical
 * strings will return the same pointer
 * 
 * Allows direct comparisons of strings
 */
typedef ConstantObject_t String_t;
static ConstantPool_t *POOL_STRINGS = NULL;

/**
 * @public @memberof String_t
 * @brief Intern a c-string to the strings constant pool
 *
 * @param str The null terminated string to intern
 */
String_t *NewString(char *str);

/**
 * @public @memberof String_t
 * @brief Create a new iterator over a given
 * string
 * 
 * @param s The String_t object to iterate over
 */
Iterator_t NewStringIterator(String_t *s);

#endif