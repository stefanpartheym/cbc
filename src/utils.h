/*******************************************************************************
 * utils.h
 * Collection of basic utility functions.
 ******************************************************************************/

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>


/* -------------------------------------------------------------------------- */
/* Memory function */

/*
 * Clear a block of memory / initialize memory with NULL-bytes.
 */
void memclr(void* memory, size_t length);

/*
 * [Wrapper for: malloc()]
 * Allocate a block of memory.
 * NOTE: This function is error checked and calls abort() if an error occurs.
 */
void* memalloc(size_t size);

/*
 * [Wrapper for: free()]
 * Free a block of allocate memory.
 */
void memfree(void* memory);

/*
 * [Wrapper for: realloc()]
 * Changes the size of an allocated memory block.
 */
void* memrealloc(void* memory, size_t size);


/* -------------------------------------------------------------------------- */
/* String functions */

/*
 * Check whether str1 and str2 are equal.
 */
bool strequ(const char* str1, const char* str2);

/*
 * Check whether str1 and str2 are equal within the provided range.
 */
bool strnequ(const char* str1, const char* str2, size_t length);


/* -------------------------------------------------------------------------- */
/* Error functions */

/*
 * Print a custom error message and abort the program.
 */
void _raise_error(const char* message,
                  const char* function,
                  const char* file,
                  int line);
#define raise_error(message) \
_raise_error(message, __func__, __FILE__, __LINE__)

/*
 * Print an error message - indecated by errno (errno.h) - and abort the
 * program.
 */
void _raise_last_error(const char* function,
                       const char* file,
                       int line);
#define raise_last_error() \
_raise_last_error(__func__, __FILE__, __LINE__)


/* -------------------------------------------------------------------------- */
/* Miscellaneous functions */

/*
 * Get the size of a file.
 */
size_t fsize(FILE* f);

/*
 * Suspend execution for millisecond intervals
 */
void msleep(unsigned int mseconds);

/*
 * Check if two double values are equal
 */
bool dequal(double a, double b);


/*******************************************************************************
 * Additional utility functions, that are not available in ANSI C.
 ******************************************************************************/
#ifdef __STDC__
/*
 * Duplicate a C string.
 */
char* strdup(const char* s);
#endif /* __STDC__ */


#endif /* UTILS_H */
