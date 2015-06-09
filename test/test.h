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
void resetup_error_handling(void** state);
void error_print_test(void** state);
void error_init_test(void** state);
void error_handling_test(void** state);

int setup_stack(void** state);
int teardown_stack(void** state);
void stack_common_test(void** state);
void stack_push_pop_test(void** state);

int setup_hash_table(void** state);
int teardown_hash_table(void** state);
void hash_table_common_test(void** state);
void hashtable_insert_remove_test(void** state);

void symbol_table_common_test(void** state);

void ast_alloc_test(void** state);
void ast_eval_test(void** state);
void ast_check_semantic_test(void** state);
void ast_check_semantic_error_test(void** state);


#endif /* TEST_H */
