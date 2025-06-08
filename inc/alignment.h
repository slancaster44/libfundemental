#ifndef __LIB_FUNDEMENTAL_ALIGNMENT_H__
#define __LIB_FUNDEMENTAL_ALIGNMENT_H__

/**
 * @file alignment.h
 * Memory alignment utilities
 */

#include "basic_types.h"

/**
 * @def MACHINE_ALIGNMENT
 * The size, in bytes, of the largest alignment of the machine.
 */
#define MACHINE_ALIGNMENT __BIGGEST_ALIGNMENT__

/**
 * @def ATTRIBUTE_ALIGNED
 * An attribute to tell the compiler to properly align a
 * data type
 */
#define ATTRIBUTE_ALIGNED __attribute__((aligned(__BIGGEST_ALIGNMENT__)))

/**
 * Return 'offset' incremented by the amount required to
 * align 'offset' to the byte boundary 'alignment'
 *
 * @param offset the offset to align
 * @param alignment the byte-boundary to align to
 */
Unsigned_t AlignInteger(Unsigned_t offset, Unsigned_t alignment);

/**
 * Return 'ptr' incremented by the amount required to
 * align 'ptr' to the byte boundary 'alignment'
 *
 * @param ptr the void pointer to align
 * @param alignment the byte-boundary to align to
 */
void *AlignPointer(void *ptr, Unsigned_t alignment);

#endif