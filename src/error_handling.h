/*******************************************************************************
 * error_handling -- Collection of error handling utilities
 ******************************************************************************/

#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H


#include <stdio.h>
#include <stdbool.h>


typedef enum
{
    CB_ERROR_UNKNOWN,
    CB_ERROR_SYNTAX,
    CB_ERROR_SEMANTIC,
    CB_ERROR_RUNTIME
} CbErrorType;

/*
 * Print error information
 */
void cb_error_print(CbErrorType type, int line, const char* message, ...);

/*
 * Print error message
 */
void cb_error_print_msg(const char* message, ...);

/*
 * Set error output stream
 */
void cb_error_set_output(FILE* error_output);

/*
 * Initialize error handling
 */
void cb_error_initialize(FILE* error_output);

/*
 * Finalize error handling
 */
void cb_error_finalize();

/*
 * Check if error handling is initialized
 */
bool cb_error_is_initialized();

/*
 * Check if an error occurred (i.e. check if the global error flag is set)
 */
bool cb_error_occurred();

/*
 * Trigger an error (i.e. set an error and activate the global error flag)
 */
void cb_error_trigger(CbErrorType type, int line, const char* message, ...);

/*
 * Process an error object
 */
void cb_error_process();


#endif /* ERROR_HANDLING_H */
