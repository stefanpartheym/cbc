#include <ctype.h>
#include <stdlib.h>

#include "../src/utils.h"
#include "test_utils.h"


/* -------------------------------------------------------------------------- */

TestDummy* dummy_create(int id)
{
    TestDummy* self = malloc(sizeof(TestDummy));
    self->id = id;
    return self;
}

void dummy_destroy(TestDummy* self)
{
    free(self);
}


/* -------------------------------------------------------------------------- */

void _assert_cb_variant_equal(const CbVariant* expected,
                              const CbVariant* actual,
                              const char * const file,
                              const int line)
{
    /* compare types */
    CbVariantType expected_type = cb_variant_get_type(expected);
    CbVariantType actual_type   = cb_variant_get_type(actual);
    _assert_true(
        cast_to_largest_integral_type(cb_variant_type_is_valid(expected_type)),
        "Expected variant type is not valid",
        file,
        line
    );
    _assert_int_equal(
        cast_to_largest_integral_type(expected_type),
        cast_to_largest_integral_type(actual_type),
        file,
        line
    );
    
    /* compare values */
    switch (expected_type)
    {
        case CB_VARIANT_TYPE_INTEGER:
            _assert_int_equal(
                cast_to_largest_integral_type(cb_integer_get_value(expected)),
                cast_to_largest_integral_type(cb_integer_get_value(actual)),
                file, line
            );
            break;
        
        case CB_VARIANT_TYPE_FLOAT:
        {
            CbFloatDataType value_expected = cb_float_get_value(expected);
            CbFloatDataType value_actual   = cb_float_get_value(actual);
            char msg[128];
            sprintf(msg, "expected %f, but was %f", value_expected, value_actual);
            
            _assert_true(dequal(value_expected, value_actual), msg, file, line);
            break;
        }
        
        case CB_VARIANT_TYPE_BOOLEAN:
            _assert_int_equal(
                cast_to_largest_integral_type(cb_boolean_get_value(expected)),
                cast_to_largest_integral_type(cb_boolean_get_value(actual)),
                file, line
            );
            break;
        
        case CB_VARIANT_TYPE_STRING:
            _assert_string_equal(
                cb_string_get_value(expected),
                cb_string_get_value(actual),
                file,
                line
            );
            break;
        
        case CB_VARIANT_TYPE_UNDEFINED:
            /* CB_VARIANT_TYPE_UNDEFINED has no value to be compared -> equal */
            break;
        
        default:
            _assert_true(0, "Invalid variant type", file, line);
            break;
    }
}

void _assert_cb_integer_equal(const CbIntegerDataType expected,
                              const CbVariant* actual,
                              const char * const file,
                              const int line)
{
    CbVariant* expected_variant = cb_integer_create(expected);
    _assert_cb_variant_equal(expected_variant, actual, file, line);
    cb_variant_destroy(expected_variant);
}

void _assert_cb_float_equal(const CbFloatDataType expected,
                            const CbVariant* actual,
                            const char * const file,
                            const int line)
{
    CbVariant* expected_variant = cb_float_create(expected);
    _assert_cb_variant_equal(expected_variant, actual, file, line);
    cb_variant_destroy(expected_variant);
}

void _assert_cb_boolean_equal(const CbBooleanDataType expected,
                              const CbVariant* actual,
                              const char * const file,
                              const int line)
{
    CbVariant* expected_variant = cb_boolean_create(expected);
    _assert_cb_variant_equal(expected_variant, actual, file, line);
    cb_variant_destroy(expected_variant);
}

void stream_to_string(FILE* stream, char* string, bool trim)
{
    rewind(stream); /* go to beginning of the stream */
    
    while (!feof(stream))
    {
        *string = fgetc(stream);
        
        if (feof(stream))
            *string = '\0';
        else
            string++;
    }
    
    if (trim) /* trim string */
        while (!isprint((int) *string))
        {
            *string = '\0';
            string--;
        }
}
