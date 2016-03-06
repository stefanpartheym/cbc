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

static const char* const CB_UNARY_OPERATOR_TYPE_STRINGS[] = {
    "-",   /* CB_UNARY_OPERATOR_TYPE_MINUS       */
    "not"  /* CB_UNARY_OPERATOR_TYPE_LOGICAL_NOT */
};

static const char* CB_BINARY_OPERATOR_TYPE_STRINGS[] = {
    "+",   /* CB_BINARY_OPERATOR_TYPE_ADD           */
    "-",   /* CB_BINARY_OPERATOR_TYPE_SUB           */
    "*",   /* CB_BINARY_OPERATOR_TYPE_MUL           */
    "/",   /* CB_BINARY_OPERATOR_TYPE_DIV           */
    "and", /* CB_BINARY_OPERATOR_TYPE_LOGICAL_AND   */
    "or",  /* CB_BINARY_OPERATOR_TYPE_LOGICAL_OR    */
    ">",   /* CB_BINARY_OPERATOR_TYPE_COMPARISON_GR */
    ">=",  /* CB_BINARY_OPERATOR_TYPE_COMPARISON_GQ */
    "<",   /* CB_BINARY_OPERATOR_TYPE_COMPARISON_LE */
    "<=",  /* CB_BINARY_OPERATOR_TYPE_COMPARISON_LQ */
    "=",   /* CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ */
    "==",  /* CB_BINARY_OPERATOR_TYPE_COMPARISON_SQ */
    "<>"   /* CB_BINARY_OPERATOR_TYPE_COMPARISON_NE */
};


/* -------------------------------------------------------------------------- */

const char* const cb_variant_type_stringify(const CbVariantType type)
{
    cb_assert(cb_variant_type_is_valid(type));

    return CB_VARIANT_TYPE_STRINGS[type];
}

const char* const cb_unary_operator_type_stringify(CbUnaryOperatorType type)
{
    /* TODO: make sure type arguemnt is valid */
    return CB_UNARY_OPERATOR_TYPE_STRINGS[type];
}

const char* const cb_binary_operator_type_stringify(CbBinaryOperatorType type)
{
    /* TODO: make sure type arguemnt is valid */
    return CB_BINARY_OPERATOR_TYPE_STRINGS[type];
}

bool cb_variant_type_is_valid(const CbVariantType type)
{
    return ((type >= CB_VARIANT_TYPE_UNDEFINED) &&
            (type <= CB_VARIANT_TYPE_STRING));
}

bool cb_variant_type_is_numeric(const CbVariantType type)
{
    return (type == CB_VARIANT_TYPE_INTEGER) ||
           (type == CB_VARIANT_TYPE_FLOAT)   ||
           (type == CB_VARIANT_TYPE_NUMERIC);
}

bool cb_variant_type_is_unary_operation_valid(const CbUnaryOperatorType operation,
                                              const CbVariantType type)
{
    bool result = false;
    switch (operation)
    {
        case CB_UNARY_OPERATOR_TYPE_MINUS:
            result = cb_variant_type_is_numeric(type) ||
                     type == CB_VARIANT_TYPE_UNDEFINED;
            break;

        case CB_UNARY_OPERATOR_TYPE_LOGICAL_NOT:
            result = type == CB_VARIANT_TYPE_BOOLEAN ||
                     type == CB_VARIANT_TYPE_UNDEFINED;
            break;

        default: cb_abort("Invalid unary operator type"); break;
    }

    return result;
}

bool cb_variant_type_is_binary_operation_valid(const CbBinaryOperatorType operation,
                                               const CbVariantType lhs,
                                               const CbVariantType rhs)
{
    bool result = false;
    switch (operation)
    {
        case CB_BINARY_OPERATOR_TYPE_ADD:
            result = (cb_variant_type_is_numeric(lhs) &&
                      cb_variant_type_is_numeric(rhs)) ||
                     (lhs == CB_VARIANT_TYPE_STRING &&
                      rhs == CB_VARIANT_TYPE_STRING) ||
                     (lhs == CB_VARIANT_TYPE_UNDEFINED ||
                      rhs == CB_VARIANT_TYPE_UNDEFINED);
            break;

        case CB_BINARY_OPERATOR_TYPE_SUB:
        case CB_BINARY_OPERATOR_TYPE_MUL:
        case CB_BINARY_OPERATOR_TYPE_DIV:
        case CB_BINARY_OPERATOR_TYPE_COMPARISON_GT:
        case CB_BINARY_OPERATOR_TYPE_COMPARISON_GE:
        case CB_BINARY_OPERATOR_TYPE_COMPARISON_LT:
        case CB_BINARY_OPERATOR_TYPE_COMPARISON_LE:
            result = (cb_variant_type_is_numeric(lhs) &&
                      cb_variant_type_is_numeric(rhs)) ||
                     (lhs == CB_VARIANT_TYPE_UNDEFINED ||
                      rhs == CB_VARIANT_TYPE_UNDEFINED);
            break;

        case CB_BINARY_OPERATOR_TYPE_LOGICAL_OR:
        case CB_BINARY_OPERATOR_TYPE_LOGICAL_AND:
            result = (lhs == CB_VARIANT_TYPE_BOOLEAN &&
                      rhs == CB_VARIANT_TYPE_BOOLEAN) ||
                     (lhs == CB_VARIANT_TYPE_UNDEFINED ||
                      rhs == CB_VARIANT_TYPE_UNDEFINED);
            break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ:
        case CB_BINARY_OPERATOR_TYPE_COMPARISON_SE:
        case CB_BINARY_OPERATOR_TYPE_COMPARISON_NE:
            result = (cb_variant_type_is_numeric(lhs) &&
                      cb_variant_type_is_numeric(rhs)) ||
                     (lhs == CB_VARIANT_TYPE_STRING &&
                      rhs == CB_VARIANT_TYPE_STRING) ||
                     (lhs == CB_VARIANT_TYPE_BOOLEAN &&
                      rhs == CB_VARIANT_TYPE_BOOLEAN) ||
                     (lhs == CB_VARIANT_TYPE_UNDEFINED ||
                      rhs == CB_VARIANT_TYPE_UNDEFINED);
            break;

        default: cb_abort("Invalid binary operator type"); break;
    }

    return result;
}


/* -------------------------------------------------------------------------- */

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

bool cb_variant_equals(const CbVariant* v1, const CbVariant* v2)
{
    CbBinaryOperatorType operator_type = CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ;
    bool result = cb_variant_type_is_binary_operation_valid(operator_type,
                                                            v1->type,
                                                            v2->type);
    if (result)
    {
        CbVariant* is_equal;
        switch (v1->type)
        {
            case CB_VARIANT_TYPE_INTEGER:
                is_equal = cb_integer_perform_binary_operation(operator_type, v1, v2);
                break;

            case CB_VARIANT_TYPE_FLOAT:
                is_equal = cb_float_perform_binary_operation(operator_type, v1, v2);
                break;

            case CB_VARIANT_TYPE_BOOLEAN:
                is_equal = cb_boolean_perform_binary_operation(operator_type, v1, v2);
                break;

            case CB_VARIANT_TYPE_STRING:
                is_equal = cb_string_perform_binary_operation(operator_type, v1, v2);
                break;

            default: is_equal = NULL;
        }

        if (is_equal == NULL)
            result = false;
        else
        {
            result = cb_boolean_get_value(is_equal);
            cb_variant_destroy(is_equal);
        }
    }

    return result;
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

CbVariant* cb_integer_perform_binary_operation(CbBinaryOperatorType operator_type,
                                               const CbVariant* left,
                                               const CbVariant* right)
{
    CbVariant* result    = NULL;
    CbIntegerDataType v1 = cb_integer_get_value(left);
    CbIntegerDataType v2 = cb_integer_get_value(right);

    switch (operator_type)
    {
        case CB_BINARY_OPERATOR_TYPE_ADD:
            result = cb_integer_create(v1 + v2); break;

        case CB_BINARY_OPERATOR_TYPE_SUB:
            result = cb_integer_create(v1 - v2); break;

        case CB_BINARY_OPERATOR_TYPE_MUL:
            result = cb_integer_create(v1 * v2); break;

        case CB_BINARY_OPERATOR_TYPE_DIV:
            if (v2 == 0)
                cb_abort("Division by zero is not allowed");
            else
            {
                if ((v1 % v2) == 0)
                    /*
                     * Division does not yield a real number.
                     * -> Create an integer result.
                     */
                    result = cb_integer_create(v1 / v2);
                else
                    /*
                     * Division yields a real number -> Create a float result.
                     */
                    result = cb_float_create((CbFloatDataType) v1 /
                                             (CbFloatDataType) v2);
            }
            break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_GT:
            result = cb_boolean_create(v1 > v2); break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_GE:
            result = cb_boolean_create(v1 >= v2); break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_LT:
            result = cb_boolean_create(v1 < v2); break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_LE:
            result = cb_boolean_create(v1 <= v2); break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ:
        case CB_BINARY_OPERATOR_TYPE_COMPARISON_SE:
            result = cb_boolean_create(v1 == v2); break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_NE:
            result = cb_boolean_create(v1 != v2); break;

        /* invalid binary operator type */
        default: cb_abort("Invalid binary operator type"); break;
    }

    return result;
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

CbVariant* cb_float_perform_binary_operation(CbBinaryOperatorType operator_type,
                                             const CbVariant* left,
                                             const CbVariant* right)
{
    CbFloatDataType v1;
    CbFloatDataType v2;
    CbVariant* result = NULL;

    v1 = cb_numeric_as_float(left);
    v2 = cb_numeric_as_float(right);

    switch (operator_type)
    {
        case CB_BINARY_OPERATOR_TYPE_ADD:
            result = cb_float_create(v1 + v2); break;

        case CB_BINARY_OPERATOR_TYPE_SUB:
            result = cb_float_create(v1 - v2); break;

        case CB_BINARY_OPERATOR_TYPE_MUL:
            result = cb_float_create(v1 * v2); break;

        case CB_BINARY_OPERATOR_TYPE_DIV:
            if (v2 == 0.0)
                cb_abort("Division by zero is not allowed");
            else
                result = cb_float_create(v1 / v2);
            break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_GT:
            result = cb_boolean_create(v1 > v2); break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_GE:
            result = cb_boolean_create((v1 > v2) || dequal(v1, v2)); break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_LT:
            result = cb_boolean_create(v1 < v2); break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_LE:
            result = cb_boolean_create((v1 < v2) || dequal(v1, v2)); break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ:
        case CB_BINARY_OPERATOR_TYPE_COMPARISON_SE:
            result = cb_boolean_create(dequal(v1, v2)); break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_NE:
            result = cb_boolean_create(!dequal(v1, v2)); break;

        /* invalid binary operator type */
        default: cb_abort("Invalid binary operator type"); break;
    }

    return result;
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

CbVariant* cb_boolean_perform_binary_operation(CbBinaryOperatorType operator_type,
                                               const CbVariant* left,
                                               const CbVariant* right)
{
    CbBooleanDataType v1;
    CbBooleanDataType v2;
    CbVariant* result = NULL;

    v1 = cb_boolean_get_value(left);
    v2 = cb_boolean_get_value(right);

    switch (operator_type)
    {
        case CB_BINARY_OPERATOR_TYPE_LOGICAL_AND:
            result = cb_boolean_create(v1 && v2); break;

        case CB_BINARY_OPERATOR_TYPE_LOGICAL_OR:
            result = cb_boolean_create(v1 || v2); break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ:
        case CB_BINARY_OPERATOR_TYPE_COMPARISON_SE:
            result = cb_boolean_create(v1 == v2); break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_NE:
            result = cb_boolean_create(v1 != v2); break;

        /* invalid binary operator type */
        default: cb_abort("Invalid binary operator type"); break;
    }

    return result;
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

void cb_string_concat(CbVariant* self, const CbVariant* source)
{
    char* buffer;

    CbConstStringDataType v1 = cb_string_get_value(self);
    CbConstStringDataType v2 = cb_string_get_value(source);

    buffer = memalloc(strlen(v1) + strlen(v2) + 1);
    strcpy(buffer, v1);
    strcat(buffer, v2);

    memfree(self->v.string); /* free old string            */
    self->v.string = buffer; /* assign concatenated string */
}

CbBooleanDataType cb_string_equal(const CbVariant* lhs, const CbVariant* rhs)
{
    return strequ(cb_string_get_value(lhs), cb_string_get_value(rhs));
}

CbBooleanDataType cb_string_lhs_equal(const CbVariant* lhs, const CbVariant* rhs)
{
    CbConstStringDataType lhs_string = cb_string_get_value(lhs);
    CbConstStringDataType rhs_string = cb_string_get_value(rhs);
    size_t lhs_length                = strlen(lhs_string);
    size_t rhs_length                = strlen(rhs_string);

    if (lhs_length > rhs_length)
        return false;
    else
        return strnequ(lhs_string, rhs_string, lhs_length);
}

CbVariant* cb_string_perform_binary_operation(CbBinaryOperatorType operator_type,
                                              const CbVariant* left,
                                              const CbVariant* right)
{
    CbVariant* result = NULL;

    switch (operator_type)
    {
        case CB_BINARY_OPERATOR_TYPE_ADD:
            result = cb_variant_copy(left);
            cb_string_concat(result, right);
            break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ:
            result = cb_boolean_create(cb_string_lhs_equal(left, right)); break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_SE:
            result = cb_boolean_create(cb_string_equal(left, right)); break;

        case CB_BINARY_OPERATOR_TYPE_COMPARISON_NE:
            result = cb_boolean_create(!cb_string_equal(left, right)); break;

        /* invalid binary operator type */
        default: cb_abort("Invalid binary operator type"); break;
    }

    return result;
}
