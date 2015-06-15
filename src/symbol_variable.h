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


#endif /* SYMBOL_VARIABLE_H */
