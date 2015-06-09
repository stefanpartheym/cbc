/*******************************************************************************
 * Implementation of CbSymbol
 ******************************************************************************/

#include <stdlib.h>

#include "utils.h"
#include "symbol.h"


/* -------------------------------------------------------------------------- */

struct CbSymbol
{
    CbSymbolType type;
    char* identifier;
    CbVariantType data_type;
};

const char* const CB_SYMBOL_TYPE_STRINGS[] = {
    "variable", /* CB_SYMBOL_TYPE_VARIABLE */
    "function"  /* CB_SYMBOL_TYPE_FUNCTION */
};


/* -------------------------------------------------------------------------- */

/*
 * Check if symbol has a specific type
 */
static bool cb_symbol_is_type(const CbSymbol* self, CbSymbolType type);


/* -------------------------------------------------------------------------- */

const char* cb_symbol_type_stringify(CbSymbolType type)
{
    /* TODO: make sure type is valid */
    return CB_SYMBOL_TYPE_STRINGS[type];
}

CbSymbol* cb_symbol_create(const char* identifier,
                           CbSymbolType type,
                           CbVariantType data_type)
{
    CbSymbol* self   = (CbSymbol*) memalloc(sizeof(CbSymbol));
    self->identifier = strdup(identifier);
    self->type       = type;
    self->data_type  = data_type;
    
    return self;
}

void cb_symbol_destroy(CbSymbol* self)
{
    memfree(self->identifier);
    memfree(self);
}

const char* cb_symbol_get_identifier(const CbSymbol* self)
{
    return self->identifier;
}

CbSymbolType cb_symbol_get_type(const CbSymbol* self)
{
    return self->type;
}

bool cb_symbol_is_variable(const CbSymbol* self)
{
    return cb_symbol_is_type(self, CB_SYMBOL_TYPE_VARIABLE);
}

bool cb_symbol_is_function(const CbSymbol* self)
{
    return cb_symbol_is_type(self, CB_SYMBOL_TYPE_FUNCTION);
}

/* -------------------------------------------------------------------------- */

static bool cb_symbol_is_type(const CbSymbol* self, CbSymbolType type)
{
    return (type == self->type);
}
