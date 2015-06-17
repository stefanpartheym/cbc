#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "cb_utils.h"
#include "variant.h"


/* -------------------------------------------------------------------------- */

struct CbVariant
{
    CbVariantType type;
    
    union
    {
        CbIntegerDataType integer;
        CbFloatDataType   decimal;
    } v;
};

static const char* const CB_VARIANT_TYPE_STRINGS[] = {
    "undefined", /* CB_VARIANT_TYPE_UNDEFINED */
    "integer",   /* CB_VARIANT_TYPE_INTEGER   */
    "float",     /* CB_VARIANT_TYPE_FLOAT     */
    "numeric",   /* CB_VARIANT_TYPE_NUMERIC   */
    "boolean",   /* CB_VARIANT_TYPE_BOOLEAN   */
    "string"     /* CB_VARIANT_TYPE_STRING    */
};


/* -------------------------------------------------------------------------- */

const char* const cb_variant_type_stringify(const CbVariantType type)
{
    cb_assert(cb_variant_type_is_valid(type));
    
    return CB_VARIANT_TYPE_STRINGS[type];
}

bool cb_variant_type_is_valid(const CbVariantType type)
{
    return ((type >= CB_VARIANT_TYPE_UNDEFINED) &&
            (type <= CB_VARIANT_TYPE_STRING));
}

CbVariant* cb_variant_create()
{
    CbVariant* self = (CbVariant*) memalloc(sizeof(CbVariant));
    self->type      = CB_VARIANT_TYPE_UNDEFINED;
    
    return self;
}

void cb_variant_destroy(CbVariant* self)
{
    switch (self->type)
    {
        case CB_VARIANT_TYPE_FLOAT:
        case CB_VARIANT_TYPE_INTEGER:
            /* TODO: currently there is no action requiered */
            break;
        
        case CB_VARIANT_TYPE_BOOLEAN:
            cb_abort("CB_VARIANT_TYPE_BOOLEAN: Not yet implemented"); break;
        
        case CB_VARIANT_TYPE_STRING:
            cb_abort("CB_VARIANT_TYPE_STRING: Not yet implemented"); break;
        
        case CB_VARIANT_TYPE_UNDEFINED:
            /* undefined variant does not requiere any additional action */
            break;
        
        /* invalid variant type */
        default: cb_abort("Invalid variant type"); break;
    }
    
    memfree(self);
}

CbVariant* cb_variant_copy(const CbVariant* variant)
{
    CbVariant* copy = NULL;
    
    cb_assert(cb_variant_type_is_valid(variant->type));
    
    copy       = cb_variant_create();
    copy->type = variant->type;
    
    switch (variant->type)
    {
        case CB_VARIANT_TYPE_INTEGER:
            copy->v.integer = variant->v.integer; break;
        
        case CB_VARIANT_TYPE_FLOAT:
            copy->v.decimal = variant->v.decimal; break;
        
        case CB_VARIANT_TYPE_BOOLEAN:
            cb_abort("CB_VARIANT_TYPE_BOOLEAN: Not yet implemented"); break;
        
        case CB_VARIANT_TYPE_STRING:
            cb_abort("CB_VARIANT_TYPE_STRING: Not yet implemented");  break;
        
        /* No action requiered -> break */
        case CB_VARIANT_TYPE_UNDEFINED: break;
        
        default: cb_abort("Invalid variant type"); break;
    }
    
    return copy;
}

CbVariantType cb_variant_get_type(const CbVariant* self)
{
    return self->type;
}

bool cb_variant_is_type(const CbVariant* self, const CbVariantType type)
{
    if (type == CB_VARIANT_TYPE_NUMERIC)
        return cb_variant_is_numeric(self);
    else
        return (type == self->type);
}

bool cb_variant_is_undefined(const CbVariant* self)
{
    return cb_variant_is_type(self, CB_VARIANT_TYPE_UNDEFINED);
}

bool cb_variant_is_integer(const CbVariant* self)
{
    return cb_variant_is_type(self, CB_VARIANT_TYPE_INTEGER);
}

bool cb_variant_is_float(const CbVariant* self)
{
    return cb_variant_is_type(self, CB_VARIANT_TYPE_FLOAT);
}

bool cb_variant_is_numeric(const CbVariant* self)
{
    return (cb_variant_is_integer(self) || cb_variant_is_float(self));
}

bool cb_variant_is_boolean(const CbVariant* self)
{
    return cb_variant_is_type(self, CB_VARIANT_TYPE_BOOLEAN);
}

bool cb_variant_is_string(const CbVariant* self)
{
    return cb_variant_is_type(self, CB_VARIANT_TYPE_STRING);
}


/* -------------------------------------------------------------------------- */

CbVariant* cb_integer_create(const CbIntegerDataType value)
{
    CbVariant* self = cb_variant_create();
    self->type      = CB_VARIANT_TYPE_INTEGER;
    self->v.integer = value;
    
    return self;
}

CbIntegerDataType cb_integer_get_value(const CbVariant* self)
{
    cb_assert(cb_variant_is_integer(self));
    
    return self->v.integer;
}


/* -------------------------------------------------------------------------- */

CbVariant* cb_float_create(const CbFloatDataType value)
{
    CbVariant* self = cb_variant_create();
    self->type      = CB_VARIANT_TYPE_FLOAT;
    self->v.decimal = value;
    
    return self;
}

CbFloatDataType cb_float_get_value(const CbVariant* self)
{
    cb_assert(cb_variant_is_float(self));
    
    return self->v.decimal;
}
