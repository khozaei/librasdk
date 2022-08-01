/*
 * Copyright (c) 2022, Hiva IoT (hiva-iot.com) and Amin Khozaei <amin.khozaei@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */
#include "libra_core.h"
#include "io/serial.h"
#include <stddef.h>
#include <stdint.h>
#include <termios.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

struct l_serial_device
{
	uint8_t			*port;
	uint32_t		baudrate;
	uint8_t			parity;
	uint8_t			access;
	uint8_t			data_bits;
	uint8_t			stop_bits;
	bool			local_echo;
	uint8_t			handshade_mode;
	//control variables
	int			    fd;
	struct termios  config;
	struct termios  old_config;
};

speed_t l_validate_baudrate(const uint32_t);


LSerialDevice l_serial_device_new (const uint8_t *port)
{
	size_t port_length;
	port_length = (l_strlen(port) + 1);
	LSerialDevice dev = malloc(sizeof(struct l_serial_device));
	if (dev)
	{
		dev->port = malloc ( sizeof(uint8_t) * port_length);
		l_strncpy(dev->port,port,port_length);
		//init termios config like cfmakeraw
		dev->config.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                | INLCR | IGNCR | ICRNL | IXON);
		dev->config.c_oflag &= ~OPOST;
		dev->config.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
		dev->config.c_cflag &= ~(CSIZE | PARENB);
		dev->config.c_cflag |= CS8;
		dev->config.c_cc[VTIME] = 0;
		dev->config.c_cc[VMIN] = 1;
	}
	return dev;
}

void l_serial_device_destroy (LSerialDevice serial_device)
{
	if (serial_device)
	{
		if (serial_device->fd > 0)
		{
			tcsetattr(serial_device->fd, TCSANOW, &serial_device->old_config);
		}
		free(serial_device->port);
		free(serial_device);
	}
}

bool l_serial_set_baudrate (LSerialDevice serial_device, const uint32_t baudrate)
{
	int result;
	speed_t speed;
	speed = l_validate_baudrate(baudrate); 
	if (serial_device && speed != B0)
	{
		serial_device->baudrate = baudrate;
		result = cfsetospeed(&serial_device->config, speed);
		return ((result == 0) && (cfsetispeed(&serial_device->config, speed) == 0));
	}
	return false;
}

void l_serial_set_parity (LSerialDevice serial_device, LParity parity)
{
	if ( serial_device )
	{
		serial_device->parity = parity;
		serial_device->config.c_cflag &= ~( PARENB | PARODD );
		if (parity == L_PARITY_EVEN)
		{
			serial_device->config.c_cflag |= PARENB;
		}
		else if (parity == L_PARITY_ODD)
		{
			serial_device->config.c_cflag |= ( PARENB | PARODD );
		}
	}
}

void l_serial_set_access_mode (LSerialDevice serial_device, LAccessMode access)
{
	if ( serial_device )
	{
		serial_device->access = access;
	}
}

void l_serial_set_databits (LSerialDevice dev, uint8_t databits)
{
	if ( dev )
	{
		switch (databits)
		{
			case 5:
				dev->config.c_cflag = ( dev->config.c_cflag & ~CSIZE ) | CS5;
				break;
			case 6:
				dev->config.c_cflag = ( dev->config.c_cflag & ~CSIZE ) | CS6;
				break;
			case 7:
				dev->config.c_cflag = ( dev->config.c_cflag & ~CSIZE ) | CS7;
				break;
			case 8:
			default:
				dev->config.c_cflag = ( dev->config.c_cflag & ~CSIZE ) | CS8;
				break;
		}
		dev->config.c_cflag |= CLOCAL | CREAD;
	}
}

void l_serial_set_stopbits (LSerialDevice dev, uint8_t stopbits)
{
	if ( dev )
	{
		if ( stopbits == 2 )
		{
			dev->config.c_cflag |= CSTOPB;
		}
		else
		{
			dev->config.c_cflag &= ~CSTOPB;
		}
	}
}

void l_serial_set_local_echo (LSerialDevice dev, bool echo)
{
	if ( dev )
	{
		if (echo)
		{
			dev->config.c_lflag |= ECHO;
		}
		else
		{
			dev->config.c_lflag &= ~ECHO;
		}
	}
}

void l_serial_set_handshake (LSerialDevice dev, LHandshake handshake)
{
	if ( dev )
	{
		if ( handshake == L_HANDSHAKE_SOFTWARE || handshake == L_HANDSHAKE_BOTH)
		{
			dev->config.c_iflag |= IXON | IXOFF;
		}
		else
		{
			dev->config.c_iflag &= ~(IXON | IXOFF | IXANY);
		}

		if (handshake == L_HANDSHAKE_HARDWARE || handshake == L_HANDSHAKE_BOTH)
		{
			dev->config.c_cflag |= CRTSCTS;
		}
		else
		{
			dev->config.c_cflag &= ~CRTSCTS;
		}
	}
}

void l_serial_connect(LSerialDevice dev)
{
	int flag;
	if ( dev )
	{
		if ( dev->fd > 0 )
		{
			close(dev->fd);
			dev->fd = -1;
		}
		switch(dev->access)
			{
				case L_READ_ONLY:
					flag = (O_RDONLY | O_NONBLOCK);
					break;
				case L_WRITE_ONLY:
					flag = (O_WRONLY | O_NONBLOCK);
					break;
				case L_READ_WRITE:
					flag = (O_RDWR | O_NONBLOCK);
					break;
				default:
					flag = (O_RDONLY | O_NONBLOCK);
			}
		dev->fd = l_open(dev->port, flag);
		if (dev->fd > 0)
		{
			tcgetattr(dev->fd, &dev->old_config);
			tcflush(dev->fd, TCIOFLUSH);
			tcsetattr(dev->fd, TCSANOW, &dev->config);
		}

	}
}

void l_serial_disconnect (LSerialDevice dev)
{
	if ( dev )
	{
		if ( dev->fd > 0 )
		{
			tcsetattr(dev->fd, TCSANOW, &dev->config);
			close(dev->fd);
			dev->fd = -1;
		}
	}
}

intmax_t l_serial_transfer (LSerialDevice dev, uint8_t *data, size_t length)
{
	intmax_t res;
	res = 0;
	if ( dev )
	{
		if ( dev->fd > 0 )
		{
			res = write(dev->fd, data, length);
			tcdrain(dev->fd);
		}
	}
	return res;
}

intmax_t l_serial_receive (LSerialDevice dev, uint8_t *data, size_t length)
{
	intmax_t res;
	res = 0;
	if ( dev )
	{
		if ( dev->fd > 0)
		{
			res = read(dev->fd, data, length);
		}
	}
	return res;
}

bool l_serial_configure (LSerialDevice dev, const uint32_t baudrate, LParity parity, 
		LAccessMode access, uint8_t databits, uint8_t stopbits, bool echo, LHandshake handshake)
{
	bool res;
	res = false;
	if ( dev )
	{
		res = l_serial_set_baudrate(dev, baudrate);
		l_serial_set_parity(dev, parity);
		l_serial_set_access_mode(dev, access);
		l_serial_set_databits(dev, databits);
		l_serial_set_stopbits(dev, stopbits);
		l_serial_set_local_echo(dev, echo);
		l_serial_set_handshake(dev, handshake);
	}
	return res;
}

void l_serial_configure_115200_8N1 (LSerialDevice dev)
{
	l_serial_configure(dev, 115200, L_PARITY_NONE, L_READ_WRITE, 8, 1, false, L_HANDSHAKE_NONE);
}

void l_serial_configure_9600_8N1 (LSerialDevice dev)
{
	l_serial_configure(dev, 9600, L_PARITY_NONE, L_READ_WRITE, 8, 1, false, L_HANDSHAKE_NONE);
}

speed_t l_validate_baudrate(const uint32_t baud)
{
	switch(baud)
	{
		case 50:
			return B50;
		case 75:
			return B75;
		case 110:
			return B110;
		case 134:
			return B134;
		case 150:
			return B150;
		case 200:
			return B200;
		case 300:
			return B300;
		case 600:
			return B600;
		case 1200:
			return B1200;
		case 2400:
			return B2400;
		case 4800:
			return B4800;
		case 9600:
			return B9600;
		case 19200:
			return B19200;
		case 38400:
			return B38400;
		case 57600:
			return B57600;
		case 115200:
			return B115200;
		case 230400:
			return B230400;
		case 460800:
			return B460800;
		case 500000:
			return B500000;
		case 576000:
			return B576000;
		case 921600:
			return B921600;
		case 1000000:
			return B1000000;
		case 1152000:
			return B1152000;
		case 1500000:
			return B1500000;
		case 2000000:
			return B2000000;
		//additional non-sparc baudrates
		case 2500000:
			return B2500000;
		case 3000000:
			return B3000000;
		case 3500000:
			return B3500000;
		case 4000000:
			return B4000000;

		default:
			return B0;
	}
}
