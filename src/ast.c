#include <stdlib.h>
#include <stdbool.h>

#include "cb_utils.h"
#include "ast.h"
#include "ast_internal.h"
#include "ast_value.h"
#include "ast_binary.h"
#include "ast_unary.h"
#include "ast_variable.h"


/* -------------------------------------------------------------------------- */

void cb_ast_node_init(CbAstNode* self,
                      CbAstType type,
                      CbAstNode* left_node,
                      CbAstNode* right_node,
                      CbAstNodeDestructorFunc destructor,
                      CbAstNodeEvalFunc eval,
                      CbAstNodeSemanticFunc semantic_check)
{
    self->type          = type;
    self->left          = left_node;
    self->right         = right_node;
    self->line          = -1;
    self->error_context = CB_ERROR_UNKNOWN;
    
    self->destructor     = destructor;
    self->eval           = eval;
    self->semantic_check = semantic_check;
}

void cb_ast_node_destroy(CbAstNode* self)
{
    self->destructor(self);
}

void cb_ast_node_set_line(CbAstNode* self, int line)
{
    self->line = line;
}

CbAstType cb_ast_node_get_type(const CbAstNode* self)
{
    return self->type;
}

CbVariant* cb_ast_node_eval(CbAstNode* self, const CbSymbolTable* symbols)
{
    self->error_context = CB_ERROR_RUNTIME;
    return self->eval(self, symbols);
}

bool cb_ast_node_check_semantic(CbAstNode* self, CbSymbolTable* symbols)
{
    self->error_context = CB_ERROR_SEMANTIC;
    return self->semantic_check(self, symbols);
}

bool cb_ast_node_check_expression_type(const CbAstNode* self,
                                       CbVariantType variant_type)
{
    bool result = false;
    
    switch (self->type)
    {
        case CB_AST_TYPE_VALUE:
            result = cb_ast_value_node_check_expression_type(
                (CbAstValueNode*) self, variant_type
            );
            break;
        
        case CB_AST_TYPE_BINARY:
            result =
                cb_ast_node_check_expression_type(self->left,  variant_type) &&
                cb_ast_node_check_expression_type(self->right, variant_type);
            break;
        
        case CB_AST_TYPE_UNARY:
            result =
                cb_ast_node_check_expression_type(self->left, variant_type);
            break;
        
        case CB_AST_TYPE_VARIABLE:
            result = cb_ast_variable_node_check_expression_type(
                (CbAstVariableNode*) self, variant_type
            );
            break;
        
        /* invalid AST node types */
        case CB_AST_TYPE_NONE:
        default: cb_abort("Invalid AST node type"); break;
    }
    
    return result;
}

CbVariantType cb_ast_node_get_expression_type(const CbAstNode* self)
{
    const CbVariant* value;
    CbVariantType result = CB_VARIANT_TYPE_UNDEFINED;
    
    switch (self->type)
    {
        
        case CB_AST_TYPE_VALUE:
            value  = cb_ast_value_node_get_value((CbAstValueNode*) self);
            result = cb_variant_get_type(value);
            break;
        
        case CB_AST_TYPE_BINARY:
            result = cb_ast_node_get_expression_type(self->left) &&
                     cb_ast_node_get_expression_type(self->right);
            break;
        
        case CB_AST_TYPE_UNARY:
            result = cb_ast_node_get_expression_type(self->left);
            break;
        
        case CB_AST_TYPE_ASSIGNMENT:
            result = cb_ast_node_get_expression_type(self->right);
            break;
        
        /*
         * For variable AST nodes there is nothing to do, since at this point
         * (semantic checking) there is no value available yet.
         */
        case CB_AST_TYPE_VARIABLE: break;
        
        /* invalid AST node types */
        case CB_AST_TYPE_NONE:
        default: cb_abort("Invalid AST node type"); break;
    }
    
    return result;
}
