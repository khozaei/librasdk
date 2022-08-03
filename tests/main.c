#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "io/serial.h"
#include "io/serial_test.h"


int main(void) 
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(serial_device),
		cmocka_unit_test(serial_destroy),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
