/*******************************************************************************
 * Tests for CbStack
 ******************************************************************************/

#include <stdlib.h>

#include "../src/stack.h"
#include "test_utils.h"
#include "test.h"


/* -------------------------------------------------------------------------- */

int setup_stack(void** state)
{
    *state = cb_stack_create();
    return 0;
}

int teardown_stack(void** state)
{
    cb_stack_destroy(*state);
    return 0;
}


/* -------------------------------------------------------------------------- */

void stack_common_test(void** state)
{
    CbStack* stack = *state;
    int integer    = 123;
    int* dest      = NULL;
    
    assert_true(cb_stack_is_empty(stack));
    
    cb_stack_push(stack, &integer);
    
    assert_false(cb_stack_is_empty(stack));
    assert_true(cb_stack_pop(stack, (void**) &dest));
    assert_true(cb_stack_is_empty(stack));
    assert_int_equal(integer, *dest);
    assert_false(cb_stack_pop(stack, (void**) &dest));
}

void stack_push_pop_test(void** state)
{
    CbStack* stack = *state;
    int i          = 0;
    
    assert_int_equal(0, cb_stack_count(stack));
    assert_true(cb_stack_is_empty(stack));
    assert_null(cb_stack_get_top_item(stack));
    
    /* push 100 items onto the stack */
    for (; i < 100; i++)
    {
        TestDummy* dummy = dummy_create(i);
        cb_stack_push(stack, dummy);
        assert_int_equal(i + 1, cb_stack_count(stack));
        assert_int_equal(dummy, cb_stack_get_top_item(stack));
    }
    
    assert_false(cb_stack_is_empty(stack));
    
    /* pop 100 items off the stack */
    for (; i > 0; i--)
    {
        TestDummy* dummy = NULL;
        assert_true(cb_stack_pop(stack, (void**) &dummy));
        assert_int_equal(i - 1, cb_stack_count(stack));
        assert_non_null(dummy);
        assert_int_equal(i - 1, dummy->id);
        free(dummy);
    }
}
