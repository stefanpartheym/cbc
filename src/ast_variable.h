/*******************************************************************************
 * @file  ast_variable.h
 * @brief Abstract syntax tree node: Variable
 * 
 * Representation of a variable in Codeblock source
 ******************************************************************************/

#ifndef AST_VARIABLE_H
#define AST_VARIABLE_H

#include "variant.h"
#include "symbol_table.h"


/**
 * @struct CbAstVariableNode
 * @brief  Representation of a variable in Codeblock source
 * 
 * @implements CbAstNode
 */
typedef struct CbAstVariableNode CbAstVariableNode;


/**
 * @memberof CbAstVariableNode
 * @brief    Constructor
 * 
 * @param identifier The name of the variable
 */
CbAstVariableNode* cb_ast_variable_node_create(const char* identifier);

/**
 * @memberof CbAstVariableNode
 * @brief    Destructor
 * 
 * @param self The CbAstVariableNode instance
 */
void cb_ast_variable_node_destroy(CbAstVariableNode* self);

/**
 * @memberof CbAstVariableNode
 * @brief    Evaluate a CbAstVariableNode
 * 
 * @param self The CbAstVariableNode instance
 */
CbVariant* cb_ast_variable_node_eval(const CbAstVariableNode* self,
                                     const CbSymbolTable* symbols);

/**
 * @memberof CbAstVariableNode
 * @brief    Check semantic
 * 
 * @param self    The CbAstVariableNode instance
 * @param symbols The symbol-table
 */
bool cb_ast_variable_node_check_semantic(const CbAstVariableNode* self,
                                         CbSymbolTable* symbols);


#endif /* AST_VARIABLE_H */
