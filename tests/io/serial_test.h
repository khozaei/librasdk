#include <stdarg.h>                 
#include <stddef.h> 
#include <setjmp.h>
#include <cmocka.h>

int serial_setup (void **state);
int serial_teardown (void **state);
void serial_device_new (void **state);
void serial_destroy (void **state);
void serial_set_baudrate (void **state);
void serial_set_parity (void **state);
void serial_set_access_mode (void **state);
void serial_set_handshake (void **state);
void serial_set_local_echo (void **state);
void serial_set_databits (void **state);
void serial_set_stopbits (void **state);
void serial_configure (void **state);
void serial_configure_115200_8N1 (void **state);
void serial_configure_9600_8N1 (void **state);
