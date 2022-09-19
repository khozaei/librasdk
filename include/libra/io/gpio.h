/*
 * Copyright (c) 2022, Amin Khozaei <amin.khozaei@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef LIBRA_GPIO_H
#define LIBRA_GPIO_H

/*! \enum l_gpio_direction
    \brief An enum for GPIO direction
	
	An enum to select direction of GPIO with possible values of L_GPIO_DIRECTION_IN
	for input mode and L_GPIO_DIRECTION_OUT for output mode
 */
enum l_gpio_direction
{
	L_GPIO_DIRECTION_IN, /**< DIRECTION_IN: input mode */
	L_GPIO_DIRECTION_OUT /**< DIRECTION_OUT: output mode */
};

/*! \enum l_gpio_value
    \brief An enum for value of GPIO
	
	An enum for value of GPIO instead of 0 or 1, with possible values of L_GPIO_LOW
	for zero and L_GPIO_HIGH for one
 */
enum l_gpio_value
{
	L_GPIO_LOW = 0, /**< LOW: zero */
	L_GPIO_HIGH = 1 /**< HIGH: one */
};

/*! \enum l_gpio_event
    \brief An enum to enable event
	
	An enum to enable event with different modes like L_GPIO_EDGE_NONE
	for disable event, L_GPIO_EDGE_RISING for enable event on rising edge,
	L_GPIO_EDGE_FALLING for enable event on falling edge and L_GPIO_EDGE_BOTH
	for enable event om both falling and rising edges.
 */
enum l_gpio_event
{
	L_GPIO_EDGE_NONE,    /**< NONE: disables events */
	L_GPIO_EDGE_RISING,  /**< RISING: catches events on rising edges */
	L_GPIO_EDGE_FALLING, /**< FALLING: catches events on falling edges */
	L_GPIO_EDGE_BOTH     /**< BOTH: cathes both rising and falling edges */
};

/*! \enum l_gpio_mode
    \brief An enum for gpio mode
	
	An enum for changing GPIO working mode. with possible modes below: 
		L_GPIO_MODE_SYSFS: GPIO enables in sysfs mode
		L_GPIO_MODE_CHARDEV: enables new way to manage GPIO
		L_GPIO_MODE_MMAP: uses memory map to manage GPIO
 */
enum l_gpio_mode
{
	L_GPIO_MODE_SYSFS,    /**< SYSFS: uses /sys/class files to manage GPIO */
	L_GPIO_MODE_CHARDEV,  /**< CHARDEV: uses character device in /dev, to manage GPIO */
	L_GPIO_MODE_MMAP	  /**< MMAP: uses memory map /dev/memmap to manage GPIO */
};

/*! \enum l_gpio_pull
    \brief An enum to enable pull-up/pull-down
	
	An enum for changing GPIO pull mode. with possible modes below: 
		L_GPIO_PULL_NOPULL: disables pull-up and pull-down
		L_GPIO_PULL_UP: enables pull-up
		L_GPIO_PULL_DOWN: enables pull-down
 */
enum l_gpio_pull
{
	L_GPIO_PULL_NOPULL,    /**< NOPULL: Neither pull-up nor pull-down */
	L_GPIO_PULL_UP,		   /**< PULL_UP: pull-up sets into gpio */
	L_GPIO_PULL_DOWN	   /**< PULL_DOWN: pull-down sets into gpio */
};

/*! \typedef LGpioDirection
    \brief A type for GPIO direction (input or output)

    A type for l_gpio_direction enum
*/
typedef enum l_gpio_direction LGpioDirection;

/*! \typedef LGpioMode
    \brief A type for GPIO mode (sysfs, chardev or mmap)

    A type for l_gpio_mode enum
*/
typedef enum l_gpio_mode LGpioMode;

/*! \typedef LGpioEvent
    \brief A type for GPIO event (none, rising, falling or both)

    A type for l_gpio_event enum
*/
typedef enum l_gpio_event LGpioEvent;

/*! \typedef LGpioValue
    \brief A type for GPIO value (LOW or HIGH)

    A type for l_gpio_value enum
*/
typedef enum l_gpio_value LGpioValue;

/*! \typedef LGpioPull
    \brief A type for GPIO Pull option

    A type for l_gpio_pull enum
*/
typedef enum l_gpio_pull LGpioPull;

#endif

