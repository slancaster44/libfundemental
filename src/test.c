#include <stdio.h>
#include <string.h>

#include "basic_types.h"
#include "alignment.h"
#include "arena.h"
#include "fixed_buffer.h"
#include "linked_list.h"
#include "constant_pool.h"
#include "binary_map.h"

static int num_failed;
static int num_passed;

void Fail(const char *msg)
{
    num_failed++;
    printf("\033[0;31m[FAIL]\033[0;37m %s\n", msg);
}

void Pass(const char *msg)
{
    num_passed++;
    printf("\033[0;32m[PASS]\033[0;37m %s\n", msg);
}

#define TEST(CONDITION, MSG) \
    {                        \
        if (CONDITION)       \
        {                    \
            Pass(MSG);       \
        }                    \
        else                 \
        {                    \
            Fail(MSG);       \
        }                    \
    }

void TestAlignment()
{
    TEST(AlignInteger(0, MACHINE_ALIGNMENT) == 0, "Align zero")
    TEST(AlignInteger(MACHINE_ALIGNMENT / 2, MACHINE_ALIGNMENT) == MACHINE_ALIGNMENT, "Align non-zero")
    TEST(AlignInteger(MACHINE_ALIGNMENT, MACHINE_ALIGNMENT) == MACHINE_ALIGNMENT, "Align alignment")
    TEST(AlignInteger(MACHINE_ALIGNMENT + (MACHINE_ALIGNMENT / 2), MACHINE_ALIGNMENT) == (MACHINE_ALIGNMENT * 2), "Align bigger than alignment")
}

int TestArena()
{
    Arena_t a;
    ConstructArena(&a);

    unsigned char *value = ArenaAllocate(&a, ARENA_BLOCK_SIZE * 2);
    memset(value, 0xdc, ARENA_BLOCK_SIZE * 2);

    for (unsigned long k = 0; k < (ARENA_BLOCK_SIZE * 2); k++)
    {
        if (value[k] != 0xdc)
        {
            return (int)k;
        }
    }

    for (unsigned long j = 1; j < ARENA_BLOCK_SIZE; j *= 2)
    {
        value = (unsigned char *)ArenaAllocate(&a, j);
        memset(value, 0xdc, j);

        for (unsigned long k = 0; k < j; k++)
        {
            if (value[k] != 0xdc)
            {
                return (int)k;
            }
        }
    }

    value = (unsigned char *)ArenaAllocate(&a, ARENA_BLOCK_SIZE * 2);
    memset(value, 0xdc, ARENA_BLOCK_SIZE * 2);

    for (unsigned long k = 0; k < (ARENA_BLOCK_SIZE * 2); k++)
    {
        if (value[k] != 0xdc)
        {
            return (int)k;
        }
    }

    DeconstructArena(&a);
    return 0;
}

Unsigned_t TestBuffer()
{
    Arena_t a;
    ConstructArena(&a);

    Buffer_t *buff = NewBuffer(&a, sizeof(Unsigned_t), 100);
    for (Unsigned_t i = 0; i < 100; i++)
    {
        BufferInsert(buff, i, &i);
    }

    for (Unsigned_t i = 0; i < 100; i++)
    {
        if ((*(Unsigned_t *)BufferIndex(buff, i)) != i)
        {
            return i;
        }
    }

    Buffer_t *buff2 = BufferClone(buff, &a);
    for (Unsigned_t i = 0; i < 100; i++)
    {
        if (BufferIndex(buff, i) != BufferIndex(buff2, i))
        {
            return i;
        }
    }

    Unsigned_t i = 0;
    for (Iterator_t it = NewBufferIterator(buff2); !IteratorDone(&it); IteratorNext(&it))
    {
        Unsigned_t *j = IteratorItem(&it);
        if (*j != i)
        {
            return i;
        }

        i++;
    }

    DeconstructArena(&a);
    return 0;
}

Unsigned_t TestList()
{
    Arena_t a;
    ConstructArena(&a);

    List_t list = {NULL};
    TEST(ListLength(&list) == 0, "Empty list length");

    for (Unsigned_t i = 1; i < 100; i++)
    {
        ListNode_t *ln = NewListNode(&a, &i, sizeof(Unsigned_t));
        ListInsertBack(&list, ln);
    }

    TEST(ListLength(&list) == 99, "Filled out list length");

    Unsigned_t i = 1;
    ListNode_t *curr = list.first_element;
    do
    {
        if ((*(Unsigned_t *)curr->data) != i)
        {
            return i;
        }

        i++;
        curr = curr->next;
    } while (curr != list.first_element);

    i = 99;
    curr = list.first_element->previous;
    do
    {
        if ((*(Unsigned_t *)curr->data) != i)
        {
            return i;
        }

        i--;
        curr = curr->next;
    } while (curr != list.first_element);

    for (Unsigned_t i = 99; i > 1; i--)
    {
        curr = ListRemoveBack(&list);
        if ((*(Unsigned_t *)curr->data) != i)
        {
            return i;
        }
    }
    TEST(ListLength(&list) == 1, "Mostly emptied list length");
    ListRemoveBack(&list);

    if (list.first_element != NULL)
    {
        return 1;
    }
    TEST(ListLength(&list) == 0, "Emptied list length");

    // List Front

    for (Unsigned_t i = 1; i < 100; i++)
    {
        ListNode_t *ln = NewListNode(&a, &i, sizeof(Unsigned_t));
        ListInsertFront(&list, ln);
    }

    i = 99;
    for (Iterator_t it = NewListIterator(&list); !IteratorDone(&it); IteratorNext(&it))
    {
        Unsigned_t *cur_num = IteratorItem(&it);

        if (*cur_num != i)
        {
            return i;
        }

        i--;
    }

    i = 99;
    curr = list.first_element;
    do
    {
        if ((*(Unsigned_t *)curr->data) != i)
        {
            return i;
        }

        i--;
        curr = curr->next;
    } while (curr != list.first_element);

    i = 1;
    Iterator_t it = NewListIterator(&list);
    IteratorPrevious(&it); //The first item is 99 not 1. Skip over it

    for (; !IteratorDone(&it); IteratorPrevious(&it))
    {
        Unsigned_t *cur_num = IteratorItem(&it);
        if (*cur_num != i)
        {
            return i;
        }

        i++;
    }

    i = 1;
    curr = list.first_element->previous;
    do
    {
        if ((*(Unsigned_t *)curr->data) != i)
        {
            return i;
        }

        i++;
        curr = curr->next;
    } while (curr != list.first_element);

    for (Unsigned_t i = 99; i > 1; i--)
    {
        curr = ListRemoveFront(&list);
        if ((*(Unsigned_t *)curr->data) != i)
        {
            return i;
        }
    }
    ListRemoveFront(&list);

    if (list.first_element != NULL)
    {
        return 1;
    }

    DeconstructArena(&a);
    return 0;
}

int check_parent_consistency(MapNode_t *root)
{
    if (root == NULL)
    {
        return 0;
    }

    if (root->left != NULL && root->left->parent != root)
    {
        return 1;
    }

    if (root->right != NULL && root->right->parent != root)
    {
        return 2;
    }

    return check_parent_consistency(root->left) + check_parent_consistency(root->right);
}

int test_map()
{
    Arena_t arena;
    ConstructArena(&arena);

    Map_t map = {NULL};
    for (Unsigned_t i = 44; i < 150; i++)
    {
        Unsigned_t i_plus_one = ((i * 2) + 1) % 100;
        InsertMapNode(&map, NewMapNode(&arena, i_plus_one, &i_plus_one, sizeof(Unsigned_t)));
        InsertMapNode(&map, NewMapNode(&arena, i, &i, sizeof(Unsigned_t)));

        if (check_parent_consistency(map.root) != 0)
        {
            return 1;
        }
    }

    Map_t map_two = {NULL};
    for (Unsigned_t i = 0; i < 200; i++)
    {
        Unsigned_t i_plus_one = (((i * 2) + 1) + 100) % 200;
        InsertMapNode(&map_two, NewMapNode(&arena, i_plus_one, &i_plus_one, sizeof(Unsigned_t)));
        InsertMapNode(&map_two, NewMapNode(&arena, i, &i, sizeof(Unsigned_t)));

        if (check_parent_consistency(map.root) != 0)
        {
            return 1;
        }
    }

    InsertMapNode(&map, map_two.root);
    BalanceMap(&map);

    if (check_parent_consistency(map.root) != 0)
    {
        return 1;
    }

    for (Unsigned_t i = 0; i < 200; i++)
    {
        MapNode_t *node = LookupMapNode(&map, i);
        int *num_ptr = (int *)node->value;

        if (*num_ptr != i)
        {
            return 1;
        }
    }

    Unsigned_t key_sum = 0;
    for (Iterator_t it = NewMapKeyIterator(&map); !IteratorDone(&it); IteratorNext(&it))
    {
        key_sum += (*(Unsigned_t*) IteratorItem(&it));
    }

    if (map.root->right->left == NULL)
    {
        return 2;
    }

    if (check_parent_consistency(map.root) != 0)
    {
        return 1;
    }

    DeconstructArena(&arena);
    return 0;
}

int test_string_interning()
{
    ConstantPool_t *pool = NewConstantPool(&ARENA_PERM);

    ConstantObject_t *hello = NewString(pool, "Hello");
    ConstantObject_t *hallo = NewString(pool, "Hello");
    ConstantObject_t *not_hello = NewConstant(pool, "AAA\0AAAAA", 9);

    if (hello != hallo)
    {
        return 1;
    }

    if (strcmp((char *)hallo->value, (char *)hello->value) != 0)
    {
        return 2;
    }

    if (strcmp((char *)hello->value, "Hello") != 0)
    {
        return 3;
    }

    for (int i = 0; i < 9; i++)
    {
        if (i == 3 && not_hello->value[i] != '\0')
        {
            return 4;
        }

        if (i != 3 && not_hello->value[i] != 'A')
        {
            return 5;
        }
    }

    return 0;
}

int main()
{
    TestAlignment();
    TEST(TestArena() == 0, "Arena test")
    TEST(TestBuffer() == 0, "Buffer testing")
    TEST(TestList() == 0, "List test")
    TEST(test_string_interning() == 0, "String interning")
    TEST(test_map() == 0, "Map test")

    printf("Tests Passed: %d\nTests Failed: %d\n", num_passed, num_failed);
    return 0;
}