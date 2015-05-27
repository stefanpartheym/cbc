/*******************************************************************************
 * Collection of custom unit-testing utilities and custom assert-functions
 * for the cmocka unit-test framework
 ******************************************************************************/

#ifndef TEST_UTILS_H
#define TEST_UTILS_H

/* includes requiered for the cmocka unit-test framework */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
/* other includes */
#include <stdio.h>
#include <stdbool.h>


typedef struct TestDummy TestDummy;
struct TestDummy
{
    int id;
};


/* -------------------------------------------------------------------------- */

TestDummy* dummy_create(int id);
void dummy_destroy(TestDummy* self);


/* -------------------------------------------------------------------------- */

void stream_to_string(FILE* stream, char* string, bool trim);


#endif /* TEST_UTILS_H */
