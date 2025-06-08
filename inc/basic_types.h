#ifndef __LIB_FUNDEMENTAL_BASIC_TYPES_H__
#define __LIB_FUNDEMENTAL_BASIC_TYPES_H__

/**
 * @file basic_types.h
 * A set of simple integer types, so this way they
 * can be consistently defined across platforms
 */

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>

/**
 * @class Byte_t
 * @brief The smallest addressable unit
 * supported by the machine
 */
typedef unsigned char Byte_t;

/**
 * @class Unsigned_t
 * The word-length unsigned type. Should have
 * same with as void*
 */
typedef size_t Unsigned_t;

/**
 * @class Signed_t
 * Same width as Unsigned_t, but signed
 */
typedef ssize_t Signed_t;

/**
 * @class
 * Boolean true/false value
 */
typedef bool Boolean_t;

#endif