/*******************************************************************************
 * CbCodeblock -- Main structure to parse and execute codeblock source code.
 * 
 * The CbCodeblock structure wraps the abstract syntax tree (AST), its result
 * and the global symbol table.
 ******************************************************************************/

#ifndef CODEBLOCK_H
#define CODEBLOCK_H


#include <stdio.h>
#include "variant.h"


/* -------------------------------------------------------------------------- */

typedef struct CbCodeblock CbCodeblock;


/* -------------------------------------------------------------------------- */

/*
 * Create a CbCodeblock object.
 */
CbCodeblock* cb_codeblock_create();

/*
 * Destroy a CbCodeblock object.
 */
void cb_codeblock_destroy(CbCodeblock* self);

/*
 * Parse a codeblock source file.
 */
bool cb_codeblock_parse_file(CbCodeblock* self, FILE* input);

/*
 * Parse a codeblock source string.
 */
bool cb_codeblock_parse_string(CbCodeblock* self, const char* string);

/*
 * Execute the parsed codeblock.
 */
bool cb_codeblock_execute(CbCodeblock* self);

/*
 * Get the result of the executed codeblock.
 */
const CbVariant* cb_codeblock_get_result(const CbCodeblock* self);


#endif /* CODEBLOCK_H */
