/*******************************************************************************
 * Tests for CbHashTable
 ******************************************************************************/

#include <stdlib.h>

#include "../src/hash_table.h"
#include "test.h"


/* -------------------------------------------------------------------------- */

int setup_hash_table(void** state)
{
    *state = cb_hash_table_create(16, NULL, free);
    return 0;
}

int teardown_hash_table(void** state)
{
    cb_hash_table_destroy(*state);
    return 0;
}


/* -------------------------------------------------------------------------- */

void hash_table_common_test(void** state)
{
    CbHashTable* ht     = *state;
    TestDummy*   dummy  = dummy_create(123);
    TestDummy*   dummy2 = NULL;

    assert_null(cb_hash_table_get(ht, "test123"));
    cb_hash_table_insert(ht, "test123", dummy);
    dummy2 = cb_hash_table_get(ht, "test123");
    assert_non_null(dummy2);
    assert_int_equal(dummy->id, dummy2->id);
}

void hashtable_insert_remove_test(void** state)
{
    CbHashTable* ht  = *state;
    TestDummy* dummy = NULL;
    int i            = 0;

    assert_int_equal(false, cb_hash_table_remove(ht, "test1"));

    for (; i < 100; i++)
    {
        char key[16];
        sprintf(key, "test_%d", i);

        cb_hash_table_insert(ht, key, dummy_create(i));
        dummy = cb_hash_table_get(ht, key);
        assert_non_null(dummy);
        assert_int_equal(i, dummy->id);
    }

    for (i = 0; i < 100; i++)
    {
        char key[16];

        sprintf(key, "test_%d", i);

        dummy = cb_hash_table_get(ht, key);
        assert_non_null(dummy);
        assert_int_equal(true, cb_hash_table_remove(ht, key));
    }
}
