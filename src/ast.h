/*******************************************************************************
 * Abstract syntax tree (AST)
 * The AST is composed of several AST node types (e.g. CbAstValueNode, ...).
 *
 * NOTE: CbAstNode is used as an interface and should only be instantiated by
 *       derived AST node types, i.e. it's an abstract class so to speak.
 *
 * Sample class hierarchy:
 * .-----------------------.
 * |       CbAstNode       |
 * |       »abstract«      |
 * |-----------------------|           .--------------------.
 * | - type: CbAstType     |<----------|   CbAstValueNode   |
 * | - ...                 |           |--------------------|
 * | - ...                 |<----      | - value: int       |
 * '-----------------------'    |      '--------------------'
 *                              |
 *                      .---------------.
 *                      |      ...      |
 *
 ******************************************************************************/

#ifndef AST_H
#define AST_H

#include "variant.h"
#include "symbol_table.h"


/* -------------------------------------------------------------------------- */

typedef struct CbAstNode CbAstNode;
typedef enum CbAstType
{
    CB_AST_TYPE_NONE,
    CB_AST_TYPE_DEBUG_PRINT,
    CB_AST_TYPE_VALUE,
    CB_AST_TYPE_BINARY,
    CB_AST_TYPE_UNARY,
    CB_AST_TYPE_VARIABLE,
    CB_AST_TYPE_ASSIGNMENT,
    CB_AST_TYPE_DECLARATION,
    CB_AST_TYPE_DECLARATION_BLOCK,
    CB_AST_TYPE_STATEMENT_LIST,
    CB_AST_TYPE_CONTROL_FLOW,
    CB_AST_TYPE_COMPARISON
} CbAstType;


/* -------------------------------------------------------------------------- */

/*
 * Destructor
 */
void cb_ast_node_destroy(CbAstNode* self);

/*
 * Line number (Setter)
 */
void cb_ast_node_set_line(CbAstNode* self, int line);

/*
 * Node type (Getter)
 */
CbAstType cb_ast_node_get_type(const CbAstNode* self);

/*
 * Evaluate AST node
 */
CbVariant* cb_ast_node_eval(CbAstNode* self, const CbSymbolTable* symbols);

/*
 * Make sure the node is valid (i.e. not NULL) and evaluate it and return its
 * result.
 * Otherwise return an empty (undefined) variant object.
 */
CbVariant* cb_ast_node_safe_eval(CbAstNode* self, const CbSymbolTable* symbols);

/*
 * Check AST node semantics
 */
bool cb_ast_node_check_semantic(CbAstNode* self, CbSymbolTable* symbols);

/*
 * Make sure the node is valid (i.e. not NULL) and call
 * cb_ast_node_check_semantic().
 * Otherwise return true.
 */
bool cb_ast_node_safe_check_semantic(CbAstNode* self, CbSymbolTable* symbols);

/*
 * Get the variant type of an expression
 *
 * NOTE: This function recursively walks the AST until it finds a value node
 *       (CB_AST_TYPE_VALUE). At this point it is possible to determine the
 *       variant type.
 */
CbVariantType cb_ast_node_get_expression_type(const CbAstNode* self);


#endif /* AST_H */
