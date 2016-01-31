/*******************************************************************************
 * Contains the CbSymbolFunction structure.
 *
 * CbSymbolFunction is a representation of a function in Codeblock source code
 * including its return data type and the abstract syntax tree (AST) that forms
 * the body of the function.
 ******************************************************************************/

#ifndef SYMBOL_FUNCTION_H
#define SYMBOL_FUNCTION_H

#include "symbol.h"


/* -------------------------------------------------------------------------- */

typedef struct CbSymbolFunction CbSymbolFunction;


/* -------------------------------------------------------------------------- */


/*
 * Create a CbSymbolFunction object.
 */
CbSymbolFunction* cb_symbol_function_create(const char* identifier);

/*
 * Destroy a CbSymbolFunction object.
 */
void cb_symbol_function_destroy(CbSymbolFunction* self);


#endif /* SYMBOL_FUNCTION_H */
