#include "io/serial.h"

int main()
{
	LSerialDevice dev;

	dev = l_serial_device_new("/dev/ttyS1");
	l_serial_device_destroy ( &dev );
	l_serial_device_destroy ( &dev );
	return 0;
}
