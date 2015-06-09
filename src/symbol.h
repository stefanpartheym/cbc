/*******************************************************************************
 * @file  symbol.h
 * @brief Contains the CbSymbol structure
 * 
 * Representation of an identifier in Codeblock source code with its data type
 * information.
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
 * @brief    Constructor
 * 
 * @param identifier Name of the symbol (aka. identifier)
 * @param type       Type of the symbol
 * @param data_type  Specifies the symbols data-type
 */
CbSymbol* cb_symbol_create(const char* identifier,
                           CbSymbolType type,
                           CbVariantType data_type);

/**
 * @memberof CbSymbol
 * @brief    Destructor
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
