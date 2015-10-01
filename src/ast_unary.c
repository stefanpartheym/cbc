#include "utils.h"
#include "cb_utils.h"
#include "error_handling.h"
#include "ast_internal.h"
#include "ast_unary.h"


/* -------------------------------------------------------------------------- */

struct CbAstUnaryNode
{
    CbAstNode base;
    CbUnaryOperatorType operator_type;
};

/*
 * Check if a unary operation is valid. Raise an error if not.
 */
static bool cb_ast_unary_node_check_operation(const CbAstUnaryNode* self,
                                              const CbVariantType type);


/* -------------------------------------------------------------------------- */

CbAstUnaryNode* cb_ast_unary_node_create(const CbUnaryOperatorType operator_type,
                                         CbAstNode* operand)
{
    CbAstUnaryNode* self = (CbAstUnaryNode*) memalloc(sizeof(CbAstUnaryNode));
    cb_ast_node_init(
        &self->base, CB_AST_TYPE_UNARY, operand, NULL,
        (CbAstNodeDestructorFunc) cb_ast_unary_node_destroy,
        (CbAstNodeEvalFunc)       cb_ast_unary_node_eval,
        (CbAstNodeSemanticFunc)   cb_ast_unary_node_check_semantic
    );
    self->operator_type = operator_type;
    
    return self;
}

void cb_ast_unary_node_destroy(CbAstUnaryNode* self)
{
    /* destroy left child node */
    cb_ast_node_destroy(self->base.left);
    
    memfree(self);
}

CbVariant* cb_ast_unary_node_eval(const CbAstUnaryNode* self,
                                  const CbSymbolTable* symbols)
{
    CbVariant* result = NULL;
    CbVariant* value  = cb_ast_node_eval(self->base.left, symbols);
    
    if (value != NULL)
    {
        if (cb_ast_unary_node_check_operation(self, cb_variant_get_type(value)))
        {
            switch (self->operator_type)
            {
                case CB_UNARY_OPERATOR_TYPE_MINUS:
                {
                    if (cb_variant_is_integer(value))
                        result = cb_integer_create( - cb_integer_get_value(value));
                    else if (cb_variant_is_float(value))
                        result = cb_float_create( - cb_float_get_value(value));
                    else
                        /* TODO: handle all compatible AST node types */
                        cb_abort("Wrong variant type");
                    
                    break;
                }
                
                case CB_UNARY_OPERATOR_TYPE_LOGICAL_NOT:
                    cb_assert(cb_variant_is_boolean(value));
                    result = cb_boolean_create(!cb_boolean_get_value(value));
                    break;
                
                /* invalid unary operator type */
                default:
                    cb_abort("Invalid unary operator type"); break;
            }
        }
        
        cb_variant_destroy(value);
    }
    
    return result;
}

bool cb_ast_unary_node_check_semantic(const CbAstUnaryNode* self,
                                      CbSymbolTable* symbols)
{
    bool result = cb_ast_node_check_semantic(self->base.left, symbols);
    if (result)
    {
        CbVariantType type = cb_ast_node_get_expression_type((CbAstNode*) self);
        result             = cb_ast_unary_node_check_operation(self, type);
    }
    
    return result;
}

static bool cb_ast_unary_node_check_operation(const CbAstUnaryNode* self,
                                              const CbVariantType type)
{
    bool result = cb_variant_type_is_unary_operation_valid(self->operator_type,
                                                           type);
    if (!result)
    {
        cb_error_trigger(
            self->base.error_context, self->base.line,
            "Invalid unary operation: %s <%s>",
            cb_unary_operator_type_stringify(self->operator_type),
            cb_variant_type_stringify(type)
        );
    }
    
    return result;
}
