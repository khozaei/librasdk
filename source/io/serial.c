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
#include <signal.h>

struct l_serial_device
{
	uint8_t				*port;
	uint8_t				access;
	int					fd;
	struct termios		config;
	struct termios		old_config;
	struct sigaction	action;
};

struct l_serial_devices
{
	LSerialDevice dev;
	struct l_serial_devices *next;
};

static struct l_serial_devices *l_serial_device_list;

speed_t l_validate_baudrate(const uint32_t);
void l_serial_action (int signum, siginfo_t *info, void *context);

LSerialDevice l_serial_device_new (const uint8_t *port)
{
	size_t port_length;
	struct l_serial_devices *tmp;
	port_length = (l_strlen(port) + 1);
	LSerialDevice dev = malloc(sizeof(struct l_serial_device));
	if (dev)
	{
		if ( l_serial_device_list )
		{
			tmp = l_serial_device_list;
			while ( tmp )
			{
				tmp = tmp->next;
			}
			tmp = malloc(sizeof(struct l_serial_devices));
			tmp->dev = dev;
			tmp->next = NULL;
		}
		else
		{
			l_serial_device_list = malloc(sizeof(struct l_serial_devices));
			l_serial_device_list->dev = dev;
			l_serial_device_list->next = NULL;
		}
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
		result = cfsetospeed(&serial_device->config, speed);
		return ((result == 0) && (cfsetispeed(&serial_device->config, speed) == 0));
	}
	return false;
}

void l_serial_set_parity (LSerialDevice serial_device, LParity parity)
{
	if ( serial_device )
	{
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

uint32_t l_serial_get_baudrate (LSerialDevice dev)
{
	speed_t baud;
	if ( dev )
	{
		if ( dev->fd > 0)
		{
			struct termios tmp;
			tcgetattr(dev->fd, &tmp);
			baud = cfgetispeed(&tmp);
		}
		else
		{
			baud = cfgetispeed(&dev->config);
		}
	}
	switch(baud)
	{
		case B50:
			return 50;
		case B75:
			return 75;
		case B110:
			return 110;
		case B134:
			return 134;
		case B150:
			return 150;
		case B200:
			return 200;
		case B300:
			return 300;
		case B600:
			return 600;
		case B1200:
			return 1200;
		case B2400:
			return 2400;
		case B4800:
			return 4800;
		case B9600:
			return 9600;
		case B19200:
			return 19200;
		case B38400:
			return 38400;
		case B57600:
			return 57600;
		case B115200:
			return 115200;
		case B230400:
			return 230400;
		case B460800:
			return 460800;
		case B500000:
			return 500000;
		case B576000:
			return 576000;
		case B921600:
			return 921600;
		case B1000000:
			return 1000000;
		case B1152000:
			return 1152000;
		case B1500000:
			return 1500000;
		case B2000000:
			return 2000000;
		//additional non-sparc baudrates
		case B2500000:
			return 2500000;
		case B3000000:
			return 3000000;
		case B3500000:
			return 3500000;
		case B4000000:
			return 4000000;

		default:
			return 0;
	}
}

LParity l_serial_get_parity (LSerialDevice dev)
{
	struct termios tmp;
	if ( dev )
	{
		if ( dev->fd > 0 )
		{
			tcgetattr(dev->fd, &tmp);
		}
		else
		{
			memcpy(&tmp,&dev->config,sizeof(dev->config));
		}

		if ( (tmp.c_cflag & PARENB) > 0 )
		{
			if ( (tmp.c_cflag & PARODD) > 0 )
			{
				return L_PARITY_ODD;
			}
			else
			{
				return L_PARITY_EVEN;
			}
		}
		else
		{
			return L_PARITY_NONE;
		}
	}
	return L_PARITY_NONE;
}

LAccessMode l_serial_get_access_mode (LSerialDevice dev)
{
	int oflag;
	if ( dev )
	{
		if ( dev->fd > 0 )
		{
			oflag = fcntl(dev->fd, F_GETFL);
			if ( (oflag & O_RDWR) > 0 )
			{
				return L_READ_WRITE;
			}
			else if ( (oflag & O_WRONLY) > 0 )
			{
				return L_WRITE_ONLY;
			}
			else
			{
				return L_READ_ONLY;
			}
		}
		else
		{
			return dev->access;
		}
	}
	return L_ACCESS_NONE;
}

uint8_t l_serial_get_databits (LSerialDevice dev)
{
	struct termios tmp;
	if ( dev )
	{
		if ( dev->fd > 0 )
		{
			tcgetattr(dev->fd, &tmp);
		}
		else
		{
			memcpy(&tmp,&dev->config,sizeof(dev->config));
		}

		switch (tmp.c_cflag & CSIZE)
		{
			case CS5:
				return 5;
			case CS6:
				return 6;
			case CS7:
				return 7;
			case CS8:
				return 8;
		}
	}
	return 0;
}

uint8_t l_serial_get_stopbits (LSerialDevice dev)
{
	struct termios tmp;
	if ( dev )
	{
		if ( dev->fd > 0 )
		{
			tcgetattr(dev->fd, &tmp);
		}
		else
		{
			memcpy(&tmp,&dev->config,sizeof(dev->config));
		}

		if (tmp.c_cflag & CSTOPB)
		{
			return 2;
		}
	}
	return 1;
}

bool l_serial_get_local_echo (LSerialDevice dev)
{
	struct termios tmp;
	if ( dev )
	{
		if ( dev->fd > 0 )
		{
			tcgetattr(dev->fd, &tmp);
		}
		else
		{
			memcpy(&tmp,&dev->config,sizeof(dev->config));
		}

		if (tmp.c_cflag & CLOCAL)
		{
			return true;
		}
	}
	return false;
}

LHandshake l_serial_get_handshake (LSerialDevice dev)
{
	struct termios tmp;
	bool software, hardware;
	software = false;
	hardware = false;
	if ( dev )
	{
		if ( dev->fd > 0 )
		{
			tcgetattr(dev->fd, &tmp);
		}
		else
		{
			memcpy(&tmp,&dev->config,sizeof(dev->config));
		}

		software = ((tmp.c_cflag & (IXON | IXOFF)) > 0);
		hardware = ((tmp.c_cflag & CRTSCTS) > 0);
	}
	if ( software && hardware )
	{
		return L_HANDSHAKE_BOTH;
	}
	else if ( hardware )
	{
		return L_HANDSHAKE_HARDWARE;
	}
	else if ( software )
	{
		return L_HANDSHAKE_SOFTWARE;
	}
	return L_HANDSHAKE_NONE;
}

void l_serial_set_receive_irq (LSerialDevice dev, bool enable)
{
	struct sigaction old_action;
	if ( dev )
	{
		if ( dev->fd != -1)
		{
			if ( enable )
			{
				dev->action.sa_sigaction = 	l_serial_action;
				dev->action.sa_flags |= SA_SIGINFO;
				dev->action.sa_restorer = NULL;
				sigaction(SIGIO, &dev->action, NULL);
				fcntl(dev->fd, F_SETFL, O_ASYNC | FNDELAY);
				fcntl(dev->fd, F_GETOWN, getpid());
			}
			else
			{
				memcpy(&old_action, &dev->action, sizeof(dev->action));
				dev->action.sa_sigaction = NULL;
				dev->action.sa_flags = SA_RESETHAND;
				sigaction(SIGIO, &dev->action, &old_action);
			}
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

void l_serial_action (int signum, siginfo_t *info, void *context)
{
	struct l_serial_devices *tmp;
	UNUSED(signum)
	UNUSED(context)
	if ( info )
	{
		tmp = l_serial_device_list;
		while ( tmp )
		{
			if (tmp->dev)
			{
				if ( tmp->dev->fd == info->si_fd )
				{
					l_serial_receive_irq_callback(tmp->dev);
				}
			}
		}
	}
}

__weak void l_serial_receive_irq_callback (LSerialDevice dev)
{
	UNUSED(dev)
}
