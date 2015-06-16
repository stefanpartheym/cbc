#include <stdlib.h>
#include <stdbool.h>

#include "cb_utils.h"
#include "ast.h"
#include "ast_internal.h"
#include "ast_value.h"
#include "ast_binary.h"
#include "ast_unary.h"


/* -------------------------------------------------------------------------- */

void cb_ast_node_init(CbAstNode* self,
                      CbAstType type,
                      CbAstNode* left_node,
                      CbAstNode* right_node,
                      CbAstNodeDestructorFunc destructor,
                      CbAstNodeEvalFunc eval,
                      CbAstNodeSemanticFunc semantic_check)
{
    self->type  = type;
    self->left  = left_node;
    self->right = right_node;
    self->line  = -1;
    
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

CbVariant* cb_ast_node_eval(const CbAstNode* self,
                            const CbSymbolTable* symbols)
{
    return self->eval(self, symbols);
}

bool cb_ast_node_check_semantic(const CbAstNode* self,
                                CbSymbolTable* symbols)
{
    return self->semantic_check(self, symbols);
}

bool cb_ast_node_check_expression_type(const CbAstNode* self,
                                       CbVariantType variant_type)
{
    bool result = false;
    
    switch (self->type)
    {
        case CB_AST_TYPE_VALUE:
            result =
                cb_ast_value_node_check_expression_type((CbAstValueNode*) self,
                                                        variant_type);
            break;
        
        case CB_AST_TYPE_BINARY:
            result =
                cb_ast_node_check_expression_type(self->left, variant_type) &&
                cb_ast_node_check_expression_type(self->right, variant_type);
            break;
        
        case CB_AST_TYPE_UNARY:
            result =
                cb_ast_node_check_expression_type(self->left, variant_type);
            break;
        
        /* invalid AST node types */
        case CB_AST_TYPE_NONE:
        default:
            cb_abort("Invalid AST node type"); break;
    }
    
    return result;
}
