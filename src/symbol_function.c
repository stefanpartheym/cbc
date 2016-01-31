#include "utils.h"
#include "symbol_function.h"
#include "symbol_internal.h"


/* -------------------------------------------------------------------------- */

struct CbSymbolFunction
{
    CbSymbol base;
    /*
     * TODO: Add necessary attributes
     *    - body: CbAstNode*
     */
};


/* -------------------------------------------------------------------------- */

/*
 * Get the functions return data type.
 */
static CbVariantType cb_symbol_function_get_data_type(const CbSymbolFunction* self);


/* -------------------------------------------------------------------------- */

CbSymbolFunction* cb_symbol_function_create(const char* identifier)
{
    CbSymbolFunction* self = memalloc(sizeof(CbSymbolFunction));
    cb_symbol_init(
        &self->base, CB_SYMBOL_TYPE_FUNCTION, identifier,
        (CbSymbolDestructorFunc)  cb_symbol_function_destroy,
        (CbSymbolGetDataTypeFunc) cb_symbol_function_get_data_type
    );

    return self;
}

void cb_symbol_function_destroy(CbSymbolFunction* self)
{
    memfree(self);
}


/* -------------------------------------------------------------------------- */

static CbVariantType cb_symbol_function_get_data_type(const CbSymbolFunction* self)
{
    /* TODO: Implement this function! */
    return CB_VARIANT_TYPE_UNDEFINED;
}
