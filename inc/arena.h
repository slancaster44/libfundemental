#ifndef __LIB_FUNDEMENTAL_ARENA_H__
#define __LIB_FUNDEMENTAL_ARENA_H__

/**
 * @file arena.h
 * Memory management arena implementation. Enables grouping
 * allocations together, so they may be free'd together
 */

#include "basic_types.h"
#include <stddef.h>

/**
 * @private
 * Basic unit of allocation
 */
typedef struct _arena_block_s
{
    struct _arena_block_s *next;
    Byte_t *start;
    Byte_t *end;
} ArenaBlock_t;

/**
 * @private
 * @def ARENA_BLOCK_SIZE
 * Size of arena block when allocated. When the block is full
 * a new block will be allocated using 'malloc().' When an
 * allocation is larger than ARENA_BLOCK_SIZE, a special block,
 * large enough for the allocation, will be malloc'd
 */
#define ARENA_BLOCK_SIZE 4096

/**
 * @private
 * @def ARENA_BLOCK_MAX_MEM
 * The maxium amount of free memory that could be in an
 * arena.
 */
#define ARENA_BLOCK_MAX_MEM (ARENA_BLOCK_SIZE - sizeof(ArenaBlock_t))

/**
 * @class Arena_t
 * @brief A memory allocation arena
 *
 * All values allocated against a given
 * arena will be freed when the arena is deconstructed
 */
typedef struct _arena_s
{
    ArenaBlock_t *blocks;
} Arena_t;

/**
 * The permanent arena. This arena will be
 * initialized on program startup, and deconstructed
 * on program exit. Allows dynamic allocation of objects
 * that will last the life of the program
 */
static Arena_t ARENA_PERM = {NULL};

/**
 * The 'none' arena. This is not a real arena. Allocations
 * against this arena will simply call malloc. It is up to the
 * user to free the returned memory themselves.
 */
static Arena_t ARENA_NONE = {(void *)-1};

/**
 * @public @memberof Arena_t
 *
 * Initialize the given arena
 * @param a The arena to initialize
 */
void ConstructArena(Arena_t *a);

/**
 * @public @memberof Arena_t
 *
 * Cleanup the given arena. Free the associated
 * memory blocks
 *
 * @param a The arena to cleanup
 */
void DeconstructArena(Arena_t *a);

/**
 * @public @memberof Arena_t
 *
 * Allocate a buffer of length 'size' to an arena. If
 * the arena is ARENA_NONE, then simply use 'malloc()'
 * from the standard library
 *
 * @param a The arena to allocate to
 * @param size The length of the allocated buffer
 */
void *ArenaAllocate(Arena_t *a, Unsigned_t size);

#endif