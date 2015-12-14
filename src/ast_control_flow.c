#include "utils.h"
#include "cb_utils.h"
#include "error_handling.h"
#include "ast_internal.h"

#include "ast_control_flow.h"


/* -------------------------------------------------------------------------- */

struct CbAstControlFlowNode
{
    CbAstNode base;
    
    CbAstControlFlowNodeType flow_type;
    CbAstNode* condition;
};


/*
 * Constructor (Internal)
 */
CbAstControlFlowNode* cb_ast_control_flow_node_create(CbAstControlFlowNodeType type,
                                                      CbAstNodeDestructorFunc destructor_func,
                                                      CbAstNodeEvalFunc eval_func,
                                                      CbAstNodeSemanticFunc check_func,
                                                      CbAstNode* condition,
                                                      CbAstNode* left,
                                                      CbAstNode* right);


/* -------------------------------------------------------------------------- */

CbAstControlFlowNode* cb_ast_if_node_create(CbAstNode* condition,
                                            CbAstNode* true_branch,
                                            CbAstNode* false_branch)
{
    return cb_ast_control_flow_node_create(
        CB_AST_CONTROL_FLOW_TYPE_IF,
        (CbAstNodeDestructorFunc) cb_ast_if_node_destroy,
        (CbAstNodeEvalFunc) cb_ast_if_node_eval,
        (CbAstNodeSemanticFunc) cb_ast_if_node_check_semantic,
        condition, true_branch, false_branch
    );
}

void cb_ast_if_node_destroy(CbAstControlFlowNode* self)
{
    /* destroy condition node */
    cb_ast_node_destroy(self->condition);
    
    /*
     * Destroy both child nodes
     * NOTE: Child nodes can be NULL. For example consider the following if:
     *    if True then
     *       // do stuff
     *    endif,
     * The statement does not contain an else-branch, which is - in this case -
     * NULL.
     */
    if (self->base.left  != NULL) cb_ast_node_destroy(self->base.left);
    if (self->base.right != NULL) cb_ast_node_destroy(self->base.right);
    
    memfree(self);
}

CbVariant* cb_ast_if_node_eval(const CbAstControlFlowNode* self,
                               const CbSymbolTable* symbols)
{
    CbVariant* result   = NULL;
    CbVariant* decision = cb_ast_node_eval(self->condition, symbols);
    
    if (cb_boolean_get_value(decision))
        result = cb_ast_node_safe_eval(self->base.left, symbols);
    else
        result = cb_ast_node_safe_eval(self->base.right, symbols);
    
    cb_variant_destroy(decision);
    
    return result;
}

bool cb_ast_if_node_check_semantic(const CbAstControlFlowNode* self,
                                   CbSymbolTable* symbols)
{
    CbVariantType condition_type = cb_ast_node_get_expression_type(self->condition);
    bool result                  = condition_type == CB_VARIANT_TYPE_BOOLEAN ||
                                   condition_type == CB_VARIANT_TYPE_UNDEFINED;
    
    if (result)
    {
        result = cb_ast_node_check_semantic(self->condition, symbols)      &&
                 cb_ast_node_safe_check_semantic(self->base.left, symbols) &&
                 cb_ast_node_safe_check_semantic(self->base.right, symbols);
    }
    else
    {
        cb_error_trigger(
            self->base.error_context, self->base.line,
            "Condition is not a boolean expression"
        );
    }
    
    return result;
}


/* -------------------------------------------------------------------------- */

CbAstControlFlowNode* cb_ast_while_node_create(CbAstNode* condition,
                                               CbAstNode* body)
{
    
    return cb_ast_control_flow_node_create(
        CB_AST_CONTROL_FLOW_TYPE_WHILE,
        (CbAstNodeDestructorFunc) cb_ast_while_node_destroy,
        (CbAstNodeEvalFunc) cb_ast_while_node_eval,
        (CbAstNodeSemanticFunc) cb_ast_while_node_check_semantic,
        condition, body, NULL
    );
}

void cb_ast_while_node_destroy(CbAstControlFlowNode* self)
{
    /* destroy condition node */
    cb_ast_node_destroy(self->condition);
    
    /* destroy left child node */
    cb_ast_node_destroy(self->base.left);
    
    memfree(self);
}

CbVariant* cb_ast_while_node_eval(const CbAstControlFlowNode* self,
                                  const CbSymbolTable* symbols)
{
    CbVariant* result   = NULL;
    bool execute_body   = true;
    
    while (execute_body)
    {
        CbVariant* condition = cb_ast_node_eval(self->condition, symbols);
        execute_body         = cb_boolean_get_value(condition);
        
        cb_variant_destroy(condition);
        if (result != NULL)
            cb_variant_destroy(result);
        
        if (execute_body)
            result = cb_ast_node_eval(self->base.left, symbols);
        else
            result = cb_variant_create();
    }
    
    return result;
}

bool cb_ast_while_node_check_semantic(const CbAstControlFlowNode* self,
                                      CbSymbolTable* symbols)
{
    CbVariantType condition_type = cb_ast_node_get_expression_type(self->condition);
    bool result                  = condition_type == CB_VARIANT_TYPE_BOOLEAN ||
                                   condition_type == CB_VARIANT_TYPE_UNDEFINED;
    
    if (result)
    {
        result = cb_ast_node_check_semantic(self->condition, symbols) &&
                 cb_ast_node_check_semantic(self->base.left, symbols);
    }
    else
    {
        cb_error_trigger(
            self->base.error_context, self->base.line,
            "Condition is not a boolean expression"
        );
    }
    
    return result;
}


/* -------------------------------------------------------------------------- */

CbAstControlFlowNode* cb_ast_control_flow_node_create(CbAstControlFlowNodeType type,
                                                      CbAstNodeDestructorFunc destructor_func,
                                                      CbAstNodeEvalFunc eval_func,
                                                      CbAstNodeSemanticFunc semantic_func,
                                                      CbAstNode* condition,
                                                      CbAstNode* left,
                                                      CbAstNode* right)
{
    CbAstControlFlowNode* self = (CbAstControlFlowNode*) memalloc(sizeof(CbAstControlFlowNode));
    cb_ast_node_init(
        &self->base, CB_AST_TYPE_CONTROL_FLOW, left, right, destructor_func,
        eval_func, semantic_func
    );
    
    self->flow_type = type;
    self->condition = condition;
    
    return self;
}
