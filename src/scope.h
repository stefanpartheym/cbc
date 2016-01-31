/*******************************************************************************
 * @file  scope.h
 * @brief Contains the CbScope structure
 *
 * Representation of a scope in Codeblock source code.
 ******************************************************************************/

#ifndef SCOPE_H
#define SCOPE_H

#include <stdbool.h>

#include "hash_table.h"


/**
 * @struct CbScope
 * @brief Representation of a scope in Codeblock source code.
 */
typedef struct CbScope CbScope;

/**
 * @memberof CbScope
 * @brief Constructor
 *
 * @param parent The parent scope
 *               If parent is NULL, the created scope is treated as gloabal
 *               scope.
 */
CbScope* cb_scope_create(const CbScope* parent);

/**
 * @memberof CbScope
 * @brief Destructor
 *
 * @param self The scope instance
 */
void cb_scope_destroy(CbScope* self);

/**
 * @memberof CbScope
 * @brief Get parent scope
 *
 * @param self The scope instance
 */
const CbScope* cb_scope_get_parent(const CbScope* self);

/**
 * @memberof CbScope
 * @brief Get hash table with symbols
 *
 * @param self The scope instance
 */
CbHashTable* cb_scope_get_symbols(const CbScope* self);


#endif /* SCOPE_H */
