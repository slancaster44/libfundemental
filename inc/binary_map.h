#ifndef __LIB_FUNDEMENTAL_BINARY_MAP_H__
#define __LIB_FUNDEMENTAL_BINARY_MAP_H__

/**
 * @file binary_map.h
 * A map implemented as a binary tree. Enables lookup
 * of key-value pairs
 */

#include <stdint.h>
#include <stdbool.h>

#include "arena.h"
#include "iterator.h"

/**
 * @class MapKey_t
 * @brief Map node key
 *
 * May contain an integer or pointer
 * value depending on usage
 */
typedef union _map_key_u
{
    /**
     * @memberof MapKey_t
     * @brief The map key as an integer
     */
    uint64_t as_integer;

    /**
     * @memberof MapKey_t
     * @brief The map key as a pointer
     */
    void *as_pointer;
} MapKey_t __attribute__((transparent_union));

/**
 * @class MapNode_t
 * @brief A binary map key-value pair
 */
typedef struct _map_node_s
{
    /**
     * @memberof MapNode_t
     *
     * @brief The left child of this map node.
     *
     * NULL if there is no left child
     */
    struct _map_node_s *left;

    /**
     * @memberof MapNode_t
     *
     * @brief The right child of this map node.
     *
     * NULL if there is no right child
     */
    struct _map_node_s *right;

    /**
     * @memberof MapNode_t
     * @brief The parent of this map node
     * NULL if this is the top-most node
     */
    struct _map_node_s *parent;

    /**
     * @memberof MapNode_t
     *
     * @brief The key for this map node
     */
    MapKey_t key;

    /**
     * @memberof MapNode_t
     *
     * @brief The length of this map node's value
     */
    uint64_t length;

    /**
     * @memberof MapNode_t
     *
     * @brief The value for this map node
     */
    uint8_t value[];
} MapNode_t;

/**
 * @class Map_t
 * @brief A key-value store implemented using
 * a binary tree
 */
typedef struct _map_s
{
    MapNode_t *root;
} Map_t;

/**
 * @public @memberof MapNode_t
 * @brief Create new map node
 *
 * @param arena The memory arena to allocate the new node against
 * @param key The key for the new key-value pair
 * @param value The value to be stored in the node
 * @param length The length, in bytes, of the vlaue to be stored in the node
 */
MapNode_t *NewMapNode(Arena_t *arena, MapKey_t key, void *value, uint64_t length);

/**
 * @public @memberof MapNode_t
 * @brief Copy a value into a map node
 *
 * @param mn The map node to copy the value to
 * @param value The value to copy
 * @param length The length, in bytes, of the value
 */
void WriteMapNodeValue(MapNode_t *mn, void *value, uint64_t length);

/**
 * @public @memberof Map_t
 * @brief Lookup a node in a map by its key
 *
 * @param map The map to search
 * @param key The key to search for
 */
MapNode_t *LookupMapNode(Map_t *map, MapKey_t key);

/**
 * @public @memberof Map_t
 * @brief Lookup a value in a map by its key
 *
 * @param map The map to search
 * @param key The key to search for
 */
void *LookupMapValue(Map_t *map, MapKey_t key);

/**
 * @public @memberof Map_t
 * @brief Returns 'true' if a key exists in a map
 *
 * @param map The map to search
 * @param key The key to search for
 */
bool ExistsInMap(Map_t *map, MapKey_t key);

/**
 * @public @memberof Map_t
 * @brief Insert a node into a map
 *
 * @param map The map to modify
 * @param node The node to insert
 */
void InsertMapNode(Map_t *map, MapNode_t *node);

/**
 * @public @memberof Map_t
 * @brief Balance the map's internal binary tree
 * so that searches take O(log(n)) time
 *
 * @param map The map to balance
 */
void BalanceMap(Map_t *map);

/** 
 * @public @memberof Map_t
 * @brief Flatten the map so all nodes can be
 * found by following node->right
 * 
 * @param map The map to flatten
*/
uint64_t FlattenMap(Map_t *map);

/**
 * @public @memberof Map_t
 * @brief Create an iterator over the keys in 
 * a map
 * 
 * @param map The map to create an iterator for
 */
Iterator_t NewMapKeyIterator(Map_t *map);

/**
 * @public @memberof Map_t
 * @brief Create an iterator over the values in 
 * a map
 * 
 * @param map The map to create an iterator for
 */
Iterator_t NewMapValueIterator(Map_t *map);

#endif