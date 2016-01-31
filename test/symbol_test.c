/*******************************************************************************
 * Tests for CbSymbolTable
 ******************************************************************************/

#include "../src/symbol_variable.h"
#include "../src/symbol_function.h"
#include "test.h"


/* -------------------------------------------------------------------------- */

void symbol_variable_test(void** state)
{
    CbSymbol* test1   = (CbSymbol*) cb_symbol_variable_create("test_var1");
    CbSymbol* test2   = (CbSymbol*) cb_symbol_variable_create("test_var2");
    CbSymbol* test3   = (CbSymbol*) cb_symbol_variable_create("test_var3");
    CbVariant* value1 = cb_integer_create(123);
    CbVariant* value2 = cb_float_create(123.000321);
    CbVariant* value3 = cb_variant_create();

    assert_int_equal(CB_VARIANT_TYPE_UNDEFINED, cb_symbol_get_data_type(test1));
    assert_int_equal(CB_VARIANT_TYPE_UNDEFINED, cb_symbol_get_data_type(test2));
    assert_int_equal(CB_VARIANT_TYPE_UNDEFINED, cb_symbol_get_data_type(test3));

    cb_symbol_variable_assign((CbSymbolVariable*) test1, value1);
    cb_symbol_variable_assign((CbSymbolVariable*) test2, value2);
    cb_symbol_variable_assign((CbSymbolVariable*) test3, value3);

    assert_int_equal(CB_VARIANT_TYPE_INTEGER,   cb_symbol_get_data_type(test1));
    assert_int_equal(CB_VARIANT_TYPE_FLOAT,     cb_symbol_get_data_type(test2));
    assert_int_equal(CB_VARIANT_TYPE_UNDEFINED, cb_symbol_get_data_type(test3));

    assert_cb_integer_equal(
        123, cb_symbol_variable_get_value((CbSymbolVariable*) test1)
    );
    assert_cb_float_equal(
        123.000321, cb_symbol_variable_get_value((CbSymbolVariable*) test2)
    );

    /* clean up */
    cb_variant_destroy(value1);
    cb_variant_destroy(value2);
    cb_variant_destroy(value3);
    cb_symbol_destroy(test1);
    cb_symbol_destroy(test2);
    cb_symbol_destroy(test3);
}
