#include <stdlib.h>

#include "utils.h"
#include "error_handling.h"
#include "ast_internal.h"
#include "ast_variable.h"


/* -------------------------------------------------------------------------- */

struct CbAstVariableNode
{
    CbAstNode base;
    char* identifier;
};


/* -------------------------------------------------------------------------- */

CbAstVariableNode* cb_ast_variable_node_create(const char* identifier)
{
    CbAstVariableNode* self = (CbAstVariableNode*) memalloc(sizeof(CbAstVariableNode));
    cb_ast_node_init(
        &self->base, CB_AST_TYPE_VARIABLE, NULL, NULL,
        (CbAstNodeDestructorFunc) cb_ast_variable_node_destroy,
        (CbAstNodeEvalFunc)       cb_ast_variable_node_eval,
        (CbAstNodeSemanticFunc)   cb_ast_variable_node_check_semantic
    );
    
    self->identifier = strdup(identifier);
    
    return self;
}

void cb_ast_variable_node_destroy(CbAstVariableNode* self)
{
    memfree(self->identifier);
    memfree(self);
}

CbVariant* cb_ast_variable_node_eval(const CbAstVariableNode* self)
{
    /*
     * TODO: Evaluate variable value
     */
    return NULL;
}

bool cb_ast_variable_node_check_semantic(const CbAstVariableNode* self,
                                         CbSymbolTable* symbols)
{
    CbSymbol* symbol = cb_symbol_table_lookup(symbols, self->identifier);
    
    if (symbol == NULL)
    {
        /*
         * Raise an error, if variable is not declared in an available scope
         */
        cb_error_trigger(
            CB_ERROR_SEMANTIC, self->base.line,
            "variable '%s' is not declared in an available scope",
            self->identifier
        );
        return false;
    }
    else if (cb_symbol_is_variable(symbol) == false)
    {
        /*
         * Raise an error, if identifier is not declared as variable
         */
        cb_error_trigger(
            CB_ERROR_SEMANTIC, self->base.line,
            "identifier '%s' is declared as %s, not as %s",
            self->identifier,
            cb_symbol_type_stringify(cb_symbol_get_type(symbol)),
            cb_symbol_type_stringify(CB_SYMBOL_TYPE_VARIABLE)
        );
        return false;
    }
    
    return true;
}
