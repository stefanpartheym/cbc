#include "utils.h"
#include "cb_utils.h"
#include "error_handling.h"
#include "symbol_variable.h"
#include "ast_variable.h"
#include "ast_internal.h"
#include "ast_binary.h"


/* -------------------------------------------------------------------------- */

struct CbAstBinaryNode
{
    CbAstNode base;
    CbBinaryOperatorType operator_type;
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

/*
 * Evaluate binary node (string)
 */
static CbVariant* cb_ast_binary_node_eval_string(const CbAstBinaryNode* self,
                                                 const CbVariant* left,
                                                 const CbVariant* right);

/*
 * Check if a binary operation is valid. Raise an error if not.
 */
static bool cb_ast_binary_node_check_operation(const CbAstBinaryNode* self,
                                               const CbVariantType lhs,
                                               const CbVariantType rhs);


/* -------------------------------------------------------------------------- */

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

CbVariant* cb_ast_binary_node_eval(const CbAstBinaryNode* self,
                                   const CbSymbolTable* symbols)
{
    CbVariant* result = NULL;
    
    /* evaluate left child node first */
    CbVariant* left = cb_ast_node_eval(self->base.left, symbols);
    if (left != NULL)
    {
        /* evaluate right child node first */
        CbVariant* right = cb_ast_node_eval(self->base.right, symbols);
        if (right != NULL)
        {
            if (cb_ast_binary_node_check_operation(self,
                                                   cb_variant_get_type(left),
                                                   cb_variant_get_type(right)))
            {
                if (cb_variant_is_numeric(left))
                {
                    if (cb_variant_is_float(left) || cb_variant_is_float(right))
                        result = cb_ast_binary_node_eval_float(self, left, right);
                    else
                        result = cb_ast_binary_node_eval_integer(self, left, right);
                }
                else if (cb_variant_is_string(left))
                    result = cb_ast_binary_node_eval_string(self, left, right);
                else
                    cb_abort("Invalid binary operation");
            }
            
            cb_variant_destroy(right);
        }
        
        cb_variant_destroy(left);
    }
    
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
        CbVariantType lhs = cb_ast_node_get_expression_type(self->base.left);
        CbVariantType rhs = cb_ast_node_get_expression_type(self->base.right);
        result            = cb_ast_binary_node_check_operation(self, lhs, rhs);
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
            if (v2 == 0)
                cb_error_trigger(CB_ERROR_RUNTIME, self->base.line,
                                 "Division by zero is not allowed");
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
        
        /* invalid binary operator type */
        default: cb_abort("Invalid binary operator type"); break;
    }
    
    return result;
}

static CbVariant* cb_ast_binary_node_eval_float(const CbAstBinaryNode* self,
                                                const CbVariant* left,
                                                const CbVariant* right)
{
    CbFloatDataType v1;
    CbFloatDataType v2;
    CbVariant* result = NULL;
    
    v1 = cb_numeric_as_float(left);
    v2 = cb_numeric_as_float(right);
    
    switch (self->operator_type)
    {
        case CB_BINARY_OPERATOR_TYPE_ADD:
            result = cb_float_create(v1 + v2); break;
        
        case CB_BINARY_OPERATOR_TYPE_SUB:
            result = cb_float_create(v1 - v2); break;
        
        case CB_BINARY_OPERATOR_TYPE_MUL:
            result = cb_float_create(v1 * v2); break;
        
        case CB_BINARY_OPERATOR_TYPE_DIV:
            if (v2 == 0.0)
                cb_error_trigger(CB_ERROR_RUNTIME, self->base.line,
                                 "Division by zero is not allowed");
            else
                result = cb_float_create(v1 / v2);
            break;
        
        /* invalid binary operator type */
        default: cb_abort("Invalid binary operator type"); break;
    }
    
    return result;
}

static CbVariant* cb_ast_binary_node_eval_string(const CbAstBinaryNode* self,
                                                 const CbVariant* left,
                                                 const CbVariant* right)
{
    CbVariant* result = NULL;
    
    cb_assert(self->operator_type == CB_BINARY_OPERATOR_TYPE_ADD);
    
    result = cb_variant_copy(left);
    cb_string_concat(result, right);
    
    return result;
}

static bool cb_ast_binary_node_check_operation(const CbAstBinaryNode* self,
                                               const CbVariantType lhs,
                                               const CbVariantType rhs)
{
    bool result = cb_variant_type_is_binary_operation_valid(self->operator_type,
                                                            lhs, rhs);
    if (!result)
    {
        cb_error_trigger(
            self->base.error_context, self->base.line,
            "Invalid binary operation: <%s> %s <%s>",
            cb_variant_type_stringify(lhs),
            cb_binary_operator_type_stringify(self->operator_type),
            cb_variant_type_stringify(rhs)
        );
    }
    
    return result;
}
