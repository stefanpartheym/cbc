#include <stdlib.h>

#include "utils.h"
#include "ast_internal.h"
#include "ast_debug_print.h"


/* -------------------------------------------------------------------------- */

struct CbAstDebugPrintNode
{
    CbAstNode base;
};


/* -------------------------------------------------------------------------- */

CbAstDebugPrintNode* cb_ast_debug_print_node_create(CbAstNode* expression)
{
    CbAstDebugPrintNode* self = (CbAstDebugPrintNode*) memalloc(sizeof(CbAstDebugPrintNode));
    cb_ast_node_init(
        &self->base, CB_AST_TYPE_DEBUG_PRINT, expression, NULL,
        (CbAstNodeDestructorFunc) cb_ast_debug_print_node_destroy,
        (CbAstNodeEvalFunc)       cb_ast_debug_print_node_eval,
        (CbAstNodeSemanticFunc)   cb_ast_debug_print_node_check_semantic
    );

    return self;
}

void cb_ast_debug_print_node_destroy(CbAstDebugPrintNode* self)
{
    memfree(self);
}

CbVariant* cb_ast_debug_print_node_eval(const CbAstDebugPrintNode* self,
                                        const CbSymbolTable* symbols)
{
    CbVariant* result = cb_ast_node_eval(self->base.left, symbols);
    if (result != NULL)
    {
        cb_variant_print(result);
        printf("\n");
        cb_variant_destroy(result);
        result = cb_variant_create();
    }

    return result;
}

bool cb_ast_debug_print_node_check_semantic(const CbAstDebugPrintNode* self,
                                            CbSymbolTable* symbols)
{
    return cb_ast_node_check_semantic(self->base.left, symbols);
}
