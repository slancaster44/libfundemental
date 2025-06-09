#include <string.h>
#include <assert.h>

#include "binary_map.h"

MapNode_t *NewMapNode(Arena_t *arena, MapKey_t key, void *value, Unsigned_t length)
{
    MapNode_t *node = ArenaAllocate(arena, sizeof(MapNode_t) + length);
    node->key = key;
    node->length = length;

    WriteMapNodeValue(node, value, length);
    return node;
}

void WriteMapNodeValue(MapNode_t *mn, void *value, Unsigned_t length)
{
    if (value == NULL || length == 0)
    {
        return;
    }

    assert(length == mn->length);
    memcpy(mn->value, value, length);
}

MapNode_t *lookup_node(MapNode_t *root, MapKey_t key)
{
    if (root == NULL || root->key.as_integer == key.as_integer)
    {
        return root;
    }
    else if (key.as_integer > root->key.as_integer)
    {
        return lookup_node(root->right, key);
    }

    return lookup_node(root->left, key);
}

MapNode_t *LookupMapNode(Map_t *map, MapKey_t key)
{
    return lookup_node(map->root, key);
}

void *LookupMapValue(Map_t *map, MapKey_t key)
{
    MapNode_t *mn = lookup_node(map->root, key);
    if (mn != NULL)
    {
        return (void *)mn->value;
    }

    return NULL;
}

bool ExistsInMap(Map_t *map, MapKey_t key)
{
    return lookup_node(map->root, key);
}

void insert_map_node(MapNode_t **root, MapNode_t *new_node)
{
    if (new_node == NULL)
    {
        return;
    }

    insert_map_node(root, new_node->left);
    insert_map_node(root, new_node->right);
    new_node->left = NULL;
    new_node->right = NULL;

    if (*root == NULL)
    {
        *root = new_node;
    }
    else if (new_node->key.as_integer == (*root)->key.as_integer)
    {

        new_node->left = (*root)->left;
        new_node->right = (*root)->right;
        
        if (new_node->right != NULL)
        {
            new_node->right->parent = new_node;
        }

        if (new_node->left != NULL)
        {
            new_node->left->parent = new_node;
        }

        new_node->parent = (*root)->parent;
        *root = new_node;
    }
    else if (new_node->key.as_integer > (*root)->key.as_integer)
    {
        new_node->parent = *root;
        insert_map_node(&(*root)->right, new_node);
    }
    else
    {
        new_node->parent = *root;
        insert_map_node(&(*root)->left, new_node);
    }
}

void InsertMapNode(Map_t *map, MapNode_t *node)
{
    insert_map_node(&map->root, node);
}

void compress(MapNode_t **root, unsigned long count)
{
    MapNode_t *scanner = *root;
    for (int i = 1; i < count; i++)
    {
        MapNode_t *child = scanner->right;
        scanner->right = child->right;
        scanner = scanner->right;
        child->right = scanner->left;
        scanner->left = child;
    }
}

void justify_parents(MapNode_t *root)
{
    if (root == NULL)
    {
        return;
    }

    if (root->left != NULL)
    {
        root->left->parent = root;
    }

    if (root->right != NULL)
    {
        root->right->parent = root;
    }

    justify_parents(root->left);
    justify_parents(root->right);
}

Unsigned_t flatten_map(MapNode_t **root)
{
    /* 1) Convert map into a linked-list through (*root)->right */
    MapNode_t fake_root;
    fake_root.right = *root;

    MapNode_t *tail = &fake_root;
    MapNode_t *remaining = *root;
    Unsigned_t length = 0;
    while (remaining != NULL)
    {
        if (remaining->left == NULL)
        {
            tail = remaining;
            remaining = remaining->right;
            length++;
        }
        else
        {
            MapNode_t *temp = remaining->left;
            remaining->left = temp->right;
            temp->right = remaining;
            remaining = temp;
            tail->right = temp;
        }
    }
    *root = fake_root.right;

    return length;
}

Unsigned_t FlattenMap(Map_t *map)
{
    Unsigned_t ret_val = flatten_map(&map->root);
    justify_parents(map->root);
    return ret_val;
}

void balance_map(MapNode_t **root)
{
    Unsigned_t length = flatten_map(root);

    /* 2) Convert back into balanced tree */
    /* num_leaves = length + 1 - 2**(log2(length+1)) */
    Unsigned_t n = 0;
    for (Unsigned_t i = length + 1; i >>= 1; n++)
        ;
    Unsigned_t num_leaves = length + 1ul - ((1ul << (n)));

    compress(root, num_leaves);
    for (length -= num_leaves; length > 1; length /= 2)
    {
        compress(root, length / 2);
    }

    justify_parents(*root);
}

void BalanceMap(Map_t *map)
{
    balance_map(&map->root);
}

typedef struct _binary_map_it_s
{
    Map_t *map;
    MapNode_t *cur_node;
} MapItOpaque_t;

_Static_assert(sizeof(MapItOpaque_t) <= IT_OPAQUE_DATA_SIZE, "Map iterator opaque size too large");

void MapIteratorNext(MapItOpaque_t *opaque)
{
    opaque->cur_node = opaque->cur_node->right;
}

void MapIteratorPrev(MapItOpaque_t *opaque)
{
    opaque->cur_node = opaque->cur_node->parent;
}

bool MapIteratorDone(MapItOpaque_t *opaque)
{
    bool status = opaque->cur_node == NULL;
    return status;
}

void *MapIteratorKeyItem(MapItOpaque_t *opaque)
{
    return &opaque->cur_node->key;
}

void *MapIteratorValueItem(MapItOpaque_t *opaque)
{
    return opaque->cur_node->value;
}

void CloseMapIterator(MapItOpaque_t *opaque)
{
    BalanceMap(opaque->map);
}

Iterator_t NewMapValueIterator(Map_t *map)
{
    Iterator_t it = {
        (IteratorMove_t)MapIteratorNext,
        (IteratorMove_t)MapIteratorPrev,
        (IteratorDone_t)MapIteratorDone,
        (IteratorItem_t)MapIteratorKeyItem,
        (IteratorClose_t)CloseMapIterator,
    };

    MapItOpaque_t* opaque = (MapItOpaque_t*) it.opaque_data;
    FlattenMap(map);
    opaque->cur_node = map->root;
    opaque->map = map;

    return it;
}

Iterator_t NewMapKeyIterator(Map_t *map)
{
    Iterator_t it = {
        (IteratorMove_t)MapIteratorNext,
        (IteratorMove_t)MapIteratorPrev,
        (IteratorDone_t)MapIteratorDone,
        (IteratorItem_t)MapIteratorValueItem,
        (IteratorClose_t)CloseMapIterator,
    };

    MapItOpaque_t* opaque = (MapItOpaque_t*) it.opaque_data;
    FlattenMap(map);
    opaque->cur_node = map->root;
    opaque->map = map;

    return it;
}
