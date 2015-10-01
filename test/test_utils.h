/*******************************************************************************
 * Collection of custom unit-testing utilities and custom assert-functions
 * for the cmocka unit-test framework
 ******************************************************************************/

#ifndef TEST_UTILS_H
#define TEST_UTILS_H

/* includes requiered for the cmocka unit-test framework */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
/* other includes */
#include <stdio.h>
#include <stdbool.h>

#include "../src/variant.h"


typedef struct TestDummy TestDummy;
struct TestDummy
{
    int id;
};


/* -------------------------------------------------------------------------- */

TestDummy* dummy_create(int id);
void dummy_destroy(TestDummy* self);


/* -------------------------------------------------------------------------- */


#define assert_cb_variant_equal(expected, actual) \
    _assert_cb_variant_equal(expected, actual, __FILE__, __LINE__)
#define assert_cb_integer_equal(expected, actual) \
    _assert_cb_integer_equal(expected, actual, __FILE__, __LINE__)
#define assert_cb_float_equal(expected, actual) \
    _assert_cb_float_equal(expected, actual, __FILE__, __LINE__)
#define assert_cb_boolean_equal(expected, actual) \
    _assert_cb_boolean_equal(expected, actual, __FILE__, __LINE__)

/*
 * Check if CbVariant structs are equals
 */
void _assert_cb_variant_equal(const CbVariant* expected,
                              const CbVariant* actual,
                              const char * const file,
                              const int line);

/*
 * Check if CbVariant has a specific integer value
 */
void _assert_cb_integer_equal(const CbIntegerDataType expected,
                              const CbVariant* actual,
                              const char * const file,
                              const int line);

/*
 * Check if CbVariant has a specific float value
 */
void _assert_cb_float_equal(const CbFloatDataType expected,
                            const CbVariant* actual,
                            const char * const file,
                            const int line);

/*
 * Check if CbVariant has a specific boolean value
 */
void _assert_cb_boolean_equal(const CbBooleanDataType expected,
                              const CbVariant* actual,
                              const char * const file,
                              const int line);

/*
 * Copy stream content to a string
 * 
 * NOTE: Arguemnt string must be allocated before this function is called!
 */
void stream_to_string(FILE* stream, char* string, bool trim);


#endif /* TEST_UTILS_H */
