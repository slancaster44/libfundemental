#ifndef __LIB_FUNDEMENTAL_ALIGNMENT_H__
#define __LIB_FUNDEMENTAL_ALIGNMENT_H__

/**
 * @file alignment.h
 * Memory alignment utilities
 */

#include <stdint.h>

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
uint64_t AlignInteger(uint64_t offset, uint64_t alignment);

/**
 * Return 'ptr' incremented by the amount required to
 * align 'ptr' to the byte boundary 'alignment'
 *
 * @param ptr the void pointer to align
 * @param alignment the byte-boundary to align to
 */
void *AlignPointer(void *ptr, uint64_t alignment);

#endif