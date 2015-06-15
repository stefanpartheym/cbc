#include <stdlib.h>

#include "utils.h"
#include "symbol.h"
#include "symbol_internal.h"


/* -------------------------------------------------------------------------- */

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

void cb_symbol_init(CbSymbol* self,
                    CbSymbolType type,
                    const char* identifier,
                    CbSymbolDestructorFunc destructor,
                    CbSymbolGetDataTypeFunc get_data_type)
{
    self->type          = type;
    self->identifier    = strdup(identifier);
    self->destructor    = destructor;
    self->get_data_type = get_data_type;
}

void cb_symbol_destroy(CbSymbol* self)
{
    memfree(self->identifier);
    self->destructor(self);
}

const char* cb_symbol_get_identifier(const CbSymbol* self)
{
    return self->identifier;
}

CbSymbolType cb_symbol_get_type(const CbSymbol* self)
{
    return self->type;
}

CbVariantType cb_symbol_get_data_type(const CbSymbol* self)
{
    return self->get_data_type(self);
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
