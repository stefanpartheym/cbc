/*******************************************************************************
 * Tests for Vector
 ******************************************************************************/

#include "../src/vector.h"
#include "test.h"


/* -------------------------------------------------------------------------- */

void vector_common_test(void** state)
{
    Vector* v;
    TestDummy* dummy = NULL;
    int index        = -1;
    
    v = vector_create();
    assert_non_null(v);
    assert_int_equal(0, vector_get_count(v));
    
    dummy = dummy_create(0);
    
    assert_non_null(dummy);
    index = vector_append(v, dummy);
    assert_int_equal(0, index);
    assert_int_equal(1, vector_get_count(v));
    assert_int_equal(dummy, vector_remove(v, index));
    assert_int_equal(0, vector_get_count(v));
    
    dummy_destroy(dummy);
    
    for (index = 0; index < 100; index++)
    {
        int i;
        dummy = dummy_create(index);
        i     = vector_append(v, dummy);
        assert_int_equal(index,     i);
        assert_int_equal(index + 1, vector_get_count(v));
    }
    
    while (vector_get_count(v) > 0)
    {
        int index = vector_get_count(v) - 1;
        dummy     = vector_remove(v, index);
        assert_int_equal(index, vector_get_count(v));
        assert_int_equal(dummy->id, index);
        assert_non_null(dummy);
        dummy_destroy(dummy);
    }
    
    vector_destroy(v);
}

void vector_get_test(void** state)
{
    Vector* v;
    TestDummy* dummy;
    int i;
    v = vector_create();
    
    for (i = 0; i < 5; i++)
        vector_append(v, dummy_create(i));
    
    for (i = 0; i < 5; i++)
    {
        dummy = NULL;
        vector_get(v, i, (VectorItem*) &dummy);
        assert_non_null(dummy);
        assert_int_equal(i, dummy->id);
    }
    
    while (vector_get_count(v) > 0)
    {
        int index       = vector_get_count(v) - 1;
        VectorItem item = vector_remove(v, index);
        assert_non_null(item);
        dummy_destroy(item);
    }
    
    vector_destroy(v);
}
