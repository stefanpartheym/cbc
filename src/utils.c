#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <errno.h>

#include "utils.h"


/* -------------------------------------------------------------------------- */

void memclr(void* memory, size_t length)
{
    memset(memory, 0, length);
}

void* memalloc(size_t size)
{
    void* memory = malloc(size);
    
    /*
     * TODO:
     * Implement some kind of error handling API to correctly handle an error
     * like the following.
     */
    if (memory == NULL) raise_last_error();
    
    return memory;
}

void memfree(void* memory)
{
    free(memory);
}

void* memrealloc(void* memory, size_t size)
{
    void* temp = realloc(memory, size);
    if (temp != NULL)
        memory = temp;
    
    return temp;
}


/* -------------------------------------------------------------------------- */

bool strequ(const char* str1, const char* str2)
{
    return (strcmp(str1, str2) == 0);
}

bool strnequ(const char* str1, const char* str2, size_t length)
{
    return (strncmp(str1, str2, length) == 0);
}


/* -------------------------------------------------------------------------- */

void _raise_error(const char* message,
                  const char* function,
                  const char* file,
                  int line)
{
    fprintf(stderr, "ERROR: %s:%d (%s): %s\n",
            file, line, function, message);
    abort();
}

void _raise_last_error(const char* function,
                       const char* file,
                       int line)
{
    int errnum = errno;
    _raise_error(strerror(errnum), function, file, line);
}


/* -------------------------------------------------------------------------- */

size_t fsize(FILE* f)
{
    size_t size;
    size_t pos = ftell(f); /* memorize current position */
    
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, pos); /* seek back to previous position */
    
    return size;
}

void msleep(unsigned int mseconds)
{
    clock_t t;
    const unsigned long CLOCKS_PER_MSEC = (int) ((float) CLOCKS_PER_SEC * 1e-3);
    unsigned long interval;
    
    interval = CLOCKS_PER_MSEC * mseconds;
    
    t = clock();
    while ((clock() - t) < interval) {}
}

bool dequal(double a, double b)
{
    return fabs(a - b) < DBL_EPSILON;
}


/* -------------------------------------------------------------------------- */

#ifdef __STDC__
char* strdup(const char* s)
{
    char* p = malloc(strlen(s) + 1);
    if(p) strcpy(p, s);
    return p;
}
#endif /* __STDC__ */
