#include <stdlib.h>

#include "utils.h"
#include "cb_utils.h"
#include "error_handling.h"
#include "symbol_variable.h"
#include "ast_internal.h"
#include "ast_variable.h"


/* -------------------------------------------------------------------------- */

struct CbAstVariableNode
{
    CbAstNode base;
    char* identifier;
    
    /*
     * TODO:
     * Implement a cache (i.e. a pointer), that holds a reference to the proper
     * symbol object in the symbol table.
     * The cache should be filled during the semantic check of this node, so
     * that the cb_symbol_table_lookup()-function is only called once per
     * execution.
     */
    /* CbSymbolVariable* variable; */
};


/* -------------------------------------------------------------------------- */

static CbSymbolVariable* cb_ast_variable_node_get_symbol_from_table(const CbAstVariableNode* self,
                                                                    const CbSymbolTable* symbols);


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

CbVariant* cb_ast_variable_node_eval(const CbAstVariableNode* self,
                                     const CbSymbolTable* symbols)
{
    CbVariant* result = NULL;
    const CbSymbolVariable* symbol =
        cb_ast_variable_node_get_symbol_from_table(self, symbols);
    
    /* return copy of the value */
    result = cb_variant_copy(cb_symbol_variable_get_value(symbol));
    
    return result;
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

bool cb_ast_variable_node_is_declared(const CbAstVariableNode* self,
                                      const CbSymbolTable* symbols)
{
    CbSymbol* symbol = cb_symbol_table_lookup(symbols, self->identifier);
    if (symbol == NULL)
        return false;
    else
        return cb_symbol_is_variable(symbol);
}

const CbVariant* cb_ast_variable_node_assign(CbAstVariableNode* self,
                                             const CbSymbolTable* symbols,
                                             const CbVariant* value)
{
    CbSymbolVariable* symbol =
        cb_ast_variable_node_get_symbol_from_table(self, symbols);
    cb_symbol_variable_assign(symbol, value);
    return cb_symbol_variable_get_value(symbol);
}


/* -------------------------------------------------------------------------- */

static CbSymbolVariable* cb_ast_variable_node_get_symbol_from_table(const CbAstVariableNode* self,
                                                                    const CbSymbolTable* symbols)
{
    CbSymbol* symbol = cb_symbol_table_lookup(symbols, self->identifier);
    /* make sure symbol is valid */
    cb_assert(symbol != NULL);
    cb_assert(cb_symbol_is_variable(symbol));
    
    return (CbSymbolVariable*) symbol;
}
