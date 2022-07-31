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
		//dev->fd = l_open(port ,O_RDONLY | O_NOCTTY | O_NDELAY);
		//if (dev->fd > 0)
		//{
		//	tcgetattr(dev->fd, &dev->old_config);
		//	memcpy(&dev->config,&dev->old_config,sizeof(dev->old_config));
		//}
		l_strncpy(dev->port,port,port_length);
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
	}
}

void l_serial_set_access_mode (LSerialDevice serial_device, LAccessMode access)
{
	if ( serial_device )
	{
		serial_device->access = access;
	}
	int flag;
	if ( serial_device )
	{
		if ( serial_device->fd > 0 )
		{
			close(serial_device->fd);
			switch(access)
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
			serial_device->fd = l_open(serial_device->port, flag);
		}
	}
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
