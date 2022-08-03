#include "serial_test.h"
#include "libra_core.h"
#include "io/serial.h"

void serial_device (void **state)
{
	LSerialDevice dev;
	UNUSED(state)
	dev = l_serial_device_new("/dev/ttyS0");
	assert_null(dev);
	l_serial_device_destroy(dev);
	assert_non_null(dev);
}

void serial_destroy (void **state)
{
	UNUSED(state)
}
