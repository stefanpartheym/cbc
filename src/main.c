/*******************************************************************************
 * cbc -- Codeblock compiler
 ******************************************************************************/

#include "error_handling.h"
#include "codeblock.h"


/* -------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
    CbCodeblock* cb;
    bool parser_result;
    FILE* input     = NULL;
    bool parse_file = argc > 1;

    /*
     * Determine whether to parse a file or stdin.
     */
    if (parse_file)
    {
        input = fopen(argv[1], "r");
        if (!input)
        {
            cb_error_print_msg("Unable to open file `%s'", argv[1]);
            return 1;
        }
    }

    /*
     * Setup environment including error handling.
     */
    cb_error_initialize(stderr);
    cb = cb_codeblock_create();

    /*
     * Parse input stream:
     * Either stdin or a file specified on the command line.
     */
    parser_result = cb_codeblock_parse_file(cb, input);
    if (parse_file) fclose(input);

    /*
     * Execute the parsed Codeblock.
     */
    if (parser_result && cb_codeblock_execute(cb))
    {
        cb_variant_print(cb_codeblock_get_result(cb));
        printf("\n");
    }

    /*
     * Cleanup environment.
     */
    cb_codeblock_destroy(cb);
    cb_error_finalize();

    return 0;
}
