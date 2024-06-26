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
#include "../src/ast_declaration.h"
#include "../src/ast_control_flow.h"

#include "test.h"


/* -------------------------------------------------------------------------- */

static void test_ast_binary_node_eval(const CbBinaryOperatorType operator,
                                      CbVariant* value1,
                                      CbVariant* value2,
                                      CbVariant* expected_result);
static CbAstValueNode* test_create_ast_value_integer_node(const CbIntegerDataType value);
static CbAstValueNode* test_create_ast_value_float_node(const CbFloatDataType value);
static CbAstValueNode* test_create_ast_value_boolean_node(const CbBooleanDataType value);


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
        CB_BINARY_OPERATOR_TYPE_ADD, cb_integer_create(123),   cb_integer_create(77),    cb_integer_create(200));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_ADD, cb_integer_create(123),   cb_integer_create(-77),   cb_integer_create(46));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_ADD, cb_float_create(123.123), cb_float_create(77.321),  cb_float_create(200.444));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_ADD, cb_float_create(123.123), cb_float_create(-77.321), cb_float_create(45.802));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_ADD, cb_integer_create(123),   cb_float_create(-77.321), cb_float_create(45.679));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_ADD, cb_float_create(123.123), cb_integer_create(77),    cb_float_create(200.123));
    /* eval binary AST nodes (string concatenation) */
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_ADD, cb_string_create("a"),    cb_string_create("b"),    cb_string_create("ab"));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_ADD, cb_string_create("foo"),  cb_string_create("bar"),  cb_string_create("foobar"));
    /* eval binary AST nodes (subtraction) */
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_SUB, cb_integer_create(123),   cb_integer_create(23),    cb_integer_create(100));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_SUB, cb_integer_create(23),    cb_integer_create(-77),   cb_integer_create(100));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_SUB, cb_integer_create(23),    cb_integer_create(77),    cb_integer_create(-54));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_SUB, cb_float_create(123.123), cb_float_create(77.321),  cb_float_create(45.802));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_SUB, cb_float_create(123.123), cb_float_create(-77.321), cb_float_create(200.444));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_SUB, cb_integer_create(123),   cb_float_create(-77.321), cb_float_create(200.321));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_SUB, cb_float_create(123.123), cb_integer_create(77),    cb_float_create(46.123));
    /* eval binary AST nodes (multiplication) */
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_MUL, cb_integer_create(123),   cb_integer_create(8),     cb_integer_create(984));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_MUL, cb_integer_create(123),   cb_integer_create(-8),    cb_integer_create(-984));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_MUL, cb_float_create(123.123), cb_float_create(77.321),  cb_float_create(9519.993483));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_MUL, cb_float_create(123.123), cb_float_create(-77.321), cb_float_create(-9519.993483));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_MUL, cb_integer_create(123),   cb_float_create(-77.321), cb_float_create(-9510.483));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_MUL, cb_float_create(123.123), cb_integer_create(77),    cb_float_create(9480.471));
    /* eval binary AST nodes (division) */
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, cb_integer_create(984),   cb_integer_create(8),     cb_integer_create(123));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, cb_integer_create(984),   cb_integer_create(-8),    cb_integer_create(-123));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, cb_integer_create(984),   cb_integer_create(5),     cb_float_create(196.8));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, cb_integer_create(984),   cb_integer_create(-5),    cb_float_create(-196.8));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, cb_integer_create(10),    cb_integer_create(8),     cb_float_create(1.25));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, cb_integer_create(10),    cb_integer_create(-8),    cb_float_create(-1.25));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, cb_float_create(5.5),     cb_float_create(0.5),     cb_float_create(11.0));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, cb_float_create(123.25),  cb_float_create(2.125),   cb_float_create(58));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, cb_float_create(123.25),  cb_float_create(-2.125),  cb_float_create(-58));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, cb_float_create(-195.6561), cb_integer_create(123),  cb_float_create(-1.5907));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_DIV, cb_float_create(123.123), cb_integer_create(77),    cb_float_create(1.599));
    /* eval binary AST nodes (logical AND) */
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_LOGICAL_AND, cb_boolean_create(true),  cb_boolean_create(true),  cb_boolean_create(true));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_LOGICAL_AND, cb_boolean_create(true),  cb_boolean_create(false), cb_boolean_create(false));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_LOGICAL_AND, cb_boolean_create(false), cb_boolean_create(false), cb_boolean_create(false));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_LOGICAL_AND, cb_boolean_create(false), cb_boolean_create(true),  cb_boolean_create(false));
    /* eval binary AST nodes (logical OR) */
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_LOGICAL_OR, cb_boolean_create(true),  cb_boolean_create(true),  cb_boolean_create(true));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_LOGICAL_OR, cb_boolean_create(true),  cb_boolean_create(false), cb_boolean_create(true));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_LOGICAL_OR, cb_boolean_create(false), cb_boolean_create(false), cb_boolean_create(false));
    test_ast_binary_node_eval(
        CB_BINARY_OPERATOR_TYPE_LOGICAL_OR, cb_boolean_create(false), cb_boolean_create(true),  cb_boolean_create(true));
    /* eval binary AST nodes (comparison operator >) */
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GT, cb_integer_create(984),     cb_integer_create(8),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GT, cb_integer_create(-1),      cb_integer_create(0),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GT, cb_integer_create(0),       cb_integer_create(-1),      cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GT, cb_integer_create(-3),      cb_integer_create(-8),      cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GT, cb_integer_create(1),       cb_integer_create(1),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GT, cb_float_create(8.5),       cb_float_create(8.3),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GT, cb_float_create(-1.993483), cb_float_create(-1.993484), cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GT, cb_float_create(1.993483),  cb_float_create(1.993484),  cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GT, cb_integer_create(2),       cb_float_create(1.999999),  cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GT, cb_float_create(0.000001),  cb_float_create(0),         cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GT, cb_float_create(-8),        cb_float_create(-8.000001), cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GT, cb_float_create(1.999999),  cb_float_create(1.999999),  cb_boolean_create(false));
    /* eval binary AST nodes (comparison operator >=) */
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GE, cb_integer_create(984),     cb_integer_create(8),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GE, cb_integer_create(-1),      cb_integer_create(0),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GE, cb_integer_create(0),       cb_integer_create(-1),      cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GE, cb_integer_create(-3),      cb_integer_create(-8),      cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GE, cb_integer_create(1),       cb_integer_create(1),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GE, cb_float_create(8.5),       cb_float_create(8.3),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GE, cb_float_create(-1.993483), cb_float_create(-1.993484), cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GE, cb_float_create(1.993483),  cb_float_create(1.993484),  cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GE, cb_integer_create(2),       cb_float_create(1.999999),  cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GE, cb_float_create(0.000001),  cb_float_create(0),         cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GE, cb_float_create(-8),        cb_float_create(-8.000001), cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_GE, cb_float_create(1.999999),  cb_float_create(1.999999),  cb_boolean_create(true));
    /* eval binary AST nodes (comparison operator <) */
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LT, cb_integer_create(984),     cb_integer_create(8),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LT, cb_integer_create(-1),      cb_integer_create(0),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LT, cb_integer_create(0),       cb_integer_create(-1),      cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LT, cb_integer_create(-3),      cb_integer_create(-8),      cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LT, cb_integer_create(1),       cb_integer_create(1),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LT, cb_float_create(8.5),       cb_float_create(8.3),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LT, cb_float_create(-1.993483), cb_float_create(-1.993484), cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LT, cb_float_create(1.993483),  cb_float_create(1.993484),  cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LT, cb_integer_create(2),       cb_float_create(1.999999),  cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LT, cb_float_create(0.000001),  cb_float_create(0),         cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LT, cb_float_create(-8),        cb_float_create(-8.000001), cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LT, cb_float_create(1.999999),  cb_float_create(1.999999),  cb_boolean_create(false));
    /* eval binary AST nodes (comparison operator <=) */
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LE, cb_integer_create(984),     cb_integer_create(8),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LE, cb_integer_create(-1),      cb_integer_create(0),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LE, cb_integer_create(0),       cb_integer_create(-1),      cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LE, cb_integer_create(-3),      cb_integer_create(-8),      cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LE, cb_integer_create(1),       cb_integer_create(1),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LE, cb_float_create(8.5),       cb_float_create(8.3),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LE, cb_float_create(-1.993483), cb_float_create(-1.993484), cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LE, cb_float_create(1.993483),  cb_float_create(1.993484),  cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LE, cb_integer_create(2),       cb_float_create(1.999999),  cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LE, cb_float_create(0.000001),  cb_float_create(0),         cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LE, cb_float_create(-8),        cb_float_create(-8.000001), cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_LE, cb_float_create(1.999999),  cb_float_create(1.999999),  cb_boolean_create(true));
    /* eval binary AST nodes (comparison operator =) */
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_integer_create(984),     cb_integer_create(8),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_integer_create(-1),      cb_integer_create(0),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_integer_create(0),       cb_integer_create(0),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_integer_create(-8),      cb_integer_create(-8),      cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_integer_create(1),       cb_integer_create(1),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_float_create(8.5),       cb_float_create(8.3),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_float_create(-1.993483), cb_float_create(-1.993484), cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_integer_create(2),       cb_float_create(1.999999),  cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_float_create(0.000001),  cb_float_create(0),         cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_float_create(0.000001),  cb_float_create(0.000001),  cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_integer_create(1),       cb_float_create(1),         cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_string_create("abc"),    cb_string_create("foobar"), cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_string_create("abc"),    cb_string_create("abc"),    cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_string_create("foo"),    cb_string_create("foobar"), cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_boolean_create(true),    cb_boolean_create(true),    cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_boolean_create(false),   cb_boolean_create(false),   cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, cb_boolean_create(true),    cb_boolean_create(false),   cb_boolean_create(false));
    /* eval binary AST nodes (comparison operator ==) */
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_integer_create(984),     cb_integer_create(8),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_integer_create(-1),      cb_integer_create(0),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_integer_create(0),       cb_integer_create(0),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_integer_create(-8),      cb_integer_create(-8),      cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_integer_create(1),       cb_integer_create(1),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_float_create(8.5),       cb_float_create(8.3),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_float_create(-1.993483), cb_float_create(-1.993484), cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_integer_create(2),       cb_float_create(1.999999),  cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_float_create(0.000001),  cb_float_create(0),         cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_float_create(0.000001),  cb_float_create(0.000001),  cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_integer_create(1),       cb_float_create(1),         cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_string_create("abc"),    cb_string_create("foobar"), cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_string_create("abc"),    cb_string_create("abc"),    cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_string_create("foo"),    cb_string_create("foobar"), cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_boolean_create(true),    cb_boolean_create(true),    cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_boolean_create(false),   cb_boolean_create(false),   cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, cb_boolean_create(true),    cb_boolean_create(false),   cb_boolean_create(false));
    /* eval binary AST nodes (comparison operator <>) */
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_integer_create(984),     cb_integer_create(8),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_integer_create(-1),      cb_integer_create(0),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_integer_create(0),       cb_integer_create(0),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_integer_create(-8),      cb_integer_create(-8),      cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_integer_create(1),       cb_integer_create(1),       cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_float_create(8.5),       cb_float_create(8.3),       cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_float_create(-1.993483), cb_float_create(-1.993484), cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_integer_create(2),       cb_float_create(1.999999),  cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_float_create(0.000001),  cb_float_create(0),         cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_float_create(0.000001),  cb_float_create(0.000001),  cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_integer_create(1),       cb_float_create(1),         cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_string_create("abc"),    cb_string_create("foobar"), cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_string_create("abc"),    cb_string_create("abc"),    cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_string_create("foo"),    cb_string_create("foobar"), cb_boolean_create(true));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_boolean_create(true),    cb_boolean_create(true),    cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_boolean_create(false),   cb_boolean_create(false),   cb_boolean_create(false));
    test_ast_binary_node_eval(CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, cb_boolean_create(true),    cb_boolean_create(false),   cb_boolean_create(true));
    
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
    /* unary AST node (expression: not <boolean>) */
    unary_node = cb_ast_unary_node_create(
        CB_UNARY_OPERATOR_TYPE_LOGICAL_NOT,
        (CbAstNode*) test_create_ast_value_boolean_node(true)
    );
    result = cb_ast_node_eval((CbAstNode*) unary_node, NULL);
    assert_cb_boolean_equal(false, result);
    cb_variant_destroy(result);
    cb_ast_node_destroy((CbAstNode*) unary_node);
    /* unary AST node (expression: not <boolean>) */
    unary_node = cb_ast_unary_node_create(
        CB_UNARY_OPERATOR_TYPE_LOGICAL_NOT,
        (CbAstNode*) test_create_ast_value_boolean_node(false)
    );
    result = cb_ast_node_eval((CbAstNode*) unary_node, NULL);
    assert_cb_boolean_equal(true, result);
    cb_variant_destroy(result);
    cb_ast_node_destroy((CbAstNode*) unary_node);
    
    /*
     * variable AST node (expression: <variable>)
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
    
    /*
     * declaration AST node (expression: |<variable>|)
     */
    {
        CbVariant* result = NULL;
        CbVariant* temp   = cb_variant_create();
        CbAstNode* node   = (CbAstNode*) cb_ast_declaration_node_create(
            CB_AST_DECLARATION_TYPE_VARIABLE, "test_var"
        );
        
        result = cb_ast_node_eval(node, NULL);
        assert_cb_variant_equal(temp, result);
        
        cb_variant_destroy(result);
        cb_variant_destroy(temp);
        cb_ast_node_destroy(node);
    }
    
    /*
     * Control flow AST nodes
     */
    {
        CbAstNode* node;
        CbVariant* result;
        CbVariant* condition   = cb_boolean_create(false);
        CbVariant* value_true  = cb_integer_create(123);
        CbVariant* value_false = cb_integer_create(321);
        
        /* Case 1: Condition is false */
        node = (CbAstNode*) cb_ast_if_node_create(
            (CbAstNode*) cb_ast_value_node_create(condition),
            (CbAstNode*) cb_ast_value_node_create(value_true),
            (CbAstNode*) cb_ast_value_node_create(value_false)
        );
        
        result = cb_ast_node_eval(node, NULL);
        assert_cb_variant_equal(value_false, result);
        
        cb_variant_destroy(result);
        cb_variant_destroy(condition);
        cb_ast_node_destroy(node);
        condition = cb_boolean_create(true);
        
        /* Case 2: Condition is true */
        node = (CbAstNode*) cb_ast_if_node_create(
            (CbAstNode*) cb_ast_value_node_create(condition),
            (CbAstNode*) cb_ast_value_node_create(value_true),
            (CbAstNode*) cb_ast_value_node_create(value_false)
        );
        
        result = cb_ast_node_eval(node, NULL);
        assert_cb_variant_equal(value_true, result);
        
        cb_variant_destroy(result);
        cb_variant_destroy(condition);
        cb_ast_node_destroy(node);
        condition = cb_boolean_create(false);
        
        /* Case 3: Condition is false, no false-branch */
        node = (CbAstNode*) cb_ast_if_node_create(
            (CbAstNode*) cb_ast_value_node_create(condition),
            (CbAstNode*) cb_ast_value_node_create(value_true),
            NULL
        );
        
        result = cb_ast_node_eval(node, NULL);
        assert_true(cb_variant_is_undefined(result));
        
        cb_variant_destroy(result);
        cb_variant_destroy(condition);
        cb_ast_node_destroy(node);
        
        cb_variant_destroy(value_true);
        cb_variant_destroy(value_false);
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
    
    /* declaring variable "test_var" */
    symbols = cb_symbol_table_create();
    node    = (CbAstNode*) cb_ast_declaration_node_create(
        CB_AST_DECLARATION_TYPE_VARIABLE, "test_var"
    );
    assert_true(cb_ast_node_check_semantic(node, symbols));
    cb_ast_node_destroy(node);
    /* additional test for accessing variable in symbol table */
    node = (CbAstNode*) cb_ast_variable_node_create("test_var");
    assert_true(cb_ast_node_check_semantic(node, symbols));
    cb_ast_node_destroy(node);
    cb_symbol_table_destroy(symbols);
    
    /* statement: if */
    {
        CbVariant* dummy1 = cb_boolean_create(true);
        CbVariant* dummy2 = cb_integer_create(123);
        CbVariant* dummy3 = cb_integer_create(321);
        
        node = (CbAstNode*) cb_ast_if_node_create(
            (CbAstNode*) cb_ast_value_node_create(dummy1),
            (CbAstNode*) cb_ast_value_node_create(dummy2),
            (CbAstNode*) cb_ast_value_node_create(dummy3)
        );
        
        assert_true(cb_ast_node_check_semantic(node, NULL));
        cb_ast_node_destroy(node);
        
        node = (CbAstNode*) cb_ast_if_node_create(
            (CbAstNode*) cb_ast_value_node_create(dummy1),
            (CbAstNode*) cb_ast_value_node_create(dummy2),
            NULL
        );
        
        assert_true(cb_ast_node_check_semantic(node, NULL));
        cb_ast_node_destroy(node);
        
        node = (CbAstNode*) cb_ast_if_node_create(
            (CbAstNode*) cb_ast_value_node_create(dummy1),
            NULL,
            NULL
        );
        
        assert_true(cb_ast_node_check_semantic(node, NULL));
        cb_ast_node_destroy(node);
        
        cb_variant_destroy(dummy1);
        cb_variant_destroy(dummy2);
        cb_variant_destroy(dummy3);
    }
}

/*
 * Test errors of semantic analysis for all AST node types
 */
void ast_check_semantic_error_test(void** state)
{
    CbAstNode* node;
    CbSymbolTable* symbols;
    CbVariant* v1;
    CbVariant* v2;
    char stream_content[128];
    
    /*
     * Test: Invalid binary operation
     */
    symbols = NULL;
    v1      = cb_integer_create(100);
    v2      = cb_boolean_create(false);
    node    = (CbAstNode*) cb_ast_binary_node_create(
        CB_BINARY_OPERATOR_TYPE_DIV,
        (CbAstNode*) cb_ast_value_node_create(v1),
        (CbAstNode*) cb_ast_value_node_create(v2)
    );
    cb_variant_destroy(v1);
    cb_variant_destroy(v2);
    cb_ast_node_set_line(node, 1);
    assert_false(cb_ast_node_check_semantic(node, symbols));
    cb_ast_node_destroy(node);
    assert_true(cb_error_occurred());
    cb_error_process();
    assert_false(cb_error_occurred());
    stream_to_string(*state, stream_content, true);
    assert_string_equal("semantic error: line 1: Invalid binary operation: "\
                        "<integer> / <boolean>", stream_content);
    
    /* discard stream content */
    resetup_error_handling(state);
    
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
    
    /* discard stream content */
    resetup_error_handling(state);
    
    /*
     * Test: Expected identifier is already declared
     */
    symbols = cb_symbol_table_create();
    assert_null(cb_symbol_table_insert(
        symbols, (CbSymbol*) cb_symbol_variable_create("test_var")
    ));
    node = (CbAstNode*) cb_ast_declaration_node_create(
        CB_AST_DECLARATION_TYPE_VARIABLE, "test_var"
    );
    cb_ast_node_set_line(node, 1);
    assert_false(cb_ast_node_check_semantic(node, symbols));
    cb_ast_node_destroy(node);
    assert_true(cb_error_occurred());
    cb_error_process();
    assert_false(cb_error_occurred());
    stream_to_string(*state, stream_content, true);
    assert_string_equal("semantic error: line 1: symbol 'test_var' already "\
                        "declared as variable in the current scope",
                        stream_content);
    cb_symbol_table_destroy(symbols);
    
    /* discard stream content */
    resetup_error_handling(state);
    
    /*
     * Test: Condition of if-statement is not boolean
     */
    {
        CbVariant* dummy = cb_integer_create(123);
        node             = (CbAstNode*) cb_ast_if_node_create(
            (CbAstNode*) cb_ast_value_node_create(dummy),
            (CbAstNode*) cb_ast_value_node_create(dummy),
            (CbAstNode*) cb_ast_value_node_create(dummy)
        );
        cb_ast_node_set_line(node, 1);
        assert_false(cb_ast_node_check_semantic(node, NULL));
        cb_ast_node_destroy(node);
        assert_true(cb_error_occurred());
        cb_error_process();
        assert_false(cb_error_occurred());
        stream_to_string(*state, stream_content, true);
        assert_string_equal("semantic error: line 1: Condition is not a "\
                            "boolean expression", stream_content);
        cb_variant_destroy(dummy);
    }
}

/*
 * Test evaluation error handling
 */
void ast_eval_error_test(void** state)
{
    CbAstNode* node;
    CbVariant* left;
    CbVariant* right;
    char buffer[1024];
    
    /*
     * Test: Invalid binary operation
     */
    left    = cb_integer_create(100);
    right   = cb_boolean_create(false);
    node    = (CbAstNode*) cb_ast_binary_node_create(
        CB_BINARY_OPERATOR_TYPE_DIV,
        (CbAstNode*) cb_ast_value_node_create(left),
        (CbAstNode*) cb_ast_value_node_create(right)
    );
    cb_variant_destroy(left);
    cb_variant_destroy(right);
    cb_ast_node_set_line(node, 1);
    assert_false(cb_ast_node_eval(node, NULL));
    cb_ast_node_destroy(node);
    assert_true(cb_error_occurred());
    cb_error_process();
    assert_false(cb_error_occurred());
    stream_to_string(*state, buffer, true);
    assert_string_equal("runtime error: line 1: Invalid binary operation: "\
                        "<integer> / <boolean>", buffer);
    
    /* discard stream content */
    resetup_error_handling(state);
    
    /*
     * Test: Division by zero
     */
    left   = cb_integer_create(3);
    right  = cb_integer_create(0);
    node   = (CbAstNode*) cb_ast_binary_node_create(
        CB_BINARY_OPERATOR_TYPE_DIV,
        (CbAstNode*) cb_ast_value_node_create(left),
        (CbAstNode*) cb_ast_value_node_create(right)
    );
    cb_ast_node_set_line(node, 1);
    
    cb_variant_destroy(left);
    cb_variant_destroy(right);
    
    assert_null(cb_ast_node_eval(node, NULL));
    assert_true(cb_error_occurred());
    cb_error_process();
    stream_to_string(*state, buffer, true);
    assert_string_equal("runtime error: line 1: Division by zero is not allowed",
                        buffer);
    
    cb_ast_node_destroy(node);
}


/* -------------------------------------------------------------------------- */

/*
 * Test evaluation of binary AST nodes (internal)
 */
static void test_ast_binary_node_eval(const CbBinaryOperatorType operator,
                                      CbVariant* value1,
                                      CbVariant* value2,
                                      CbVariant* expected_result)
{
    CbAstBinaryNode* node = cb_ast_binary_node_create(
        operator,
        (CbAstNode*) cb_ast_value_node_create(value1),
        (CbAstNode*) cb_ast_value_node_create(value2)
    );
    CbVariant* result = cb_ast_node_eval((CbAstNode*) node, NULL);
    
    assert_cb_variant_equal(expected_result, result);
    
    cb_variant_destroy(result);
    cb_ast_node_destroy((CbAstNode*) node);
    
    cb_variant_destroy(value1);
    cb_variant_destroy(value2);
    cb_variant_destroy(expected_result);
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

/*
 * Create a boolean value node (internal)
 */
static CbAstValueNode* test_create_ast_value_boolean_node(const CbBooleanDataType value)
{
    CbVariant* variant         = cb_boolean_create(value);
    CbAstValueNode* value_node = cb_ast_value_node_create(variant);
    cb_variant_destroy(variant);
    
    return value_node;
}
