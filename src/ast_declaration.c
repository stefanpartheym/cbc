#include "utils.h"
#include "cb_utils.h"
#include "error_handling.h"
#include "symbol_variable.h"
#include "symbol_function.h"
#include "ast_internal.h"
#include "ast_declaration.h"


/* -------------------------------------------------------------------------- */

struct CbAstDeclarationNode
{
    CbAstNode base;
    CbAstDeclarationType type;
    char* identifier;
};


/* -------------------------------------------------------------------------- */

CbAstDeclarationNode* cb_ast_declaration_node_create(CbAstDeclarationType type,
                                                     const char* identifier)
{
    CbAstDeclarationNode* self = (CbAstDeclarationNode*) memalloc(sizeof(CbAstDeclarationNode));
    cb_ast_node_init(
        &self->base, CB_AST_TYPE_DECLARATION, NULL, NULL,
        (CbAstNodeDestructorFunc) cb_ast_declaration_node_destroy,
        (CbAstNodeEvalFunc)       cb_ast_declaration_node_eval,
        (CbAstNodeSemanticFunc)   cb_ast_declaration_node_check_semantic
    );
    
    self->type       = type;
    self->identifier = strdup(identifier);
    
    return self;
}

void cb_ast_declaration_node_destroy(CbAstDeclarationNode* self)
{
    memfree(self->identifier);
    memfree(self);
}

CbVariant* cb_ast_declaration_node_eval(const CbAstDeclarationNode* self,
                                        const CbSymbolTable* symbols)
{
    /*
     * NOTE: The symbol was already declared during the semantic check.
     *       -> No further action requiered.
     */
    
    /*
     * TODO: Enable possibility to declare symbols "on the fly".
     *       This could be useful when deciding to evaluate the AST without
     *       doing a semantic check.
     */
    
    return cb_variant_create();
}

bool cb_ast_declaration_node_check_semantic(const CbAstDeclarationNode* self,
                                            CbSymbolTable* symbols)
{
    CbSymbol* symbol = cb_symbol_table_lookup(symbols, self->identifier);
    
    /*
     * There should not be a symbol with the same identifier yet!
     */
    if (symbol != NULL)
    {
        /*
         * There was already a declaration of the same identifier.
         */
        cb_error_trigger(
            CB_ERROR_SEMANTIC, self->base.line,
            "symbol '%s' already declared as %s in the current scope",
            self->identifier,
            cb_symbol_type_stringify(cb_symbol_get_type(symbol))
        );
        return false;
    }
    
    /*
     * Symbol was not declared yet. -> Declare symbol
     */
    switch (self->type)
    {
        case CB_AST_DECLARATION_TYPE_VARIABLE:
            symbol = (CbSymbol*) cb_symbol_variable_create(self->identifier);
            break;
        
        case CB_AST_DECLARATION_TYPE_FUNCTION:
            symbol = (CbSymbol*) cb_symbol_function_create(self->identifier);
            break;
        
        default: cb_abort("Invalid symbol declaration type"); break;
    }
    
    cb_symbol_table_insert(symbols, symbol);
    
    return true;
}
