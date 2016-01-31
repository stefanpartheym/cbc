#include <stdlib.h>

#include "utils.h"
#include "cb_utils.h"
#include "stack.h"
#include "hash_table.h"
#include "symbol_table.h"


/* -------------------------------------------------------------------------- */

struct CbSymbolTable
{
    CbStack* scope_stack;
    CbScope* global_scope;
};


static CbHashTable* cb_symbol_table_get_current_symbols(const CbSymbolTable* self);


/* -------------------------------------------------------------------------- */

CbSymbolTable* cb_symbol_table_create()
{
    CbSymbolTable* self = (CbSymbolTable*) memalloc(sizeof(CbSymbolTable));
    self->scope_stack   = cb_stack_create();
    self->global_scope  = cb_scope_create(NULL); /* create global scope */

    cb_stack_push(self->scope_stack, self->global_scope);

    return self;
}

void cb_symbol_table_destroy(CbSymbolTable* self)
{
    cb_assert(cb_stack_pop(self->scope_stack, NULL));
    cb_stack_destroy(self->scope_stack);
    cb_scope_destroy(self->global_scope); /* destroy global scope separately */
    memfree(self);
}

const CbSymbol* cb_symbol_table_insert(const CbSymbolTable* self,
                                       CbSymbol* symbol)
{
    CbHashTable* symbols   = cb_symbol_table_get_current_symbols(self);
    const char* identifier = cb_symbol_get_identifier(symbol);
    CbSymbol* result       = NULL;

    result = cb_hash_table_get(symbols, identifier);
    if (result == NULL)
        /*
         * Insert symbol only if there isn't already  any symbol with the same
         * identifier in the current scope.
         */
        cb_hash_table_insert(symbols, identifier, symbol);

    return result;
}

CbSymbol* cb_symbol_table_lookup(const CbSymbolTable* self,
                                 const char* identifier)
{
    CbSymbol* result       = NULL;
    const CbScope* current = cb_stack_get_top_item(self->scope_stack);
    const CbScope* parent  = cb_scope_get_parent(current);
    CbHashTable* symbols   = cb_scope_get_symbols(current);

    result = cb_hash_table_get(symbols, identifier);
    while (result == NULL && parent != NULL)
    {
        symbols = cb_scope_get_symbols(parent);
        result  = cb_hash_table_get(symbols, identifier);
        parent  = cb_scope_get_parent(parent);
    }

    return result;
}

void cb_symbol_table_enter_scope(CbSymbolTable* self)
{
    const CbScope* parent = cb_stack_get_top_item(self->scope_stack);
    cb_symbol_table_switch_scope(self, parent);
}

void cb_symbol_table_switch_scope(CbSymbolTable* self, const CbScope* parent)
{
    CbScope* new_scope;

    if (parent == NULL)
        parent = self->global_scope;

    new_scope = cb_scope_create(parent);
    cb_stack_push(self->scope_stack, new_scope);
}

void cb_symbol_table_leave_scope(CbSymbolTable* self)
{
    CbScope* current;
    cb_assert(cb_stack_pop(self->scope_stack, (void**) &current));
    cb_scope_destroy(current);
}


/* -------------------------------------------------------------------------- */

static CbHashTable* cb_symbol_table_get_current_symbols(const CbSymbolTable* self)
{
    const CbScope* current = cb_stack_get_top_item(self->scope_stack);
    cb_assert(current != NULL);
    return  cb_scope_get_symbols(current);
}
