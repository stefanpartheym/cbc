/*******************************************************************************
 * Internal declaration of CbSymbol:
 * Used to access CbSymbol-properties in any derived symbol type
 * (like CbVariable and CbFunction).
 * 
 * NOTE: Do not include/use this header anywhere else than in CbSymbol
 *       implementation files!
 * 
 ******************************************************************************/

#ifndef SYMBOL_INTERNAL_H
#define SYMBOL_INTERNAL_H

#include "symbol.h"


/* -------------------------------------------------------------------------- */

typedef void          (*CbSymbolDestructorFunc)  (CbSymbol*);
typedef CbVariantType (*CbSymbolGetDataTypeFunc) (const CbSymbol*);

struct CbSymbol
{
    CbSymbolType type;
    char* identifier;
    
    CbSymbolDestructorFunc  destructor;
    CbSymbolGetDataTypeFunc get_data_type;
};


/* -------------------------------------------------------------------------- */


/*
 * Initialize a CbSymbol object.
 */
void cb_symbol_init(CbSymbol* self,
                    CbSymbolType type,
                    const char* identifier,
                    CbSymbolDestructorFunc destructor,
                    CbSymbolGetDataTypeFunc get_data_type);


#endif /* SYMBOL_INTERNAL_H */
