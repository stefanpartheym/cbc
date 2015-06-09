#include <stdlib.h>
#include <stdbool.h>

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

static const char* const CB_UNARY_OPERATOR_TYPE_STRINGS[] = {
    "-" /* CB_UNARY_OPERATOR_TYPE_MINUS */
};


/* -------------------------------------------------------------------------- */

const char* cb_unary_operator_type_stringify(CbUnaryOperatorType type)
{
    /* TODO: make sure type arguemnt is valid */
    return CB_UNARY_OPERATOR_TYPE_STRINGS[type];
}

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

CbVariant* cb_ast_unary_node_eval(const CbAstUnaryNode* self)
{
    CbVariant* result = NULL;
    CbVariant* value  = cb_ast_node_eval(self->base.left);
    
    /* make sure variant type of evaluated node is correct */
    cb_assert(cb_variant_is_numeric(value));
    
    switch (self->operator_type)
    {
        case CB_UNARY_OPERATOR_TYPE_MINUS:
        {
            if (cb_variant_is_numeric(value))
                result = cb_integer_create( - cb_integer_get_value(value));
            else
                /* TODO: handle all compatible AST node types */
                cb_abort("Wrong variant type");
            
            break;
        }
        
        /* invalid unary operator type */
        default:
            cb_abort("Invalid unary operator type"); break;
    }
    
    cb_variant_destroy(value);
    
    return result;
}

bool cb_ast_unary_node_check_semantic(const CbAstUnaryNode* self,
                                      CbSymbolTable* symbols)
{
    bool result = cb_ast_node_check_semantic(self->base.left, symbols);
    if (result)
        if (!cb_ast_node_check_expression_type(self->base.left,
                                               CB_VARIANT_TYPE_NUMERIC))
        {
            cb_error_trigger(
                CB_ERROR_SEMANTIC, self->base.line,
                "invalid operand type for unary '%s', expecting %s type",
                cb_unary_operator_type_stringify(self->operator_type),
                cb_variant_type_stringify(CB_VARIANT_TYPE_NUMERIC)
            );
            result = false;
        }
    
    return result;
}
