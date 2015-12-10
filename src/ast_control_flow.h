/*******************************************************************************
 * Abstract syntax tree node: ControlFlow
 * Representation of an control flow statement, such as:
 *    - if
 *    - case
 *    - while
 *    - for
 * 
 * Inherites from CbAstNode
 ******************************************************************************/

#ifndef AST_CONTROL_FLOW_H
#define AST_CONTROL_FLOW_H

#include "ast.h"


/* -------------------------------------------------------------------------- */

typedef struct CbAstControlFlowNode CbAstControlFlowNode;

typedef enum
{
    CB_AST_CONTROL_FLOW_TYPE_IF
} CbAstControlFlowNodeType;


/* -------------------------------------------------------------------------- */

/*
 * Constructor for an if-statement
 */
CbAstControlFlowNode* cb_ast_if_node_create(CbAstNode* condition,
                                            CbAstNode* true_branch,
                                            CbAstNode* false_branch);

/*
 * Destructor for an if-statement
 */
void cb_ast_if_node_destroy(CbAstControlFlowNode* self);

/*
 * Evaluate an if-statement
 */
CbVariant* cb_ast_if_node_eval(const CbAstControlFlowNode* self,
                               const CbSymbolTable* symbols);

/*
 * Check semantics for an if-statement
 */
bool cb_ast_if_node_check_semantic(const CbAstControlFlowNode* self,
                                   CbSymbolTable* symbols);


#endif /* AST_CONTROL_FLOW_H */
