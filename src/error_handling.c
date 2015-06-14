#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "cb_utils.h"
#include "utils.h"
#include "error_handling.h"


/* -------------------------------------------------------------------------- */

typedef struct CbError CbError;
struct CbError
{
    CbErrorType type;
    int line;
    char* message;
};

static FILE* err_out        = NULL; /* default error output stream */
static CbError* err_object  = NULL;
static bool err_initialized = false;


/*
 * Create an error object.
 */
static CbError* cb_error_create(CbErrorType type, int line, const char* message);

/*
 * Destroy an error object.
 */
static void cb_error_destroy(CbError* self);

/*
 * Format and print message including additional information (internal)
 */
static void cb_error_print_internal(FILE* output, CbErrorType type, int line,
                                    const char* format, va_list* args);

/*
 * Format and print message (internal)
 */
static void cb_error_print_msg_internal(FILE* output, const char* format,
                                        va_list* args);


/* -------------------------------------------------------------------------- */

void cb_error_print(CbErrorType type, int line, const char* message, ...)
{
    va_list arglist;
    va_start(arglist, message);
    cb_error_print_internal(err_out, type, line, message, &arglist);
    va_end(arglist);
}

void cb_error_print_msg(const char* message, ...)
{
    va_list arglist;
    fprintf(err_out, "Error: ");
    va_start(arglist, message);
    cb_error_print_msg_internal(err_out, message, &arglist);
    va_end(arglist);
}

void cb_error_set_output(FILE* error_output)
{
    cb_assert(error_output != NULL); /* error_output must be a valid pointer */
    
    err_out = error_output;
}

void cb_error_initialize(FILE* error_output)
{
    /* make sure error handling is not initialized yet */
    cb_assert(!err_initialized);
    
    cb_error_set_output(error_output);
    err_initialized = true;
}

void cb_error_finalize()
{
    /* make sure error handling is already initialized */
    cb_assert(err_initialized);
    
    err_initialized = false;
}

bool cb_error_is_initialized()
{
    return err_initialized;
}

bool cb_error_occurred()
{
    cb_assert(err_initialized);
    
    return (err_object != NULL);
}

void cb_error_trigger(CbErrorType type, int line, const char* message, ...)
{
    /*
     * TODO: Old errors will be discarded, if this function is called more than
     *       once in a row without processing the old errors.
     *       -> Implement an error stack
     */
    
    va_list arglist;
    char* message_buf;
    
    /* error handling must be initialized for this function */
    cb_assert(err_initialized);
    
    /* allocate twice the memory of the message format string */
    message_buf = (char*) malloc(sizeof(char) * (strlen(message) * 2));
    
    va_start(arglist, message);
    vsprintf(message_buf, message, arglist);
    va_end(arglist);
    
    if (err_object != NULL)
    {
        /*
         * TODO: Maybe calling the function cb_error_process() would be
         *       reasonable here.
         */
        cb_error_destroy(err_object);
        err_object = NULL;
    }
    
    err_object = cb_error_create(type, line, message_buf);
    free(message_buf);
}

void cb_error_process()
{
    /* error handling must be initialized for this function */
    cb_assert(err_initialized);
    
    if (err_object != NULL)
    {
        cb_error_print(err_object->type, err_object->line, err_object->message);
        cb_error_destroy(err_object);
        err_object = NULL;
    }
}


/* -------------------------------------------------------------------------- */

static CbError* cb_error_create(CbErrorType type, int line, const char* message)
{
    CbError* self = (CbError*) malloc(sizeof(CbError));
    self->type    = type;
    self->line    = line;
    self->message = strdup(message);
    return self;
}

static void cb_error_destroy(CbError* self)
{
    free(self->message);
    free(self);
}

static void cb_error_print_internal(FILE* output, CbErrorType type, int line,
                                    const char* format, va_list* args)
{
    switch (type)
    {
        case CB_ERROR_SYNTAX:
            fprintf(output, "syntax error: "); break;
        
        case CB_ERROR_SEMANTIC:
            fprintf(output, "semantic error: "); break;
        
        case CB_ERROR_RUNTIME:
            fprintf(output, "runtime error: "); break;
        
        default: fprintf(output, "unknown error: "); break;
    }
    
    if (line > 0)
        fprintf(output, "line %d: ", line);
    
    cb_error_print_msg_internal(output, format, args);
}

static void cb_error_print_msg_internal(FILE* output, const char* format,
                                        va_list* args)
{
    /* print formatted message */
    vfprintf(output, format, *args);
    fprintf(output, "\n");
}
