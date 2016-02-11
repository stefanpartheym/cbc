/*******************************************************************************
 * Abstract syntax tree node: Debug print
 * This AST node is for debugging purposes: Print a Codeblock value to stdout.
 *
 * Inherites from CbAstNode
 ******************************************************************************/

#ifndef AST_DEBUG_PRINT_H
#define AST_DEBUG_PRINT_H

#include "variant.h"


/* -------------------------------------------------------------------------- */

typedef struct CbAstDebugPrintNode CbAstDebugPrintNode;


/* -------------------------------------------------------------------------- */

/*
 * Constructor
 */
CbAstDebugPrintNode* cb_ast_debug_print_node_create(CbAstNode* expression);

/*
 * Destructor
 */
void cb_ast_debug_print_node_destroy(CbAstDebugPrintNode* self);

/*
 * Evaluate value node
 */
CbVariant* cb_ast_debug_print_node_eval(const CbAstDebugPrintNode* self,
                                        const CbSymbolTable* symbols);

/*
 * Check semantics
 */
bool cb_ast_debug_print_node_check_semantic(const CbAstDebugPrintNode* self,
                                            CbSymbolTable* symbols);


#endif /* AST_DEBUG_PRINT_H */
