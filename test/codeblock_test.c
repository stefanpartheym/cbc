/*******************************************************************************
 * Tests for the CbCodeblock structure
 ******************************************************************************/

#include "../src/codeblock.h"
#include "test.h"


/* -------------------------------------------------------------------------- */

static FILE* write_temp_file(const char* content);


/* -------------------------------------------------------------------------- */

void codeblock_common_test(void** state)
{
    const char* const TEST_STRING = "333 + 55 * 7 - 99,";
    const char* const FAIL_STRING = "undefined_var,";
    const int TEST_RESULT         = 619;
    FILE* test_file;
    const CbVariant* result;
    CbCodeblock* cb = cb_codeblock_create();

    assert_true(cb_codeblock_parse_string(cb, TEST_STRING));
    assert_true(cb_codeblock_execute(cb));
    result = cb_codeblock_get_result(cb);
    assert_non_null(result);
    assert_cb_integer_equal(TEST_RESULT, result);

    /* invalid codeblock */
    assert_true(cb_codeblock_parse_string(cb, FAIL_STRING));
    assert_false(cb_codeblock_execute(cb));

    /* empty codeblock */
    assert_true(cb_codeblock_parse_string(cb, ""));
    assert_true(cb_codeblock_execute(cb));

    /* codeblock from a file */
    test_file = write_temp_file(TEST_STRING);
    assert_true(cb_codeblock_parse_file(cb, test_file));
    fclose(test_file);

    assert_true(cb_codeblock_execute(cb));
    result = cb_codeblock_get_result(cb);
    assert_non_null(result);
    assert_cb_integer_equal(TEST_RESULT, result);

    cb_codeblock_destroy(cb);
}


/* -------------------------------------------------------------------------- */

static FILE* write_temp_file(const char* content)
{
    FILE* f = tmpfile();
    fputs(content, f);

    freopen(NULL, "r", f); /* reopen file for read operations */
    fseek(f, 0, SEEK_SET); /* goto beginning of file */

    return f;
}
