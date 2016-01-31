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


/* -------------------------------------------------------------------------- */

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

/*
 * Get the variant type of a binary operation
 */
CbVariantType cb_ast_binary_node_get_expression_type(const CbAstBinaryNode* self);


#endif /* AST_BINARY_H */
