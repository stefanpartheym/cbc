/*******************************************************************************
 * cbc -- Codeblock compiler
 ******************************************************************************/

#include <stdio.h>
#include "utils.h"
#include "cb_utils.h"
#include "error_handling.h"
#include "symbol_table.h"
#include "cbc_lexer.h"
#include "cbc_parser.h"


/* -------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
    CbAstNode* ast = NULL;
    
    cb_error_initialize(stderr);
    
    if (yyparse(&ast) == 0)
    {
        CbSymbolTable* symbols = cb_symbol_table_create();
        
        if (cb_ast_node_check_semantic(ast, symbols))
        {
            CbVariant* result;
            result = cb_ast_node_eval(ast, symbols);
            if (result == NULL)
                cb_error_process();
            else
            {
                char* result_str = cb_variant_to_string(result);
                printf("Result: %s\n", result_str);
                memfree(result_str);
                cb_variant_destroy(result);
            }
        }
        else
            cb_error_process();
        
        cb_symbol_table_destroy(symbols);
        cb_ast_node_destroy(ast);
    }
    
    cb_error_finalize();
    
    return 0;
}
