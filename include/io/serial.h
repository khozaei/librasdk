/*
 * Copyright (c) 2022, Hiva IoT (hiva-iot.com) and Amin Khozaei <amin.khozaei@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef LIBRA_SERIAL_H
#define LIBRA_SERIAL_H
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*! \enum l_parity
    \brief An enum to select parity type

    An enum to select parity type with possible values of L_PARITY_NONE,
    L_PARITY_ODD, and L_PARITY_EVEN
*/
enum l_parity
{
	L_PARITY_NONE, /**< NONE: Without parity */
	L_PARITY_ODD,  /**< ODD: With odd parity */
	L_PARITY_EVEN  /**< EVEN: With even parity */
};

/*! \enum l_access_mode
    \brief An enum to select serial access mode

    An enum to select serial access mode with possible values of L_READ_ONLY,
    L_WRITE_ONLY, and L_READ_WRITE
*/
enum l_access_mode
{
	L_READ_ONLY  = 0,    /**< READ_ONLY: The serial turns to read access only */
	L_WRITE_ONLY = 1,    /**< WRITE_ONLY: The serial turns to write access only*/
	L_READ_WRITE = 2,    /**< READ_WRITE: the serial turns to full access mode */
	L_ACCESS_NONE = 255  /**< ACCESS_NONE: without permission */
};

/*! \enum l_handshake
    \brief An enum to select handshake mode

    An enum to select handshake mode with possible values of L_HANDSHAKE_NONE,
    L_HANDSHAKE_SOFTWARE, L_HANDSHAKE_HARDWARE, and L_HANDSHAKE_BOTH
*/
enum l_handshake
{
	L_HANDSHAKE_NONE,		/**< NONE: Without handshake */
	L_HANDSHAKE_SOFTWARE,   /**< SOFTWARE: software handshake */
	L_HANDSHAKE_HARDWARE,   /**< HARDWARE: hardware handshake */
	L_HANDSHAKE_BOTH		/**< BOTH: both software and hardware handshake */
};


/*! \typedef LSerialDevice
    \brief A type for serial device

    A type for serial device to keep device configuration and management.
*/
typedef struct l_serial_device* LSerialDevice;

/*! \typedef LParity
    \brief A type for parity

    A type for l_parity enum
*/
typedef enum l_parity LParity;

/*! \typedef LAccessMode
    \brief A type for access mode

    A type for l_access_mode enum
*/
typedef enum l_access_mode LAccessMode;

/*! \typedef LHandshake
    \brief A type for handshake mode

    A type for l_handshake enum
*/
typedef enum l_handshake LHandshake;

/*! \fn LSerialDevice l_serial_device_new (uint8_t *port)
    \brief A function that creates new serial device of type LSerialDevice

    \param port: The port name, for ex. "/dev/ttyAMA0" or "/dev/ttyUSB0"
*/
LSerialDevice l_serial_device_new (const char *port);

/*! \fn void l_serial_device_destroy (LSerialDevice serial_device)
    \brief A function to free all allocated memory by the given serial device

    \param serial_device: The serial device that should be free
*/
void l_serial_device_destroy (LSerialDevice serial_device);

/*! \fn bool l_serial_set_baudrate (LSerialDevice serial_device, const uint32_t baudrate)
    \brief A function to set serial baud rate

    \param serial_device: The serial device
    \param baudrate: The baudrate value
    \return Returns true if baudrate value is valid else returns false
*/
bool l_serial_set_baudrate (LSerialDevice serial_device, const uint32_t baudrate);

/*! \fn void l_serial_set_parity (LSerialDevice serial_device, LParity parity)
    \brief A function to set serial parity

    \param serial_device: The serial device
    \param parity: The serial parity
*/
void l_serial_set_parity (LSerialDevice serial_device, LParity parity);

/*! \fn void l_serial_set_access_mode (LSerialDevice serial_device, LAccessMode access)
    \brief A function to set serial access mode

    \param serial_device: The serial device
    \param access: The serial access mode
*/
void l_serial_set_access_mode (LSerialDevice serial_device, LAccessMode access);

/*! \fn void l_serial_set_databits (LSerialDevice serial_device, uint8_t databits)
    \brief A function to set serial databits

    \param serial_device: The serial device
    \param databits: The serial databits value from 5 to 8
*/
void l_serial_set_databits (LSerialDevice serial_device, uint8_t databits);

/*! \fn void l_serial_set_stopbits (LSerialDevice serial_device, uint8_t stopbits)
    \brief A function to set serial stopbits

    \param serial_device: The serial device
    \param stopbits: The serial stopbits with value of 1 or 2
*/
void l_serial_set_stopbits (LSerialDevice serial_device, uint8_t stopbits);

/*! \fn void l_serial_set_local_echo (LSerialDevice serial_device, bool echo)
    \brief A function to set serial local echo

    \param serial_device: The serial device
    \param echo: The serial local echo enable/disable (true/false)
*/
void l_serial_set_local_echo (LSerialDevice serial_device, bool echo);

/*! \fn void l_serial_set_handshake (LSerialDevice serial_device, LHandshake handshake)
    \brief A function to set serial handshake mode

    \param serial_device: The serial device
    \param handshake: The serial handshake mode (NONE,SOFTWARE,HARDWARE,BOTH)
*/
void l_serial_set_handshake (LSerialDevice serial_device, LHandshake handshake);

/*! \fn uint32_t l_serial_get_baudrate (LSerialDevice serial_device)
    \brief A function to get current serial baud rate

    \param serial_device: The serial device
    \return Returns serial baudrate value
*/
uint32_t l_serial_get_baudrate (LSerialDevice serial_device);

/*! \fn LParity l_serial_get_parity (LSerialDevice serial_device)
    \brief A function to get serial parity

    \param serial_device: The serial device
    \return Returns serial parity
*/
LParity l_serial_get_parity (LSerialDevice serial_device);

/*! \fn LAccessMode l_serial_get_access_mode (LSerialDevice serial_device)
    \brief A function to get serial access mode

    \param serial_device: The serial device
    \return Returns serial access mode
*/
LAccessMode l_serial_get_access_mode (LSerialDevice serial_device);

/*! \fn uint8_t l_serial_get_databits (LSerialDevice serial_device)
    \brief A function to get serial databits

    \param serial_device: The serial device
    \return Returns serial databits value
*/
uint8_t l_serial_get_databits (LSerialDevice serial_device);

/*! \fn uint8_t l_serial_get_stopbits (LSerialDevice serial_device)
    \brief A function to get serial stopbits

    \param serial_device: The serial device
    \return Returns serial stopbits
*/
uint8_t l_serial_get_stopbits (LSerialDevice serial_device);

/*! \fn bool l_serial_get_local_echo (LSerialDevice serial_device)
    \brief A function to get serial local echo

    \param serial_device: The serial device
    \return Returns serial local echo state
*/
bool l_serial_get_local_echo (LSerialDevice serial_device);

/*! \fn LHandshake l_serial_get_handshake (LSerialDevice serial_device)
    \brief A function to get serial handshake mode

    \param serial_device: The serial device
    \return Returns serial handshake mode (NONE,SOFTWARE,HARDWARE,BOTH)
*/
LHandshake l_serial_get_handshake (LSerialDevice serial_device);

/*! \fn bool l_serial_configure (LSerialDevice serial_device, const uint32_t baudrate, LParity parity,
		LAccessMode access, uint8_t databits, uint8_t stopbits, bool local_echo, LHandshake handshake)
    \brief A function to configure serial device

    \param serial_device: The serial device
	\param baudrate: The baudrate value
	\param parity: The serial parity
	\param access: The serial access mode
	\param databits: The serial databits value from 5 to 8
	\param stopbits: The serial stopbits with value of 1 or 2
    \param local_echo: The serial local echo enable/disable (true/false)
	\param handshake: The serial handshake mode (NONE,SOFTWARE,HARDWARE,BOTH)
	\return Returns true if baudrate value is valid else returns false
*/
bool l_serial_configure (LSerialDevice serial_device, const uint32_t baudrate, LParity parity, 
		LAccessMode access, uint8_t databits, uint8_t stopbits, bool local_echo, LHandshake handshake);

/*! \fn void l_serial_configure_115200_8N1(LSerialDevice serial_device)
    \brief A function to configure serial with 115200 baudrate, 8 databits, none parity, 1 stopbits, and read_write access mode

    \param serial_device: The serial device
*/
void l_serial_configure_115200_8N1(LSerialDevice serial_device);

/*! \fn void l_serial_configure_9600_8N1(LSerialDevice serial_device)
    \brief A function to configure serial with 9600 baudrate, 8 databits, none parity, 1 stopbits, and read_write access mode

    \param serial_device: The serial device
*/
void l_serial_configure_9600_8N1(LSerialDevice serial_device);

/*! \fn void l_serial_set_receive_irq (LSerialDevice serial_device, bool enable)
    \brief A function to enable/disable IRQ on receive data

    \param serial_device: The serial device
	\param enable: true as enable and false as disable
*/
void l_serial_set_receive_irq (LSerialDevice serial_device, bool enable);

/*! \fn void l_serial_connect(LSerialDevice serial_device)
    \brief A function for connect to serial device

    \param serial_device: The serial device
*/
void l_serial_connect(LSerialDevice serial_device);

/*! \fn void l_serial_disconnect(LSerialDevice serial_device)
    \brief A function to disconnect from serial device

    \param serial_device: The serial device
*/
void l_serial_disconnect(LSerialDevice serial_device);

/*! \fn void l_serial_disconnect(LSerialDevice serial_device)
    \brief A function to disconnect from serial device

    \param serial_device: The serial device
	\param data: The data array to send over serial
	\param length: The length of data array
	\return Returns actually bytes sent
*/
intmax_t l_serial_transfer(LSerialDevice serial_device, uint8_t *data, size_t length);

/*! \fn uint8_t l_serial_receive(LSerialDevice serial_device, uint8_t *data, uint8_t length)
    \brief A function to read received data from serial device

    \param serial_device: The serial device
	\param outdata: The data array to save received data from serial
	\param length: The langth of data to read from serial
	\return Returns actually data received length
*/
intmax_t l_serial_receive(LSerialDevice serial_device, uint8_t *outdata, size_t length);

/*! \fn void l_serial_receive_irq_callback (int signum)
    \brief A callback function that defined by user to receive data using IRQ \
		the serial IRQ should be enable to have this feature by l_serial_set_receive_irq \
		function call

    \param serial_device: The serial device that caused the IRQ to handle
*/
void l_serial_receive_irq_callback (LSerialDevice serial_device);

#endif
