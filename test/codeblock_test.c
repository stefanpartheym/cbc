/*******************************************************************************
 * Tests for Vector
 ******************************************************************************/

#include "../src/codeblock.h"
#include "test.h"


/* -------------------------------------------------------------------------- */

static const char* write_temp_file(const char* content);


/* -------------------------------------------------------------------------- */

void codeblock_common_test(void** state)
{
    const char* const TEST_STRING = "333 + 55 * 7 - 99";
    const int TEST_RESULT         = 619;
    FILE* test_file;
    const CbVariant* result;
    CbCodeblock* cb = cb_codeblock_create();
    
    assert_true(cb_codeblock_parse_string(cb, TEST_STRING));
    assert_true(cb_codeblock_execute(cb));
    result = cb_codeblock_get_result(cb);
    assert_non_null(result);
    assert_cb_integer_equal(TEST_RESULT, result);
    
    test_file = fopen(write_temp_file(TEST_STRING), "r");
    assert_true(cb_codeblock_parse_file(cb, test_file));
    fclose(test_file);
    
    assert_true(cb_codeblock_execute(cb));
    result = cb_codeblock_get_result(cb);
    assert_non_null(result);
    assert_cb_integer_equal(TEST_RESULT, result);
    
    cb_codeblock_destroy(cb);
}


/* -------------------------------------------------------------------------- */

static const char* write_temp_file(const char* content)
{
    const char* file_name = tmpnam(NULL);
    
    FILE* f = fopen(file_name, "w");
    fputs(content, f);
    fclose(f);
    
    return file_name;
}
