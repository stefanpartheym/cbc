/*******************************************************************************
 * Abstract syntax tree node: ControlFlow
 * Representation of an control flow statement, such as:
 *    - if
 *    - case
 *    - while
 *    - for
 *
 * Inherites from CbAstNode
 ******************************************************************************/

#ifndef AST_CONTROL_FLOW_H
#define AST_CONTROL_FLOW_H

#include "ast.h"
#include "vector.h"


/* -------------------------------------------------------------------------- */

typedef struct CbAstControlFlowNode CbAstControlFlowNode;
typedef struct CbAstSwitchCaseNode CbAstSwitchCaseNode;
typedef struct CbAstCaseNode CbAstCaseNode;

enum CbAstControlFlowNodeType
{
    CB_AST_CONTROL_FLOW_TYPE_IF,
    CB_AST_CONTROL_FLOW_TYPE_WHILE,
    CB_AST_CONTROL_FLOW_TYPE_FOR,
    CB_AST_CONTROL_FLOW_TYPE_SWITCH
};

typedef enum CbAstControlFlowNodeType CbAstControlFlowNodeType;


/* -------------------------------------------------------------------------- */

/*
 * Constructor for an if-statement
 */
CbAstControlFlowNode* cb_ast_if_node_create(CbAstNode* condition,
                                            CbAstNode* true_branch,
                                            CbAstNode* false_branch);

/*
 * Destructor for an if-statement
 */
void cb_ast_if_node_destroy(CbAstControlFlowNode* self);

/*
 * Evaluate an if-statement
 */
CbVariant* cb_ast_if_node_eval(const CbAstControlFlowNode* self,
                               const CbSymbolTable* symbols);

/*
 * Check semantics for an if-statement
 */
bool cb_ast_if_node_check_semantic(const CbAstControlFlowNode* self,
                                   CbSymbolTable* symbols);


/* -------------------------------------------------------------------------- */

/*
 * Constructor for a while-statement
 */
CbAstControlFlowNode* cb_ast_while_node_create(CbAstNode* condition,
                                               CbAstNode* body);

/*
 * Destructor for a while-statement
 */
void cb_ast_while_node_destroy(CbAstControlFlowNode* self);

/*
 * Evaluate a while-statement
 */
CbVariant* cb_ast_while_node_eval(const CbAstControlFlowNode* self,
                                  const CbSymbolTable* symbols);

/*
 * Check semantics for a while-statement
 */
bool cb_ast_while_node_check_semantic(const CbAstControlFlowNode* self,
                                      CbSymbolTable* symbols);


/* -------------------------------------------------------------------------- */

/*
 * Constructor for a for-statement
 */
CbAstControlFlowNode* cb_ast_for_node_create(CbAstNode* initialisation,
                                             CbAstNode* final_value,
                                             CbAstNode* body);

/*
 * Destructor for a for-statement
 */
void cb_ast_for_node_destroy(CbAstControlFlowNode* self);

/*
 * Evaluate a for-statement
 */
CbVariant* cb_ast_for_node_eval(const CbAstControlFlowNode* self,
                                const CbSymbolTable* symbols);

/*
 * Check semantics for a for-statement
 */
bool cb_ast_for_node_check_semantic(const CbAstControlFlowNode* self,
                                    CbSymbolTable* symbols);


/* -------------------------------------------------------------------------- */

/*
 * Constructor for a switch-statement
 */
CbAstSwitchCaseNode* cb_ast_switch_case_node_create(CbAstNode* switch_node,
                                                    Vector* cases);

/*
 * Destructor for a switch-statement
 */
void cb_ast_switch_case_node_destroy(CbAstSwitchCaseNode* self);

/*
 * Evaluate a switch-statement
 */
CbVariant* cb_ast_switch_case_node_eval(const CbAstSwitchCaseNode* self,
                                        const CbSymbolTable* symbols);

/*
 * Check semantics for a switch-statement
 */
bool cb_ast_switch_case_node_check_semantic(const CbAstSwitchCaseNode* self,
                                            CbSymbolTable* symbols);


/* -------------------------------------------------------------------------- */

/*
 * Constructor for the case node
 */
CbAstCaseNode* cb_ast_case_node_create(CbAstNode* condition, CbAstNode* body);

/*
 * Destructor for the case node
 */
void cb_ast_case_node_destroy(CbAstCaseNode* self);

/*
 * Evaluate a case node
 */
CbVariant* cb_ast_case_node_eval(const CbAstCaseNode* self,
                                 const CbSymbolTable* symbols);

/*
 * Check semantics for a case node
 */
bool cb_ast_case_node_check_semantic(const CbAstCaseNode* self,
                                     CbSymbolTable* symbols);


#endif /* AST_CONTROL_FLOW_H */
