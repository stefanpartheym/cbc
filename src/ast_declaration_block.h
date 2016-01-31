/*******************************************************************************
 * @file  ast_declaration_block.h
 * @brief Abstract syntax tree node: Declaration of multiple variables
 *
 * Representation of a variable declaration block in Codeblock source code.
 ******************************************************************************/

#ifndef AST_DECLARATION_BLOCK_H
#define AST_DECLARATION_BLOCK_H

#include "ast_declaration.h"


/**
 * @struct CbAstDeclarationBlockNode
 * @brief  Representation of a variable declaration block in Codeblock source
 *         code.
 *
 * @implements CbAstNode
 */
typedef struct CbAstDeclarationBlockNode CbAstDeclarationBlockNode;


/**
 * @memberof CbAstDeclarationBlockNode
 * @brief    Create a CbAstDeclarationBlockNode object.
 */
CbAstDeclarationBlockNode* cb_ast_declaration_block_node_create();

/**
 * @memberof CbAstDeclarationBlockNode
 * @brief    Destroy a CbAstDeclarationBlockNode object.
 *
 * @param self The CbAstDeclarationBlockNode instance
 */
void cb_ast_declaration_block_node_destroy(CbAstDeclarationBlockNode* self);

/**
 * @memberof CbAstDeclarationBlockNode
 * @brief    Evaluate a CbAstDeclarationBlockNode.
 *
 * @param self    The CbAstDeclarationBlockNode instance
 * @param symbols The symbol-table
 */
CbVariant* cb_ast_declaration_block_node_eval(const CbAstDeclarationBlockNode* self,
                                              const CbSymbolTable* symbols);

/**
 * @memberof CbAstDeclarationBlockNode
 * @brief    Check semantics of a CbAstDeclarationBlockNode.
 *
 * @param self    The CbAstDeclarationBlockNode instance
 * @param symbols The symbol-table
 */
bool cb_ast_declaration_block_node_check_semantic(const CbAstDeclarationBlockNode* self,
                                                  CbSymbolTable* symbols);

/**
 * @memberof CbAstDeclarationBlockNode
 * @brief    Add a declaration node to the internal declaration list.
 *
 * @param self The CbAstDeclarationBlockNode instance
 * @param node The declaration node to add to the internal list
 */
void cb_ast_declaration_block_node_add(CbAstDeclarationBlockNode* self,
                                       CbAstDeclarationNode* node);


#endif /* AST_DECLARATION_BLOCK_H */
