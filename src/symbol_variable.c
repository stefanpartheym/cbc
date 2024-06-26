#include "utils.h"
#include "symbol_variable.h"
#include "symbol_internal.h"


/* -------------------------------------------------------------------------- */

struct CbSymbolVariable
{
    CbSymbol base;
    CbVariant* value;
};


/* -------------------------------------------------------------------------- */

/*
 * Get the symbols data type.
 */
static CbVariantType cb_symbol_variable_get_data_type(const CbSymbolVariable* self);


/* -------------------------------------------------------------------------- */

CbSymbolVariable* cb_symbol_variable_create(const char* identifier)
{
    CbSymbolVariable* self = memalloc(sizeof(CbSymbolVariable));
    cb_symbol_init(
        &self->base, CB_SYMBOL_TYPE_VARIABLE, identifier,
        (CbSymbolDestructorFunc)  cb_symbol_variable_destroy,
        (CbSymbolGetDataTypeFunc) cb_symbol_variable_get_data_type
    );
    self->value = cb_variant_create();
    
    return self;
}

void cb_symbol_variable_destroy(CbSymbolVariable* self)
{
    cb_variant_destroy(self->value);
    memfree(self);
}

void cb_symbol_variable_assign(CbSymbolVariable* self, const CbVariant* value)
{
    /*
     * TODO: Implement cb_variant_assign(...) to avoid free()-ing and allocating
     *       a new CbVariant object each time.
     */
    
    cb_variant_destroy(self->value);
    self->value = cb_variant_copy(value);
}

const CbVariant* cb_symbol_variable_get_value(const CbSymbolVariable* self)
{
    return self->value;
}


/* -------------------------------------------------------------------------- */

static CbVariantType cb_symbol_variable_get_data_type(const CbSymbolVariable* self)
{
    return cb_variant_get_type(self->value);
}
