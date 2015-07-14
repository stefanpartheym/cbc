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
        CbBooleanDataType boolean;
        CbStringDataType  string;
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

static const char* const CB_VARIANT_DISPLAY_VALUE_UNDEFINED = "<undefined>";
static const char* const CB_VARIANT_DISPLAY_VALUE_TRUE      = "True";
static const char* const CB_VARIANT_DISPLAY_VALUE_FALSE     = "False";


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
        case CB_VARIANT_TYPE_BOOLEAN:
            /* TODO: currently there is no action requiered */
            break;
        
        case CB_VARIANT_TYPE_STRING:
            memfree(self->v.string); break;
        
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
            copy->v.boolean = variant->v.boolean; break;
        
        case CB_VARIANT_TYPE_STRING:
            copy->v.string = strdup(variant->v.string); break;
        
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

char* cb_variant_to_string(const CbVariant* self)
{
    char* result;
    
    switch (self->type)
    {
        case CB_VARIANT_TYPE_UNDEFINED:
            result = memalloc(strlen(CB_VARIANT_DISPLAY_VALUE_UNDEFINED) + 1);
            sprintf(result, CB_VARIANT_DISPLAY_VALUE_UNDEFINED);
            break;
        
        case CB_VARIANT_TYPE_INTEGER:
            /* 63 digits should be enough for a long int */
            result = memalloc(64);
            sprintf(result, "%ld", self->v.integer);
            break;
        
        case CB_VARIANT_TYPE_FLOAT:
            result = memalloc(512);
            sprintf(result, "%f", self->v.decimal);
            break;
        
        case CB_VARIANT_TYPE_BOOLEAN:
            result = memalloc(6);
            const char* display_value = CB_VARIANT_DISPLAY_VALUE_FALSE;
            if (self->v.boolean)
                display_value = CB_VARIANT_DISPLAY_VALUE_TRUE;
            
            sprintf(result, "%s", display_value);
            break;
        
        case CB_VARIANT_TYPE_STRING:
            result = memalloc(strlen(self->v.string) + 1);
            sprintf(result, "%s", self->v.string);
            break;
        
        default: cb_abort("Invalid variant type"); break;
    }
    
    return result;
}

void cb_variant_print(const CbVariant* self)
{
    char* variant_str = cb_variant_to_string(self);
    printf("%s", variant_str);
    memfree(variant_str);
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

CbIntegerDataType cb_numeric_as_integer(const CbVariant* self)
{
    CbIntegerDataType result;
    cb_assert(cb_variant_is_numeric(self));
    
    if (cb_variant_is_float(self))
        result = (CbIntegerDataType) cb_float_get_value(self);
    else
        result = cb_integer_get_value(self);
    
    return result;
}

CbFloatDataType cb_numeric_as_float(const CbVariant* self)
{
    CbFloatDataType result;
    cb_assert(cb_variant_is_numeric(self));
    
    if (cb_variant_is_float(self))
        result = cb_float_get_value(self);
    else
        result = (CbFloatDataType) cb_integer_get_value(self);
    
    return result;
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


/* -------------------------------------------------------------------------- */

CbVariant* cb_boolean_create(const CbBooleanDataType value)
{
    CbVariant* self = cb_variant_create();
    self->type      = CB_VARIANT_TYPE_BOOLEAN;
    self->v.boolean = value;
    
    return self;
}

CbFloatDataType cb_boolean_get_value(const CbVariant* self)
{
    cb_assert(cb_variant_is_boolean(self));
    
    return self->v.boolean;
}


/* -------------------------------------------------------------------------- */

CbVariant* cb_string_create(CbConstStringDataType value)
{
    CbVariant* self = cb_variant_create();
    self->type      = CB_VARIANT_TYPE_STRING;
    self->v.string  = strdup(value);
    
    return self;
}

CbConstStringDataType cb_string_get_value(const CbVariant* self)
{
    cb_assert(cb_variant_is_string(self));
    
    return self->v.string;
}
