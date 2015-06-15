/*******************************************************************************
 * Tests for CbSymbolTable
 ******************************************************************************/

#include "../src/symbol_table.h"
#include "../src/symbol_variable.h"
#include "test.h"


/* -------------------------------------------------------------------------- */

/*void setup_symbol_table(void** state)
{
    *state = cb_symbol_table_create(16, NULL, free);
}

void teardown_symbol_table(void** state)
{
    cb_symbol_table_destroy(*state);
}*/


/* -------------------------------------------------------------------------- */

void symbol_table_common_test(void** state)
{
    const CbSymbol* symbol;
    CbSymbol* test1   = (CbSymbol*) cb_symbol_variable_create("test_var");
    CbSymbol* test2   = (CbSymbol*) cb_symbol_variable_create("test_var2");
    CbSymbol* test3   = (CbSymbol*) cb_symbol_variable_create("test_var3");
    CbSymbolTable* st = cb_symbol_table_create();
    
    assert_null(cb_symbol_table_insert(st, test1));
    
    symbol = cb_symbol_table_insert(st, test1);
    assert_non_null(symbol);
    assert_true(cb_symbol_is_variable(symbol));
    
    symbol = cb_symbol_table_lookup(st, "invalid");
    assert_null(symbol);
    
    symbol = cb_symbol_table_lookup(st, "test_var");
    assert_non_null(symbol);
    assert_true(cb_symbol_is_variable(symbol));
    
    /* enter nested scope */
    cb_symbol_table_enter_scope(st);
        assert_null(cb_symbol_table_insert(st, test2));
        
        symbol = cb_symbol_table_lookup(st, "test_var2");
        assert_non_null(symbol);
        assert_true(cb_symbol_is_variable(symbol));
        
        symbol = cb_symbol_table_lookup(st, "test_var");
        assert_non_null(symbol);
        assert_true(cb_symbol_is_variable(symbol));
        
        symbol = cb_symbol_table_lookup(st, "invalid");
        assert_null(symbol);
        
        /* switch to a different scope (function call) */
        cb_symbol_table_switch_scope(st, NULL);
            assert_null(cb_symbol_table_insert(st, test3));
            
            symbol = cb_symbol_table_lookup(st, "test_var");
            assert_non_null(symbol);
            assert_true(cb_symbol_is_variable(symbol));
            
            /* test_var2 should not be available in this scope */
            symbol = cb_symbol_table_lookup(st, "test_var2");
            assert_null(symbol);
        cb_symbol_table_leave_scope(st);
    cb_symbol_table_leave_scope(st);
    
    symbol = cb_symbol_table_lookup(st, "test_var2");
    assert_null(symbol);
    
    cb_symbol_table_destroy(st);
}
