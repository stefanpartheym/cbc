/*******************************************************************************
 * @file  symbol_table.h
 * @brief Contains the CbSymbolTable structure
 * 
 * The symbol table is used during the semantic analysis of Codeblock source
 * code to validate the usage of identifiers.
 ******************************************************************************/

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

#include "scope.h"
#include "symbol.h"


/**
 * @struct CbSymbolTable
 * @brief  Table data structure to store identifier information
 * 
 * This symbol table structure is used to store information about identifiers
 * that occur in Codeblock source code.
 */
typedef struct CbSymbolTable CbSymbolTable;

/**
 * @memberof CbSymbolTable
 * @brief    Constructor
 */
CbSymbolTable* cb_symbol_table_create();

/**
 * @memberof CbSymbolTable
 * @brief    Destructor
 * 
 * @param self The CbSymbolTable instance
 */
void cb_symbol_table_destroy(CbSymbolTable* self);

/**
 * @memberof CbSymbolTable
 * @brief    Insert a new symbol instance
 * 
 * @param self   The CbSymbolTable instance
 * @param symbol The symbol that is inserted into the table
 *               (NOTE: The table will free allocated memory for symbol)
 * 
 * @return Returns NULL if the symbol was inserted successfully.
 *         If a symbol with the same identifier already exists in the current
 *         scope, a pointer to that symbol is returned.
 */
const CbSymbol* cb_symbol_table_insert(const CbSymbolTable* self,
                                       CbSymbol* symbol);

/**
 * @memberof CbSymbolTable
 * @brief    Lookup a symbol by its identifier
 * 
 * @param self       The CbSymbolTable instance
 * @param identifier The identifier of the symbol
 * 
 * @return Returns NULL if there is no symbol declared with the given identifier
 */
CbSymbol* cb_symbol_table_lookup(const CbSymbolTable* self,
                                 const char* identifier);

/**
 * @memberof CbSymbolTable
 * @brief    Enter a new scope
 * 
 * @param self The CbSymbolTable instance
 */
void cb_symbol_table_enter_scope(CbSymbolTable* self);

/**
 * @memberof CbSymbolTable
 * @brief    Switch to a new scope (e.g. when calling a function)
 * 
 * @param self   The CbSymbolTable instance
 * @param parent The parent scope of the new scope
 *               (pass NULL for 'global scope')
 */
void cb_symbol_table_switch_scope(CbSymbolTable* self, const CbScope* parent);

/**
 * @memberof CbSymbolTable
 * @brief    Leave current scope
 * 
 * @param self The CbSymbolTable instance
 */
void cb_symbol_table_leave_scope(CbSymbolTable* self);


#endif /* SYMBOL_TABLE_H */
