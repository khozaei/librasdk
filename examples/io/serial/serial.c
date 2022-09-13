#include "libra/io/serial.h"
#include <stdio.h>

int main()
{
	LSerialDevice dev;
	FILE *fp;
	char output[1024];
	int res;

	fp = popen ("socat -d -d pty,raw,echo=0 pty,raw,echo=0","r");
	if ( fp )
	{
		fgets(output, 10, fp);
		printf("%s\n",output);
		pclose(fp);
	}
	dev = l_serial_device_new("/dev/ttyS1");
	l_serial_device_destroy ( &dev );
	l_serial_device_destroy ( &dev );
	return 0;
}
