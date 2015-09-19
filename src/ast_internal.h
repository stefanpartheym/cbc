/*******************************************************************************
 * Internal declaration of CbAstNode:
 * Used to access CbAstNode-properties in any derived AST node type
 * 
 * NOTE: Do not include/use this header anywhere else than in AST node
 *       implementation files!
 ******************************************************************************/

#ifndef AST_INTERNAL_H
#define AST_INTERNAL_H

#include "error_handling.h"
#include "ast.h"


/* -------------------------------------------------------------------------- */

typedef void       (*CbAstNodeDestructorFunc) (CbAstNode*);
typedef CbVariant* (*CbAstNodeEvalFunc)       (const CbAstNode*,
                                               const CbSymbolTable*);
typedef bool       (*CbAstNodeSemanticFunc)   (const CbAstNode*,
                                               CbSymbolTable*);

struct CbAstNode
{
    CbAstType type;
    int line;
    CbErrorType error_context;
    
    struct CbAstNode* left;
    struct CbAstNode* right;
    
    CbAstNodeDestructorFunc destructor;
    CbAstNodeEvalFunc eval;
    CbAstNodeSemanticFunc semantic_check;
};


/* -------------------------------------------------------------------------- */

/*
 * Initialize a CbAstNode object
 */
void cb_ast_node_init(CbAstNode* self,
                      CbAstType type,
                      CbAstNode* left_node,
                      CbAstNode* right_node,
                      CbAstNodeDestructorFunc destructor,
                      CbAstNodeEvalFunc eval,
                      CbAstNodeSemanticFunc semantic_check);


#endif /* AST_INTERNAL_H */
