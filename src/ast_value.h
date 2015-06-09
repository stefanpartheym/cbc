/*******************************************************************************
 * Abstract syntax tree node: Value
 * Representation of a constant value of any type
 * 
 * Inherites from CbAstNode
 ******************************************************************************/

#ifndef AST_VALUE_H
#define AST_VALUE_H

#include "variant.h"


/* -------------------------------------------------------------------------- */

typedef struct CbAstValueNode CbAstValueNode;


/* -------------------------------------------------------------------------- */

/*
 * Constructor
 */
CbAstValueNode* cb_ast_value_node_create(const CbVariant* value);

/*
 * Destructor
 */
void cb_ast_value_node_destroy(CbAstValueNode* self);

/*
 * Value (Getter)
 */
const CbVariant* cb_ast_value_node_get_value(const CbAstValueNode* self);

/*
 * Evaluate value node
 */
CbVariant* cb_ast_value_node_eval(const CbAstValueNode* self);

/*
 * Check semantics
 */
bool cb_ast_value_node_check_semantic(const CbAstValueNode* self,
                                      const void* dummy);

/*
 * Check if Value node expression has a specific type
 */
bool cb_ast_value_node_check_expression_type(const CbAstValueNode* self,
                                             CbVariantType variant_type);


#endif /* AST_VALUE_H */
