/*******************************************************************************
 * Contains the CbSymbolVariable structure.
 * 
 * CbSymbolVariable is a representation of a variable in Codeblock source code
 * including its data type and current value information.
 ******************************************************************************/

#ifndef SYMBOL_VARIABLE_H
#define SYMBOL_VARIABLE_H

#include "symbol.h"


/* -------------------------------------------------------------------------- */

typedef struct CbSymbolVariable CbSymbolVariable;


/* -------------------------------------------------------------------------- */


/*
 * Create a CbSymbolVariable object.
 */
CbSymbolVariable* cb_symbol_variable_create(const char* identifier);

/*
 * Destroy a CbSymbolVariable object.
 */
void cb_symbol_variable_destroy(CbSymbolVariable* self);

/*
 * Assign a value to a variable.
 */
void cb_symbol_variable_assign(CbSymbolVariable* self, const CbVariant* value);

/*
 * Get the current value.
 * TODO: Implement!
 */
/* const CbVariant* cb_symbol_variable_get_value(const CbSymbolVariable* self); */


#endif /* SYMBOL_VARIABLE_H */
