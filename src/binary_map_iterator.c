#include "binary_map.h"
#include "linked_list.h"


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
    if (status)
    {
        BalanceMap(opaque->map);
    }

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

Iterator_t NewMapValueIterator(Map_t *map)
{
    Iterator_t it = {
        (IteratorMove_t)MapIteratorNext,
        (IteratorMove_t)MapIteratorPrev,
        (IteratorDone_t)MapIteratorDone,
        (IteratorItem_t)MapIteratorKeyItem,
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
    };

    MapItOpaque_t* opaque = (MapItOpaque_t*) it.opaque_data;
    FlattenMap(map);
    opaque->cur_node = map->root;
    opaque->map = map;

    return it;
}
