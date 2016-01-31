/*******************************************************************************
 * Abstract syntax tree node: Assignment
 * Representation of an assignment operation.
 *
 * Inherites from CbAstNode
 ******************************************************************************/

#ifndef AST_ASSIGNMENT_H
#define AST_ASSIGNMENT_H

#include "ast.h"


/* -------------------------------------------------------------------------- */

typedef struct CbAstAssignmentNode CbAstAssignmentNode;


/* -------------------------------------------------------------------------- */

/*
 * Constructor
 */
CbAstAssignmentNode* cb_ast_assignment_node_create(CbAstNode* left,
                                                   CbAstNode* right);

/*
 * Destructor
 */
void cb_ast_assignment_node_destroy(CbAstAssignmentNode* self);

/*
 * Evaluate/perform assignment
 */
CbVariant* cb_ast_assignment_node_eval(const CbAstAssignmentNode* self,
                                       const CbSymbolTable* symbols);

/*
 * Check semantics
 */
bool cb_ast_assignment_node_check_semantic(const CbAstAssignmentNode* self,
                                           CbSymbolTable* symbols);


#endif /* AST_ASSIGNMENT_H */
