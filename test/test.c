#include "test.h"


/* -------------------------------------------------------------------------- */
int main(int argc, char* argv[])
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(vector_common_test),
        cmocka_unit_test(vector_get_test),
        cmocka_unit_test(variant_alloc_test),
        cmocka_unit_test(variant_types_test),
        cmocka_unit_test_setup_teardown(error_print_test, setup_error_handling, teardown_error_handling),
        cmocka_unit_test_setup_teardown(error_init_test, setup_error_handling, teardown_error_handling),
        cmocka_unit_test_setup_teardown(error_handling_test, setup_error_handling, teardown_error_handling),
        cmocka_unit_test_setup_teardown(stack_common_test, setup_stack, teardown_stack),
        cmocka_unit_test_setup_teardown(stack_push_pop_test, setup_stack, teardown_stack),
        cmocka_unit_test_setup_teardown(hash_table_common_test, setup_hash_table, teardown_hash_table),
        cmocka_unit_test_setup_teardown(hashtable_insert_remove_test, setup_hash_table, teardown_hash_table),
        cmocka_unit_test(symbol_table_common_test),
        cmocka_unit_test(ast_alloc_test),
        cmocka_unit_test(ast_eval_test),
        cmocka_unit_test(ast_check_semantic_test),
        cmocka_unit_test_setup_teardown(ast_check_semantic_error_test, setup_error_handling, teardown_error_handling)
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}
