/*******************************************************************************
 * @file  ast_declaration.h
 * @brief Abstract syntax tree node: Declaration of a symbol
 *
 * Representation of a symbol declaration in Codeblock source code
 ******************************************************************************/

#ifndef AST_DECLARATION_H
#define AST_DECLARATION_H

#include "variant.h"
#include "symbol_table.h"


/**
 * @struct CbAstDeclarationNode
 * @brief  Representation of a variable declaration in Codeblock source code
 *
 * @implements CbAstNode
 */
typedef struct CbAstDeclarationNode CbAstDeclarationNode;

/**
 * @enum  CbAstDeclarationType
 * @brief The declaration type, specifies what kind of symbol is being declared.
 */
typedef enum CbAstDeclarationType
{
    CB_AST_DECLARATION_TYPE_VARIABLE,
    CB_AST_DECLARATION_TYPE_FUNCTION
} CbAstDeclarationType;


/**
 * @memberof CbAstDeclarationNode
 * @brief    Create a CbAstDeclarationNode object.
 *
 * @param identifier The name of the symbol to be declared
 */
CbAstDeclarationNode* cb_ast_declaration_node_create(CbAstDeclarationType type,
                                                     const char* identifier);

/**
 * @memberof CbAstDeclarationNode
 * @brief    Destroy a CbAstDeclarationNode object.
 *
 * @param self The CbAstDeclarationNode instance
 */
void cb_ast_declaration_node_destroy(CbAstDeclarationNode* self);

/**
 * @memberof CbAstDeclarationNode
 * @brief    Evaluate a CbAstDeclarationNode.
 *
 * @param self    The CbAstDeclarationNode instance
 * @param symbols The symbol-table
 */
CbVariant* cb_ast_declaration_node_eval(const CbAstDeclarationNode* self,
                                        const CbSymbolTable* symbols);

/**
 * @memberof CbAstDeclarationNode
 * @brief    Check semantics of a CbAstDeclarationNode.
 *
 * @param self    The CbAstDeclarationNode instance
 * @param symbols The symbol-table
 */
bool cb_ast_declaration_node_check_semantic(const CbAstDeclarationNode* self,
                                            CbSymbolTable* symbols);


#endif /* AST_DECLARATION_H */
