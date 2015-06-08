#include "test.h"


/* -------------------------------------------------------------------------- */
int main(int argc, char* argv[])
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(vector_test),
        cmocka_unit_test(vector_get_test),
        cmocka_unit_test(variant_alloc_test),
        cmocka_unit_test(variant_types_test),
        cmocka_unit_test_setup_teardown(error_print_test, setup_error_handling, teardown_error_handling),
        cmocka_unit_test_setup_teardown(error_init_test, setup_error_handling, teardown_error_handling),
        cmocka_unit_test_setup_teardown(error_handling_test, setup_error_handling, teardown_error_handling)
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}
