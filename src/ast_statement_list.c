#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"
#include "cb_utils.h"
#include "error_handling.h"

#include "ast_internal.h"
#include "ast_statement_list.h"


/* -------------------------------------------------------------------------- */

CbAstNode* cb_ast_statement_list_node_create(CbAstNode* left, CbAstNode* right)
{
    CbAstNode* self = (CbAstNode*) memalloc(sizeof(CbAstNode));
    cb_ast_node_init(
        self, CB_AST_TYPE_STATEMENT_LIST, left, right,
        (CbAstNodeDestructorFunc) cb_ast_statement_list_node_destroy,
        (CbAstNodeEvalFunc)       cb_ast_statement_list_node_eval,
        (CbAstNodeSemanticFunc)   cb_ast_statement_list_node_check_semantic
    );
    
    return self;
}

void cb_ast_statement_list_node_destroy(CbAstNode* self)
{
    /* destroy child nodes */
    cb_ast_node_destroy(self->left);
    cb_ast_node_destroy(self->right);
    
    memfree(self);
}

CbVariant* cb_ast_statement_list_node_eval(const CbAstNode* self,
                                           const CbSymbolTable* symbols)
{
    CbVariant* result = cb_ast_node_eval(self->left, symbols);
    if (!cb_error_occurred())
    {
        cb_variant_destroy(result);
        result = cb_ast_node_eval(self->right, symbols);
    }
    
    return result;
}

bool cb_ast_statement_list_node_check_semantic(const CbAstNode* self,
                                               CbSymbolTable* symbols)
{
    bool result = cb_ast_node_check_semantic(self->left, symbols);
    if (result)
        result = cb_ast_node_check_semantic(self->right, symbols);
    
    return result;
}
