#include "serial_test.h"
#include "libra/io/serial.h"

#include <stdlib.h>
#include <regex.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int serial_setup (void **state)
{
	LSerialDevice dev;

	dev = l_serial_device_new ("/dev/ttyS0");
	*state = dev;
	return (dev != NULL)?0:-1;
}

int serial_teardown (void **state)
{
	LSerialDevice dev;

	dev = *state;
	l_serial_device_destroy(&dev);
	return 0;
}

void serial_device_new (void **state)
{
	LSerialDevice dev;


	dev = l_serial_device_new("/dev/ttyS0");
	*state = dev;
	assert_non_null ( dev );
}

void serial_destroy (void **state)
{
	LSerialDevice dev;

	dev = *state;
	l_serial_device_destroy ( &dev );
	assert_null ( dev );
	//double free test
	l_serial_device_destroy ( &dev );
	assert_null ( dev );
}

void serial_set_baudrate (void **state)
{
	LSerialDevice dev;

	dev = *state;
	l_serial_set_baudrate (&dev , 115200);
	assert_int_equal( l_serial_get_baudrate ( &dev ), 115200);
	l_serial_set_baudrate (&dev, 12000);
	assert_int_not_equal( l_serial_get_baudrate(&dev), 12000);
}

void serial_set_parity (void **state)
{
	LSerialDevice dev;

	dev = *state;
	l_serial_set_parity (&dev, L_PARITY_EVEN);
	assert_int_equal ( l_serial_get_parity (&dev), L_PARITY_EVEN);
	l_serial_set_parity (&dev, 10);
	assert_int_not_equal ( l_serial_get_parity (&dev), 10);
}

void serial_set_access_mode (void **state)
{
	LSerialDevice dev;

	dev = *state;
	l_serial_set_access_mode (&dev, L_WRITE_ONLY);
	assert_int_equal ( l_serial_get_access_mode (&dev), L_WRITE_ONLY);
	l_serial_set_access_mode (&dev, 24);
	assert_int_equal (l_serial_get_access_mode (&dev), L_ACCESS_NONE);
}

void serial_set_handshake (void **state)
{
	LSerialDevice dev;

	dev = *state;
	l_serial_set_handshake (&dev, L_HANDSHAKE_BOTH);
	assert_int_equal (l_serial_get_handshake(&dev), L_HANDSHAKE_BOTH);
	l_serial_set_handshake (&dev, 27);
	assert_int_equal (l_serial_get_handshake(&dev), L_HANDSHAKE_NONE);
}

void serial_set_local_echo (void **state)
{
	LSerialDevice dev;

	dev = *state;
	l_serial_set_local_echo (&dev, true);
	assert_true (l_serial_get_local_echo (&dev));
}

void serial_set_databits (void **state)
{
	LSerialDevice dev;

	dev = *state;
	l_serial_set_databits (&dev, 7);
	assert_int_equal (l_serial_get_databits (&dev), 7);
	l_serial_set_databits (&dev, 13);
	assert_int_equal (l_serial_get_databits (&dev), 8);
}

void serial_set_stopbits (void **state)
{
	LSerialDevice dev;

	dev = *state;
	l_serial_set_stopbits (&dev, 2);
	assert_int_equal (l_serial_get_stopbits (&dev), 2);
	l_serial_set_stopbits (&dev, 11);
	assert_int_equal (l_serial_get_stopbits (&dev), 1);
}

void serial_configure (void **state)
{
	LSerialDevice dev;

	dev = *state;
	l_serial_configure (&dev, 19200, L_PARITY_ODD, L_READ_ONLY, 6, 1, false, L_HANDSHAKE_HARDWARE);
	assert_int_equal (l_serial_get_baudrate (&dev), 19200);
	assert_int_equal (l_serial_get_parity (&dev), L_PARITY_ODD);
	assert_int_equal (l_serial_get_access_mode (&dev), L_READ_ONLY);
	assert_int_equal (l_serial_get_databits (&dev), 6);
	assert_int_equal (l_serial_get_stopbits (&dev), 1);
	assert_int_equal (l_serial_get_handshake (&dev), L_HANDSHAKE_HARDWARE);
	assert_false (l_serial_get_local_echo (&dev));
}

void serial_configure_115200_8N1 (void **state)
{
	LSerialDevice dev;

	dev = *state;
	l_serial_configure_115200_8N1 (&dev);
	assert_int_equal (l_serial_get_baudrate (&dev), 115200);
	assert_int_equal (l_serial_get_parity (&dev), L_PARITY_NONE);
	assert_int_equal (l_serial_get_access_mode (&dev), L_READ_WRITE);
	assert_int_equal (l_serial_get_databits (&dev), 8);
	assert_int_equal (l_serial_get_stopbits (&dev), 1);
	assert_int_equal (l_serial_get_handshake (&dev), L_HANDSHAKE_NONE);
	assert_false (l_serial_get_local_echo (&dev));

}

void serial_configure_9600_8N1 (void **state)
{
	LSerialDevice dev;

	dev = *state;
	l_serial_configure_9600_8N1 (&dev);
	assert_int_equal (l_serial_get_baudrate (&dev), 9600);
	assert_int_equal (l_serial_get_parity (&dev), L_PARITY_NONE);
	assert_int_equal (l_serial_get_access_mode (&dev), L_READ_WRITE);
	assert_int_equal (l_serial_get_databits (&dev), 8);
	assert_int_equal (l_serial_get_stopbits (&dev), 1);
	assert_int_equal (l_serial_get_handshake (&dev), L_HANDSHAKE_NONE);
	assert_false (l_serial_get_local_echo (&dev));
}
