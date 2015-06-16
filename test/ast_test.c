/*******************************************************************************
 * Test for abstract syntax tree (AST)
 ******************************************************************************/

#include "../src/error_handling.h"
#include "../src/symbol_table.h"
#include "../src/symbol_variable.h"
#include "../src/symbol_function.h"
#include "../src/ast.h"
#include "../src/ast_value.h"
#include "../src/ast_binary.h"
#include "../src/ast_unary.h"
#include "../src/ast_variable.h"

#include "test.h"


/* -------------------------------------------------------------------------- */

static void test_ast_binary_node_eval(const CbBinaryOperatorType operator,
                                      const CbIntegerDataType value1,
                                      const CbIntegerDataType value2,
                                      CbVariant* expected_result);
static CbAstValueNode* test_create_ast_value_integer_node(const CbIntegerDataType value);
static CbAstValueNode* test_create_ast_value_float_node(const CbFloatDataType value);


/* -------------------------------------------------------------------------- */

/*
 * Test constructors and destructors of all AST node types
 */
void ast_alloc_test(void** state)
{
    CbAstBinaryNode* binary_node = NULL;
    CbAstValueNode* value_node   = NULL;
    CbAstUnaryNode* unary_node   = NULL;
    CbAstVariableNode* var_node  = NULL;
    
    value_node = test_create_ast_value_integer_node(123);
    cb_ast_node_destroy((CbAstNode*) value_node);
    
    value_node = test_create_ast_value_float_node(123.00123);
    cb_ast_node_destroy((CbAstNode*) value_node);
    
    binary_node = cb_ast_binary_node_create(
        CB_BINARY_OPERATOR_TYPE_ADD,
        (CbAstNode*) test_create_ast_value_integer_node(123),
        (CbAstNode*) test_create_ast_value_integer_node(77)
    );
    cb_ast_node_destroy((CbAstNode*) binary_node);
    
    unary_node = cb_ast_unary_node_create(
        CB_UNARY_OPERATOR_TYPE_MINUS,
        (CbAstNode*) test_create_ast_value_integer_node(123)
    );
    cb_ast_node_destroy((CbAstNode*) unary_node);
    
    var_node = cb_ast_variable_node_create("test_var");
    cb_ast_node_destroy((CbAstNode*) var_node);
}

/*
 * Test evaluation of all AST node types
 */
void ast_eval_test(void** state)
{
    CbAstUnaryNode* unary_node = NULL;
    CbVariant* result          = NULL;
    
    /* eval binary AST nodes (addition) */
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_ADD, 123, 77,  cb_integer_create(200));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_ADD, 123, -77, cb_integer_create(46));
    /* eval binary AST nodes (subtraction) */
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_SUB, 123, 23,  cb_integer_create(100));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_SUB, 23,  -77, cb_integer_create(100));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_SUB, 23,  77,  cb_integer_create(-54));
    /* eval binary AST nodes (multiplication) */
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_MUL, 123, 8,   cb_integer_create(984));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_MUL, 123, -8,  cb_integer_create(-984));
    /* eval binary AST nodes (division) */
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, 984, 8,   cb_float_create(123));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, 984, -8,  cb_float_create(-123));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, 984, 5,   cb_float_create(196.8));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, 984, -5,  cb_float_create(-196.8));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, 10,  8,   cb_float_create(1.25));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, 10,  -8,  cb_float_create(-1.25));
    
    /* unary AST node (expression: - <integer>) */
    unary_node = cb_ast_unary_node_create(
        CB_UNARY_OPERATOR_TYPE_MINUS,
        (CbAstNode*) test_create_ast_value_integer_node(123)
    );
    result = cb_ast_node_eval((CbAstNode*) unary_node, NULL);
    assert_cb_integer_equal(-123, result);
    cb_variant_destroy(result);
    cb_ast_node_destroy((CbAstNode*) unary_node);
    /* unary AST node (expression: - <float>) */
    unary_node = cb_ast_unary_node_create(
        CB_UNARY_OPERATOR_TYPE_MINUS,
        (CbAstNode*) test_create_ast_value_float_node(321.00123)
    );
    result = cb_ast_node_eval((CbAstNode*) unary_node, NULL);
    assert_cb_float_equal(-321.00123, result);
    cb_variant_destroy(result);
    cb_ast_node_destroy((CbAstNode*) unary_node);
    
    /*
     * variable AST node (expression: <variable>)
     * TODO: This test will fail at the moment due to the not yet implemented
     *       CbAstVariableNode structure.
     */
    {
        CbAstNode* node            = NULL;
        CbSymbolTable* symbols     = NULL;
        CbSymbolVariable* variable = NULL;
        CbVariant* temp            = NULL;
        
        /* prepare symbol and symbol table */
        symbols  = cb_symbol_table_create();
        variable = cb_symbol_variable_create("test_var");
        temp     = cb_integer_create(123);
        cb_symbol_variable_assign(variable, temp);
        cb_variant_destroy(temp);
        assert_null(cb_symbol_table_insert(symbols, (CbSymbol*) variable));
        /* prepare AST node */
        node = (CbAstNode*) cb_ast_variable_node_create("test_var");
        
        result = cb_ast_node_eval(node, symbols);
        assert_non_null(result);
        assert_cb_integer_equal(123, result);
        
        /* clean up */
        cb_variant_destroy(result);
        cb_ast_node_destroy(node);
        cb_symbol_table_destroy(symbols);
    }
}

/*
 * Test semantic analysis of all AST node types
 */
void ast_check_semantic_test(void** state)
{
    CbAstNode* node;
    CbSymbolTable* symbols;
    
    /* expression: <numeric> + ( - <numeric> ) */
    node = (CbAstNode*) cb_ast_binary_node_create(
        CB_BINARY_OPERATOR_TYPE_ADD,
        (CbAstNode*) test_create_ast_value_integer_node(123),
        (CbAstNode*) cb_ast_unary_node_create(
            CB_UNARY_OPERATOR_TYPE_MINUS,
            (CbAstNode*) test_create_ast_value_integer_node(321)
        )
    );
    assert_true(cb_ast_node_check_semantic(node, NULL));
    cb_ast_node_destroy(node);
    
    /* expression: <numeric> + <float> */
    node = (CbAstNode*) cb_ast_binary_node_create(
        CB_BINARY_OPERATOR_TYPE_ADD,
        (CbAstNode*) test_create_ast_value_integer_node(123),
        (CbAstNode*) test_create_ast_value_float_node(321.00123)
    );
    assert_true(cb_ast_node_check_semantic(node, NULL));
    cb_ast_node_destroy(node);
    
    /* expression: - <float> */
    node = (CbAstNode*) cb_ast_unary_node_create(
        CB_UNARY_OPERATOR_TYPE_MINUS,
        (CbAstNode*) test_create_ast_value_float_node(321.00123)
    );
    assert_true(cb_ast_node_check_semantic(node, NULL));
    cb_ast_node_destroy(node);
    
    /* variable "test_var" */
    symbols = cb_symbol_table_create();
    assert_null(cb_symbol_table_insert(
        symbols, (CbSymbol*) cb_symbol_variable_create("test_var")
    ));
    node = (CbAstNode*) cb_ast_variable_node_create("test_var");
    assert_true(cb_ast_node_check_semantic(node, symbols));
    cb_ast_node_destroy(node);
    cb_symbol_table_destroy(symbols);
}

/*
 * Test errors of semantic analysis for all AST node types
 */
void ast_check_semantic_error_test(void** state)
{
    CbAstNode* node;
    CbSymbolTable* symbols;
    char stream_content[128];
    
    /*
     * Test: Expected identifier is declared as a function
     */
    symbols = cb_symbol_table_create();
    assert_null(cb_symbol_table_insert(
        symbols, (CbSymbol*) cb_symbol_function_create("test_var")
    ));
    node = (CbAstNode*) cb_ast_variable_node_create("test_var");
    cb_ast_node_set_line(node, 1);
    assert_false(cb_ast_node_check_semantic(node, symbols));
    cb_ast_node_destroy(node);
    assert_true(cb_error_occurred());
    cb_error_process();
    assert_false(cb_error_occurred());
    stream_to_string(*state, stream_content, true);
    assert_string_equal("semantic error: line 1: identifier 'test_var' is "\
                        "declared as function, not as variable",
                        stream_content);
    cb_symbol_table_destroy(symbols);
    
    /* discard stream content */
    resetup_error_handling(state);
    
    /*
     * Test: Expected identifier is not declared at all
     */
    symbols = cb_symbol_table_create();
    node = (CbAstNode*) cb_ast_variable_node_create("test_var");
    cb_ast_node_set_line(node, 1);
    assert_false(cb_ast_node_check_semantic(node, symbols));
    cb_ast_node_destroy(node);
    assert_true(cb_error_occurred());
    cb_error_process();
    assert_false(cb_error_occurred());
    stream_to_string(*state, stream_content, true);
    assert_string_equal("semantic error: line 1: variable 'test_var' is "\
                        "not declared in an available scope", stream_content);
    cb_symbol_table_destroy(symbols);
}


/* -------------------------------------------------------------------------- */

/*
 * Test evaluation of binary AST nodes (internal)
 */
static void test_ast_binary_node_eval(const CbBinaryOperatorType operator,
                                      const CbIntegerDataType value1,
                                      const CbIntegerDataType value2,
                                      CbVariant* expected_result)
{
    CbAstBinaryNode* node = cb_ast_binary_node_create(
        operator,
        (CbAstNode*) test_create_ast_value_integer_node(value1),
        (CbAstNode*) test_create_ast_value_integer_node(value2)
    );
    
    CbVariant* result = cb_ast_node_eval((CbAstNode*) node, NULL);
    assert_cb_variant_equal(expected_result, result);
    cb_variant_destroy(result);
    cb_variant_destroy(expected_result);
    
    cb_ast_node_destroy((CbAstNode*) node);
}

/*
 * Create an integer value node (internal)
 */
static CbAstValueNode* test_create_ast_value_integer_node(const CbIntegerDataType value)
{
    CbVariant* variant         = cb_integer_create(value);
    CbAstValueNode* value_node = cb_ast_value_node_create(variant);
    cb_variant_destroy(variant);
    
    return value_node;
}

/*
 * Create a float value node (internal)
 */
static CbAstValueNode* test_create_ast_value_float_node(const CbFloatDataType value)
{
    CbVariant* variant         = cb_float_create(value);
    CbAstValueNode* value_node = cb_ast_value_node_create(variant);
    cb_variant_destroy(variant);
    
    return value_node;
}
