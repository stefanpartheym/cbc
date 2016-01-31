/*******************************************************************************
 * Test for CbVariant
 ******************************************************************************/

#include <string.h>

#include "../src/variant.h"
#include "../src/utils.h"
#include "test.h"


/* -------------------------------------------------------------------------- */

static const char* TEST_STRING  = "test string";
static const char* TEST_STRING2 = "another test string";


/* -------------------------------------------------------------------------- */

/*
 * Test constructors and destructors of CbVariant
 */
void variant_alloc_test(void** state)
{
    CbVariant* variant = NULL;

    /* undefined variant */
    variant = cb_variant_create();
    assert_non_null(variant);
    cb_variant_destroy(variant);

    /* integer variant */
    variant = cb_integer_create(123);
    assert_non_null(variant);
    cb_variant_destroy(variant);

    /* float variant */
    variant = cb_float_create(123.000321);
    assert_non_null(variant);
    cb_variant_destroy(variant);

    /* boolean variant */
    variant = cb_boolean_create(true);
    assert_non_null(variant);
    cb_variant_destroy(variant);

    /* string variant */
    variant = cb_string_create(TEST_STRING);
    assert_non_null(variant);
    cb_variant_destroy(variant);
}

/*
 * Test general functionalities
 */
void variant_types_test(void** state)
{
    CbVariant* variant;
    CbVariant* variant2;
    char buffer[32];

    /* undefined variant */
    variant = cb_variant_create();
    assert_true(cb_variant_is_undefined(variant));
    cb_variant_destroy(variant);

    /* integer variant */
    variant = cb_integer_create(123);
    assert_true(cb_variant_is_integer(variant));
    assert_int_equal(123, cb_integer_get_value(variant));
    cb_variant_destroy(variant);

    /* float variant */
    variant = cb_float_create(123.000321);
    assert_true(cb_variant_is_float(variant));
    assert_true(dequal(123.000321, cb_float_get_value(variant)));
    cb_variant_destroy(variant);

    /* boolean variant */
    variant = cb_boolean_create(true);
    assert_true(cb_variant_is_boolean(variant));
    assert_int_equal(true, cb_boolean_get_value(variant));
    cb_variant_destroy(variant);

    /* string variant */
    strcpy(buffer, TEST_STRING);
    strcat(buffer, TEST_STRING2);
    variant = cb_string_create(TEST_STRING);
    assert_true(cb_variant_is_string(variant));
    assert_string_equal(TEST_STRING, cb_string_get_value(variant));
    variant2 = cb_string_create(TEST_STRING2);
    assert_true(cb_variant_is_string(variant2));
    cb_string_concat(variant, variant2);
    assert_string_equal(buffer, cb_string_get_value(variant));
    cb_variant_destroy(variant2);
    cb_variant_destroy(variant);
}

/*
 * Test cb_variant_to_string()
 */
void variant_to_string_test(void** state)
{
    CbVariant* variant;
    char* str;

    /* undefined variant */
    variant = cb_variant_create();
    str     = cb_variant_to_string(variant);
    assert_string_equal("<undefined>", str);
    memfree(str);
    cb_variant_destroy(variant);

    /* integer variant */
    variant = cb_integer_create(123);
    str     = cb_variant_to_string(variant);
    assert_string_equal("123", str);
    memfree(str);
    cb_variant_destroy(variant);

    /* float variant */
    variant = cb_float_create(123.000321);
    str     = cb_variant_to_string(variant);
    assert_string_equal("123.000321", str);
    memfree(str);
    cb_variant_destroy(variant);

    /* boolean variant */
    variant = cb_boolean_create(true);
    str     = cb_variant_to_string(variant);
    assert_string_equal("True", str);
    memfree(str);
    cb_variant_destroy(variant);
    variant = cb_boolean_create(false);
    str     = cb_variant_to_string(variant);
    assert_string_equal("False", str);
    memfree(str);
    cb_variant_destroy(variant);

    /* string variant */
    variant = cb_string_create(TEST_STRING);
    str     = cb_variant_to_string(variant);
    assert_string_equal(TEST_STRING, str);
    memfree(str);
    cb_variant_destroy(variant);
}
