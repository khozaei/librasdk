#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "io/serial_test.h"

int main(void) 
{
    const struct CMUnitTest tests[] = {
    cmocka_unit_test_teardown(serial_device_new,serial_teardown),
	cmocka_unit_test_setup(serial_destroy,serial_setup),
	cmocka_unit_test_setup_teardown(serial_set_baudrate,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_set_parity,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_set_access_mode,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_set_handshake,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_set_local_echo,serial_setup,serial_teardown),	
	cmocka_unit_test_setup_teardown(serial_set_databits,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_set_stopbits,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_configure,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_configure_115200_8N1,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_configure_9600_8N1,serial_setup,serial_teardown),
    };
    cmocka_run_group_tests_name("serial",tests, NULL, NULL);
    return 0;
}
