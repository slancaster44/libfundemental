#include "constant_pool.h"
#include "fixed_buffer.h"

ConstantPool_t *NewConstantPool(Arena_t *a)
{
    ConstantPool_t *pool = ArenaAllocate(a, sizeof(ConstantPool_t));
    pool->arena = a;
    return pool;
}

ConstantObject_t *NewConstant(ConstantPool_t *pool, void *value, Unsigned_t length)
{
    if (length == 0)
    {
        return NULL;
    }

    Byte_t *buff = ((Byte_t *)value);
    Unsigned_t bucket_hash = RawBufferHash(buff, length);
    Unsigned_t bucket_idx = bucket_hash % NUM_INTERN_BUCKETS;

    ListNode_t **b;
    for (b = &(pool->buckets[bucket_idx].first_element); *b != NULL; b = &((*b)->next))
    {
        ConstantObject_t *stored_obj = (ConstantObject_t *)(*b)->data;
        if ((stored_obj->hash != bucket_hash) || (stored_obj->length != length))
        {
            continue;
        }

        /* If we've seen this string before, return original copy */
        Unsigned_t i;
        for (i = 0; i < length; i++)
        {
            if (stored_obj->value[i] != buff[i])
            {
                break;
            }
        }

        if (i == length)
        {
            return stored_obj;
        }
    }

    ListNode_t *bucket = NewListNode(pool->arena, NULL, length + sizeof(ConstantObject_t));
    *b = bucket;

    ConstantObject_t *obj = (ConstantObject_t *)bucket->data;
    obj->length = length;
    obj->hash = bucket_hash;
    memcpy(obj->value, value, length);

    return obj;
}

ConstantObject_t *NewString(ConstantPool_t *pool, char *str)
{
    return NewConstant(pool, str, strlen(str));
}