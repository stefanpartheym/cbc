#include <stdlib.h>

#include "utils.h"
#include "ast_internal.h"
#include "ast_value.h"


/* -------------------------------------------------------------------------- */

struct CbAstValueNode
{
    CbAstNode base;
    CbVariant* value;
};


/* -------------------------------------------------------------------------- */

CbAstValueNode* cb_ast_value_node_create(const CbVariant* value)
{
    CbAstValueNode* self = (CbAstValueNode*) memalloc(sizeof(CbAstValueNode));
    cb_ast_node_init(
        &self->base, CB_AST_TYPE_VALUE, NULL, NULL,
        (CbAstNodeDestructorFunc) cb_ast_value_node_destroy,
        (CbAstNodeEvalFunc)       cb_ast_value_node_eval,
        (CbAstNodeSemanticFunc)   cb_ast_value_node_check_semantic
    );
    
    self->value = cb_variant_copy(value);
    
    return self;
}

void cb_ast_value_node_destroy(CbAstValueNode* self)
{
    cb_variant_destroy(self->value);
    memfree(self);
}

const CbVariant* cb_ast_value_node_get_value(const CbAstValueNode* self)
{
    return self->value;
}

CbVariant* cb_ast_value_node_eval(const CbAstValueNode* self)
{
    return cb_variant_copy(self->value);
}

bool cb_ast_value_node_check_semantic(const CbAstValueNode* self,
                                      const void* dummy)
{
    /*
     * There are no child nodes available for this node
     * -> no further checking requiered
     */
    return true;
}
