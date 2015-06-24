/*******************************************************************************
 * Abstract syntax tree node: StatementList
 * Connects two AST nodes.
 * 
 * Inherites from CbAstNode
 ******************************************************************************/

#ifndef AST_STATEMENT_LIST_H
#define AST_STATEMENT_LIST_H

#include "variant.h"
#include "symbol_table.h"
#include "ast.h"


/* -------------------------------------------------------------------------- */

/*
 * Constructor
 */
CbAstNode* cb_ast_statement_list_node_create(CbAstNode* left, CbAstNode* right);

/*
 * Destructor
 */
void cb_ast_statement_list_node_destroy(CbAstNode* self);

/*
 * Evaluate statement list
 */
CbVariant* cb_ast_statement_list_node_eval(const CbAstNode* self,
                                           const CbSymbolTable* symbols);

/*
 * Check semantics
 */
bool cb_ast_statement_list_node_check_semantic(const CbAstNode* self,
                                               CbSymbolTable* symbols);


#endif /* AST_STATEMENT_LIST_H */
