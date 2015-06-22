/*******************************************************************************
 * CbVariant is a data type that can hold any kind of data types (e.g. integer,
 * string, etc.)
 ******************************************************************************/

#ifndef VARIANT_H
#define VARIANT_H

#include <stdbool.h>


/* -------------------------------------------------------------------------- */

typedef struct CbVariant CbVariant;
typedef enum
{
    CB_VARIANT_TYPE_UNDEFINED,
    CB_VARIANT_TYPE_INTEGER,
    CB_VARIANT_TYPE_FLOAT,
    CB_VARIANT_TYPE_NUMERIC, /* pseudo, variant type */
    CB_VARIANT_TYPE_BOOLEAN,
    CB_VARIANT_TYPE_STRING
} CbVariantType;

/* data type aliases */
typedef long int CbIntegerDataType;
typedef double   CbFloatDataType;


/* -------------------------------------------------------------------------- */

/*
 * String representation of a CbVariantType.
 */
const char* const cb_variant_type_stringify(const CbVariantType type);

/*
 * Check if a specific variant type is valid.
 */
bool cb_variant_type_is_valid(const CbVariantType type);


/* -------------------------------------------------------------------------- */
/* basic variant type functions */

/*
 * Constructor (default)
 */
CbVariant* cb_variant_create();

/*
 * Destructor
 */
void cb_variant_destroy(CbVariant* self);

/*
 * Constructor (Copy)
 */
CbVariant* cb_variant_copy(const CbVariant* variant);

/*
 * Variant type (Getter)
 */
CbVariantType cb_variant_get_type(const CbVariant* self);

/*
 * Convert a variant to its string representation.
 * NOTE: The returned c string needs to be freed after usage!
 */
char* cb_variant_to_string(const CbVariant* self);

/*
 * Print the string representation of the variant.
 */
void cb_variant_print(const CbVariant* self);

/*
 * Check if variant object has a specific type
 */
bool cb_variant_is_type(const CbVariant* self, const CbVariantType type);

/*
 * Check if variant is undefined
 */
bool cb_variant_is_undefined(const CbVariant* self);

/*
 * Check if variant is an integer value
 */
bool cb_variant_is_integer(const CbVariant* self);

/*
 * Check if variant is a float value
 */
bool cb_variant_is_float(const CbVariant* self);

/*
 * Check if variant is numeric (either integer or float)
 */
bool cb_variant_is_numeric(const CbVariant* self);

/*
 * Check if variant is boolean
 */
bool cb_variant_is_boolean(const CbVariant* self);

/*
 * Check if variant is a string
 */
bool cb_variant_is_string(const CbVariant* self);


/* -------------------------------------------------------------------------- */
/* integer variant type functions */

/*
 * Constructor (Integer)
 */
CbVariant* cb_integer_create(const CbIntegerDataType value);

/*
 * Integer value (Getter)
 */
CbIntegerDataType cb_integer_get_value(const CbVariant* self);


/* -------------------------------------------------------------------------- */
/* float variant type functions */

/*
 * Constructor (Float)
 */
CbVariant* cb_float_create(const CbFloatDataType value);

/*
 * Float value (Getter)
 */
CbFloatDataType cb_float_get_value(const CbVariant* self);


#endif /* VARIANT_H */
