/*******************************************************************************
 * Unit-Test for cbc (using cmocka unit-test framework)
 ******************************************************************************/

#ifndef TEST_H
#define TEST_H

#include "test_utils.h"


void vector_test(void** state);
void vector_get_test(void** state);

void variant_alloc_test(void** state);
void variant_types_test(void** state);

int setup_error_handling(void** state);
int teardown_error_handling(void** state);
void error_print_test(void** state);
void error_init_test(void** state);
void error_handling_test(void** state);


#endif /* TEST_H */
