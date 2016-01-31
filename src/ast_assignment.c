#include "utils.h"
#include "cb_utils.h"
#include "error_handling.h"
#include "ast_internal.h"
#include "ast_variable.h"

#include "ast_assignment.h"


/* -------------------------------------------------------------------------- */

struct CbAstAssignmentNode
{
    CbAstNode base;
};


/* -------------------------------------------------------------------------- */

CbAstAssignmentNode* cb_ast_assignment_node_create(CbAstNode* left,
                                                   CbAstNode* right)
{
    CbAstAssignmentNode* self = (CbAstAssignmentNode*) memalloc(sizeof(CbAstAssignmentNode));
    cb_ast_node_init(
        &self->base, CB_AST_TYPE_ASSIGNMENT, left, right,
        (CbAstNodeDestructorFunc) cb_ast_assignment_node_destroy,
        (CbAstNodeEvalFunc)       cb_ast_assignment_node_eval,
        (CbAstNodeSemanticFunc)   cb_ast_assignment_node_check_semantic
    );

    return self;
}

void cb_ast_assignment_node_destroy(CbAstAssignmentNode* self)
{
    /* destroy both child nodes */
    cb_ast_node_destroy(self->base.left);
    cb_ast_node_destroy(self->base.right);

    memfree(self);
}

CbVariant* cb_ast_assignment_node_eval(const CbAstAssignmentNode* self,
                                       const CbSymbolTable* symbols)
{
    CbVariant* result = NULL;
    CbAstVariableNode* node;
    CbVariant* value;

    cb_assert(self->base.left->type == CB_AST_TYPE_VARIABLE);

    node  = (CbAstVariableNode*) self->base.left;
    value = cb_ast_node_eval(self->base.right, symbols);
    if (value != NULL)
    {
        result = cb_variant_copy(cb_ast_variable_node_assign(
            node, symbols, value
        ));
        cb_variant_destroy(value);
    }

    return result;
}

bool cb_ast_assignment_node_check_semantic(const CbAstAssignmentNode* self,
                                           CbSymbolTable* symbols)
{
    /* check semantic of child nodes first */
    bool result = cb_ast_node_check_semantic(self->base.left, symbols) &&
                  cb_ast_node_check_semantic(self->base.right, symbols);
    /* also, make sure the variant (value) type of the expression is correct */
    if (result)
    {
        /* lhs must be a variable */
        result = self->base.left->type == CB_AST_TYPE_VARIABLE;
        if (!result)
            cb_error_trigger(CB_ERROR_SEMANTIC, self->base.line,
                             "Values can only be assigned to variables");

        /*
         * TODO: In order to check semantics for the rhs properly, one needs
         *       (in addition to the AST node type) some kind of attribute,
         *       that specifies whether the AST node is an expression or
         *       something else.
         */
    }

    return result;
}
