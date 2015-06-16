/*******************************************************************************
 * Abstract syntax tree node: Unary
 * Representation of an unary operation (such as unary minus)
 * 
 * Inherites from CbAstNode
 ******************************************************************************/

#ifndef AST_UNARY_H
#define AST_UNARY_H

#include "variant.h"


/* -------------------------------------------------------------------------- */

typedef struct CbAstUnaryNode CbAstUnaryNode;
typedef enum
{
    CB_UNARY_OPERATOR_TYPE_MINUS
} CbUnaryOperatorType;


/* -------------------------------------------------------------------------- */

/*
 * Get string representation of an unary operator type
 */
const char* cb_unary_operator_type_stringify(CbUnaryOperatorType type);

/*
 * Constructor
 */
CbAstUnaryNode* cb_ast_unary_node_create(const CbUnaryOperatorType operator_type,
                                         CbAstNode* operand);

/*
 * Destructor
 */
void cb_ast_unary_node_destroy(CbAstUnaryNode* self);

/*
 * Evaluate unary node
 */
CbVariant* cb_ast_unary_node_eval(const CbAstUnaryNode* self,
                                  const CbSymbolTable* symbols);

/*
 * Check semantics
 */
bool cb_ast_unary_node_check_semantic(const CbAstUnaryNode* self,
                                      CbSymbolTable* symbols);


#endif /* AST_UNARY_H */
