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
typedef long int    CbIntegerDataType;
typedef double      CbFloatDataType;
typedef bool        CbBooleanDataType;
typedef char*       CbStringDataType;
typedef const char* CbConstStringDataType;

/* unary operators */
typedef enum
{
    CB_UNARY_OPERATOR_TYPE_MINUS,
    CB_UNARY_OPERATOR_TYPE_LOGICAL_NOT
} CbUnaryOperatorType;

/* binary operators */
typedef enum
{
    CB_BINARY_OPERATOR_TYPE_ADD,
    CB_BINARY_OPERATOR_TYPE_SUB,
    CB_BINARY_OPERATOR_TYPE_MUL,
    CB_BINARY_OPERATOR_TYPE_DIV,
    CB_BINARY_OPERATOR_TYPE_LOGICAL_AND,
    CB_BINARY_OPERATOR_TYPE_LOGICAL_OR,
    CB_BINARY_OPERATOR_TYPE_COMPARISON_GT, /* greater than          */
    CB_BINARY_OPERATOR_TYPE_COMPARISON_GE, /* greater than or equal */
    CB_BINARY_OPERATOR_TYPE_COMPARISON_LT, /* less than             */
    CB_BINARY_OPERATOR_TYPE_COMPARISON_LE, /* less than or equal    */
    CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, /* equal                 */
    CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, /* string equal          */
    CB_BINARY_OPERATOR_TYPE_COMPARISON_NE  /* not equal             */
} CbBinaryOperatorType;


/* -------------------------------------------------------------------------- */

/*
 * String representation of a CbVariantType.
 */
const char* const cb_variant_type_stringify(const CbVariantType type);

/*
 * Get string representation of an unary operator type
 */
const char* const cb_unary_operator_type_stringify(CbUnaryOperatorType type);

/*
 * Get string representation of an binary operator type
 */
const char* const cb_binary_operator_type_stringify(CbBinaryOperatorType type);

/*
 * Check if a specific variant type is valid.
 */
bool cb_variant_type_is_valid(const CbVariantType type);

/*
 * Check if a specific variant type is numeric.
 */
bool cb_variant_type_is_numeric(const CbVariantType type);

/*
 * Check if unary operation is valid
 */
bool cb_variant_type_is_unary_operation_valid(const CbUnaryOperatorType operation,
                                              const CbVariantType type);

/*
 * Check if binary operation is valid
 */
bool cb_variant_type_is_binary_operation_valid(const CbBinaryOperatorType operation,
                                               const CbVariantType lhs,
                                               const CbVariantType rhs);


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
/* numeric variant type functions */

/*
 * Get numeric value as integer.
 */
CbIntegerDataType cb_numeric_as_integer(const CbVariant* self);

/*
 * Get numeric value as double.
 */
CbFloatDataType cb_numeric_as_float(const CbVariant* self);


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


/* -------------------------------------------------------------------------- */
/* boolean variant type functions */

/*
 * Constructor (Boolean)
 */
CbVariant* cb_boolean_create(const CbBooleanDataType value);

/*
 * Boolean value (Getter)
 */
CbFloatDataType cb_boolean_get_value(const CbVariant* self);


/* -------------------------------------------------------------------------- */
/* string variant type functions */

/*
 * Constructor (String)
 */
CbVariant* cb_string_create(CbConstStringDataType value);

/*
 * String value (Getter)
 */
CbConstStringDataType cb_string_get_value(const CbVariant* self);

/*
 * Compare two strings
 */
CbBooleanDataType cb_string_equal(const CbVariant* lhs, const CbVariant* rhs);

/*
 * Perform left handed side string comparison
 */
CbBooleanDataType cb_string_lhs_equal(const CbVariant* lhs, const CbVariant* rhs);

/*
 * Concatenate two strings together
 */
void cb_string_concat(CbVariant* self, const CbVariant* source);


#endif /* VARIANT_H */
