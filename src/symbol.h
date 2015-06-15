/*******************************************************************************
 * This file contains the CbSymbol structure.
 * The CbSymbol strcuture is a representation of any identifier in Codeblock
 * source code with its data type information.
 * 
 * NOTE: CbSymbol is used as an interface and should only be instantiated by
 *       derived structures like CbSymbolVariable or CbSymbolFunction.
 ******************************************************************************/

#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>

#include "variant.h"


/**
 * @struct CbSymbol
 * @brief  Representation of an identifier in Codeblock source code with its
 *         data type information.
 */
typedef struct CbSymbol CbSymbol;

typedef enum
{
    CB_SYMBOL_TYPE_VARIABLE,
    CB_SYMBOL_TYPE_FUNCTION
} CbSymbolType;


/**
 * @memberof CbSymbol
 * @brief    Get string representation of a CbSymbol-type
 */
const char* cb_symbol_type_stringify(CbSymbolType type);


/**
 * @memberof CbSymbol
 * @brief    (virtual) Destructor
 * 
 * @param self The CbSymbol instance
 */
void cb_symbol_destroy(CbSymbol* self);

/**
 * @memberof CbSymbol
 * @brief    Get the symbol identifier
 * 
 * @param self The CbSymbol instance
 */
const char* cb_symbol_get_identifier(const CbSymbol* self);

/**
 * @memberof CbSymbol
 * @brief    Get the symbol type
 * 
 * @param self The CbSymbol instance
 */
CbSymbolType cb_symbol_get_type(const CbSymbol* self);

/**
 * @memberof CbSymbol
 * @brief    (virtual) Get the symbols data type
 * 
 * @param self The CbSymbol instance
 */
CbVariantType cb_symbol_get_data_type(const CbSymbol* self);

/**
 * @memberof CbSymbol
 * @brief    Check if symbol represents a variable
 * 
 * @param self The CbSymbol instance
 */
bool cb_symbol_is_variable(const CbSymbol* self);

/**
 * @memberof CbSymbol
 * @brief    Check if symbol represents a function
 * 
 * @param self The CbSymbol instance
 */
bool cb_symbol_is_function(const CbSymbol* self);


#endif /* SYMBOL_H */
