/*******************************************************************************
 * Abstract syntax tree node: Binary
 * Representation of a binary operation (e.g. addition, subtraction, etc.)
 * NOTE: This does not include assignment operations, for these operations the
 *       node type CbAstAssignmentNode is used.
 * 
 * Inherites from CbAstNode
 ******************************************************************************/

#ifndef AST_BINARY_H
#define AST_BINARY_H

#include "variant.h"


/* -------------------------------------------------------------------------- */

typedef struct CbAstBinaryNode CbAstBinaryNode;
typedef enum
{
    CB_BINARY_OPERATOR_TYPE_ADD,
    CB_BINARY_OPERATOR_TYPE_SUB,
    CB_BINARY_OPERATOR_TYPE_MUL,
    CB_BINARY_OPERATOR_TYPE_DIV
} CbBinaryOperatorType;


/* -------------------------------------------------------------------------- */

/*
 * Get string representation of an binary operator type
 */
const char* cb_binary_operator_type_stringify(CbBinaryOperatorType type);

/*
 * Constructor
 */
CbAstBinaryNode* cb_ast_binary_node_create(CbBinaryOperatorType operator_type,
                                           CbAstNode* left,
                                           CbAstNode* right);

/*
 * Destructor
 */
void cb_ast_binary_node_destroy(CbAstBinaryNode* self);

/*
 * Evaluate binary node
 */
CbVariant* cb_ast_binary_node_eval(const CbAstBinaryNode* self,
                                   const CbSymbolTable* symbols);

/*
 * Check semantics
 */
bool cb_ast_binary_node_check_semantic(const CbAstBinaryNode* self,
                                       CbSymbolTable* symbols);


#endif /* AST_BINARY_H */
