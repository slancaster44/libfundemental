#include <string.h>
#include <assert.h>

#include "binary_map.h"

MapNode_t *NewMapNode(Arena_t *arena, MapKey_t key, void *value, uint64_t length)
{
    MapNode_t *node = ArenaAllocate(arena, sizeof(MapNode_t) + length);
    node->key = key;
    node->length = length;

    WriteMapNodeValue(node, value, length);
    return node;
}

void WriteMapNodeValue(MapNode_t *mn, void *value, uint64_t length)
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

uint64_t flatten_map(MapNode_t **root)
{
    /* 1) Convert map into a linked-list through (*root)->right */
    MapNode_t fake_root;
    fake_root.right = *root;

    MapNode_t *tail = &fake_root;
    MapNode_t *remaining = *root;
    uint64_t length = 0;
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

uint64_t FlattenMap(Map_t *map)
{
    uint64_t ret_val = flatten_map(&map->root);
    justify_parents(map->root);
    return ret_val;
}

void balance_map(MapNode_t **root)
{
    uint64_t length = flatten_map(root);

    /* 2) Convert back into balanced tree */
    /* num_leaves = length + 1 - 2**(log2(length+1)) */
    uint64_t n = 0;
    for (uint64_t i = length + 1; i >>= 1; n++)
        ;
    uint64_t num_leaves = length + 1ul - ((1ul << (n)));

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
