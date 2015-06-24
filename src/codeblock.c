#include "utils.h"
#include "cb_utils.h"
#include "error_handling.h"
#include "symbol_table.h"
#include "ast.h"
#include "cbc_lexer.h"
#include "cbc_parser.h"
#include "codeblock.h"


/* -------------------------------------------------------------------------- */

enum CbCodeblockState
{
    CB_STATE_READY,
    CB_STATE_PARSED,
    CB_STATE_EXECUTED_SUCCESS,
    CB_STATE_EXECUTED_FAILURE
};

struct CbCodeblock
{
    CbVariant* result;
    CbSymbolTable* symbols;
    CbAstNode* ast;
    enum CbCodeblockState state;
};


/* -------------------------------------------------------------------------- */

/*
 * Parse the codeblock (internal)
 */
static bool cb_codeblock_parse_internal(CbCodeblock* self);

/*
 * Reset codeblock state.
 */
static void cb_codeblock_reset(CbCodeblock* self);


/* -------------------------------------------------------------------------- */

CbCodeblock* cb_codeblock_create()
{
    CbCodeblock* self = memalloc(sizeof(CbCodeblock));
    
    self->result  = NULL;
    self->ast     = NULL;
    self->state   = CB_STATE_READY;
    
    return self;
}

void cb_codeblock_destroy(CbCodeblock* self)
{
    cb_codeblock_reset(self);
    memfree(self);
}

bool cb_codeblock_parse_file(CbCodeblock* self, FILE* input)
{
    bool result = false;
    
    if (input) /* determine input stream */
        yyin = input;
    else
        yyin = stdin;
    
    result = cb_codeblock_parse_internal(self);
    
    yylex_destroy(); /* cleanup lexer */
    
    return result;
}

bool cb_codeblock_parse_string(CbCodeblock* self, const char* string)
{
    bool result = false;
    
    YY_BUFFER_STATE buffer_state = yy_scan_string(string);
    result = cb_codeblock_parse_internal(self);
    
    /* 
     * Cleanup
     * NOTE: Delete the buffer BEFORE calling yylex_destroy(), otherwise
     *       memory-leaks will occur!
     */
    yy_delete_buffer(buffer_state);
    yylex_destroy();
    
    return result;
}

bool cb_codeblock_execute(CbCodeblock* self)
{
    bool result;
    CbSymbolTable* symbols;
    
    cb_assert(self->state == CB_STATE_PARSED);
    
    self->state = CB_STATE_EXECUTED_FAILURE;
    result      = false;
    symbols     = cb_symbol_table_create();
    
    result = cb_ast_node_check_semantic(self->ast, symbols);
    if (result) self->result = cb_ast_node_eval(self->ast, symbols);
    
    if (cb_error_occurred())
    {
        result = false;
        cb_error_process();
    }
    else
        self->state = CB_STATE_EXECUTED_SUCCESS;
    
    cb_symbol_table_destroy(symbols);
    
    return result;
}

const CbVariant* cb_codeblock_get_result(const CbCodeblock* self)
{
    cb_assert(self->state == CB_STATE_EXECUTED_SUCCESS);
    
    return self->result;
}


/* -------------------------------------------------------------------------- */

static bool cb_codeblock_parse_internal(CbCodeblock* self)
{
    bool result = false;
    
    /*
     * Reset codeblock: This is necessary in case the codeblock was already
     * parsed or executed before.
     */
    cb_codeblock_reset(self);
    
    self->state = CB_STATE_PARSED;
    
    switch (yyparse(&self->ast))
    {
        case 0: result = true; break;
        
        case 1:
            cb_error_print_msg("Parsing failed due to invalid input");
            break;
        
        case 2:
            cb_error_print_msg("Parsing failed due to memory exhaustion");
            break;
    }
    
    return result;
}

static void cb_codeblock_reset(CbCodeblock* self)
{
    switch (self->state)
    {
        case CB_STATE_READY:    /* nothing to do */ break;
        
        case CB_STATE_EXECUTED_SUCCESS: cb_variant_destroy(self->result);
        case CB_STATE_EXECUTED_FAILURE:
        case CB_STATE_PARSED: cb_ast_node_destroy(self->ast);
                              self->state = CB_STATE_READY;
                              break;
    }
}
