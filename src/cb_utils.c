#ifndef DEBUG
#include <assert.h>
#endif /* not DEBUG */

#include <stdlib.h>
#include <stdio.h>

#include "cb_utils.h"


/* -------------------------------------------------------------------------- */

/*
 * Custom assert function with additional information
 */
void _cb_assert(const int condition,
                const char* condition_str,
                const char* file,
                const int line,
                const char* function)
{
    if (!condition)
    {
        fprintf(stderr,
                "CB_ASSERT: assertion \"%s\" failed: "\
                "file \"%s\", line %d, function: %s\n",
                condition_str, file, line, function);
        /* -> terminate running application */
        abort();
    }
}

/*
 * Print error message and abort()
 */
void _cb_abort(const char* message,
               const char* file,
               const int line,
               const char* function)
{
    fprintf(stderr,
            "CB_ABORT: exception in file \"%s\", line %d, function: %s\n"\
            " -> %s\n",
            file, line, function, message);
    /* -> terminate running application */
    abort();
}
