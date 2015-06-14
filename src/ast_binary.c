#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"
#include "cb_utils.h"
#include "error_handling.h"
#include "ast_internal.h"
#include "ast_binary.h"


/* -------------------------------------------------------------------------- */

struct CbAstBinaryNode
{
    CbAstNode base;
    CbBinaryOperatorType operator_type;
};

static const char* CB_BINARY_OPERATOR_TYPE_STRINGS[] = {
    "+", /* CB_BINARY_OPERATOR_TYPE_ADD */
    "-", /* CB_BINARY_OPERATOR_TYPE_SUB */
    "*", /* CB_BINARY_OPERATOR_TYPE_MUL */
    "/"  /* CB_BINARY_OPERATOR_TYPE_DIV */
};

/*
 * Evaluate binary node (integer)
 */
static CbVariant* cb_ast_binary_node_eval_integer(const CbAstBinaryNode* self,
                                                  const CbVariant* left,
                                                  const CbVariant* right);

/*
 * Evaluate binary node (float)
 */
static CbVariant* cb_ast_binary_node_eval_float(const CbAstBinaryNode* self,
                                                const CbVariant* left,
                                                const CbVariant* right);


/* -------------------------------------------------------------------------- */

const char* cb_binary_operator_type_stringify(CbBinaryOperatorType type)
{
    /* TODO: make sure type arguemnt is valid */
    return CB_BINARY_OPERATOR_TYPE_STRINGS[type];
}

CbAstBinaryNode* cb_ast_binary_node_create(CbBinaryOperatorType operator_type,
                                           CbAstNode* left,
                                           CbAstNode* right)
{
    CbAstBinaryNode* self = (CbAstBinaryNode*) memalloc(sizeof(CbAstBinaryNode));
    cb_ast_node_init(
        &self->base, CB_AST_TYPE_BINARY, left, right,
        (CbAstNodeDestructorFunc) cb_ast_binary_node_destroy,
        (CbAstNodeEvalFunc)       cb_ast_binary_node_eval,
        (CbAstNodeSemanticFunc)   cb_ast_binary_node_check_semantic
    );
    self->operator_type = operator_type;
    
    return self;
}

void cb_ast_binary_node_destroy(CbAstBinaryNode* self)
{
    /* destroy both child nodes */
    cb_ast_node_destroy(self->base.left);
    cb_ast_node_destroy(self->base.right);
    
    memfree(self);
}

CbVariant* cb_ast_binary_node_eval(const CbAstBinaryNode* self)
{
    CbVariant* result = NULL;
    /* evaluate child nodes first */
    CbVariant* left  = cb_ast_node_eval(self->base.left);
    CbVariant* right = cb_ast_node_eval(self->base.right);
    
    /* make sure variant type of evaluated nodes is correct */
    cb_assert(cb_variant_is_numeric(left) && cb_variant_is_numeric(right));
    
    /* evaluate child nodes */
    if (cb_variant_is_float(left) || cb_variant_is_float(right))
        result = cb_ast_binary_node_eval_float(self, left, right);
    else
        result = cb_ast_binary_node_eval_integer(self, left, right);
    
    cb_variant_destroy(left);
    cb_variant_destroy(right);
    
    return result;
}

bool cb_ast_binary_node_check_semantic(const CbAstBinaryNode* self,
                                       CbSymbolTable* symbols)
{
    /* check semantic of child nodes first */
    bool result = cb_ast_node_check_semantic(self->base.left, symbols) &&
                  cb_ast_node_check_semantic(self->base.right, symbols);
    /* also, make sure the variant (value) type of the expression is correct */
    if (result)
    {
        CbVariantType expected_type = CB_VARIANT_TYPE_NUMERIC;
        if (! (cb_ast_node_check_expression_type(self->base.left,  expected_type) &&
               cb_ast_node_check_expression_type(self->base.right, expected_type)) )
        {
            /*
             * Raise an error, if not both child nodes are numeric expressions!
             */
            cb_error_trigger(
                CB_ERROR_SEMANTIC, self->base.line,
                "invalid operand type for binary '%s', expecting %s type",
                cb_binary_operator_type_stringify(self->operator_type),
                cb_variant_type_stringify(expected_type)
            );
            result = false;
        }
    }
    
    return result;
}


/* -------------------------------------------------------------------------- */

static CbVariant* cb_ast_binary_node_eval_integer(const CbAstBinaryNode* self,
                                                  const CbVariant* left,
                                                  const CbVariant* right)
{
    CbVariant* result    = NULL;
    CbIntegerDataType v1 = cb_integer_get_value(left);
    CbIntegerDataType v2 = cb_integer_get_value(right);
    
    switch (self->operator_type)
    {
        case CB_BINARY_OPERATOR_TYPE_ADD:
            result = cb_integer_create(v1 + v2); break;
        
        case CB_BINARY_OPERATOR_TYPE_SUB:
            result = cb_integer_create(v1 - v2); break;
        
        case CB_BINARY_OPERATOR_TYPE_MUL:
            result = cb_integer_create(v1 * v2); break;
        
        case CB_BINARY_OPERATOR_TYPE_DIV:
            /* division can result in a real number -> create float result */
            result = cb_float_create((CbFloatDataType) v1 /
                                     (CbFloatDataType) v2);
            break;
        
        /* invalid binary operator type */
        default: cb_abort("Invalid binary operator type"); break;
    }
    
    return result;
}

static CbVariant* cb_ast_binary_node_eval_float(const CbAstBinaryNode* self,
                                                const CbVariant* left,
                                                const CbVariant* right)
{
    CbVariant* result  = NULL;
    CbFloatDataType v1 = cb_float_get_value(left);
    CbFloatDataType v2 = cb_float_get_value(right);
    
    switch (self->operator_type)
    {
        case CB_BINARY_OPERATOR_TYPE_ADD:
            result = cb_float_create(v1 + v2); break;
        
        case CB_BINARY_OPERATOR_TYPE_SUB:
            result = cb_float_create(v1 - v2); break;
        
        case CB_BINARY_OPERATOR_TYPE_MUL:
            result = cb_float_create(v1 * v2); break;
        
        case CB_BINARY_OPERATOR_TYPE_DIV:
            result = cb_float_create(v1 / v2); break;
        
        /* invalid binary operator type */
        default: cb_abort("Invalid binary operator type"); break;
    }
    
    return result;
}