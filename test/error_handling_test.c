/*******************************************************************************
 * Tests for error handling
 ******************************************************************************/

#include "../src/error_handling.h"
#include "test.h"


/* -------------------------------------------------------------------------- */

/*
 * Setup
 */
int setup_error_handling(void** state)
{
    FILE* test_output = tmpfile();
    cb_error_initialize(test_output);

    *state = test_output;

    return 0;
}

/*
 * Teardown
 */
int teardown_error_handling(void** state)
{
    cb_error_finalize();
    fclose(*state);

    return 0;
}

/*
 * Resetup
 */
void resetup_error_handling(void** state)
{
    teardown_error_handling(state);
    setup_error_handling(state);
}


/* -------------------------------------------------------------------------- */

void error_print_test(void** state)
{
    char stream_content[64];

    cb_error_print(CB_ERROR_SYNTAX, 1, "test error");

    stream_to_string(*state, stream_content, true);
    assert_string_equal("syntax error: line 1: test error", stream_content);
}

void error_init_test(void** state)
{
    assert_true(cb_error_is_initialized());
}

void error_handling_test(void** state)
{
    char stream_content[64];

    assert_false(cb_error_occurred());
    cb_error_trigger(CB_ERROR_RUNTIME, 1, "test error");
    assert_true(cb_error_occurred());
    cb_error_process();
    assert_false(cb_error_occurred());

    stream_to_string(*state, stream_content, true);
    assert_string_equal("runtime error: line 1: test error", stream_content);
}
