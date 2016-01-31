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

/**
 * @memberof CbAstVariableNode
 * @brief    Check if expression type is valid
 *
 * @param self          The CbAstVariableNode instance
 * @param expected_type Expected expression type
 *
 * This function always returns true, since the data-type of the variable
 * is not known during semantic check.
 */
bool cb_ast_variable_node_check_expression_type(const CbAstVariableNode* self,
                                                CbVariantType expected_type);

/**
 * @memberof CbAstVariableNode
 * @brief    Check if the identifier of the variable is declared properly.
 *
 * @param self    The CbAstVariableNode instance.
 * @param symbols The symbol-table to look for the identifier.
 */
bool cb_ast_variable_node_is_declared(const CbAstVariableNode* self,
                                      const CbSymbolTable* symbols);

/**
 * @memberof CbAstVariableNode
 * @brief    Assign a new value to the symbol referenced by this node.
 *
 * @param self    The CbAstVariableNode instance.
 * @param symbols The symbol-table to look for the identifier.
 * @param value   The new value to be assigned.
 *
 * @return Returns a pointer to the new value assigned to the symbol.
 */
const CbVariant* cb_ast_variable_node_assign(CbAstVariableNode* self,
                                             const CbSymbolTable* symbols,
                                             const CbVariant* value);


#endif /* AST_VARIABLE_H */
