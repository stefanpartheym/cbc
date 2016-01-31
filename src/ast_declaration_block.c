 #include "utils.h"
#include "cb_utils.h"
#include "error_handling.h"
#include "vector.h"
#include "symbol_table.h"
#include "ast_internal.h"
#include "ast_declaration_block.h"


/* -------------------------------------------------------------------------- */

struct CbAstDeclarationBlockNode
{
    CbAstNode base;
    Vector* declarations;
};


/* -------------------------------------------------------------------------- */

CbAstDeclarationBlockNode* cb_ast_declaration_block_node_create()
{
    CbAstDeclarationBlockNode* self = memalloc(sizeof(CbAstDeclarationBlockNode));
    cb_ast_node_init(
        &self->base, CB_AST_TYPE_DECLARATION_BLOCK, NULL, NULL,
        (CbAstNodeDestructorFunc) cb_ast_declaration_block_node_destroy,
        (CbAstNodeEvalFunc)       cb_ast_declaration_block_node_eval,
        (CbAstNodeSemanticFunc)   cb_ast_declaration_block_node_check_semantic
    );

    self->declarations = vector_create();

    return self;
}

void cb_ast_declaration_block_node_destroy(CbAstDeclarationBlockNode* self)
{
    size_t i;
    size_t size = vector_get_count(self->declarations);
    for (i = 0; i < size; i++)
    {
        CbAstDeclarationNode* node = NULL;
        cb_assert(
            vector_get(self->declarations, i, (VectorItem*) &node)
        );
        cb_ast_declaration_node_destroy(node);
    }

    vector_destroy(self->declarations);
    memfree(self);
}

CbVariant* cb_ast_declaration_block_node_eval(const CbAstDeclarationBlockNode* self,
                                              const CbSymbolTable* symbols)
{
    /*
     * NOTE: The variables were already declared during the semantic check.
     *       -> No further action requiered.
     */

    return cb_variant_create();
}

bool cb_ast_declaration_block_node_check_semantic(const CbAstDeclarationBlockNode* self,
                                                  CbSymbolTable* symbols)
{
    size_t i;
    size_t size = vector_get_count(self->declarations);
    bool result = true;

    for (i = 0; result && i < size; i++)
    {
        CbAstDeclarationNode* node = NULL;
        cb_assert(
            vector_get(self->declarations, i, (VectorItem*) &node)
        );
        result = cb_ast_declaration_node_check_semantic(node, symbols);
    }

    return result;
}

void cb_ast_declaration_block_node_add(CbAstDeclarationBlockNode* self,
                                       CbAstDeclarationNode* node)
{
    vector_append(self->declarations, node);
}
