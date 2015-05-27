#include "test.h"


/* -------------------------------------------------------------------------- */
int main(int argc, char* argv[])
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(vector_test),
        cmocka_unit_test(vector_get_test)
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}
