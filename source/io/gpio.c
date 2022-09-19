/*
 * Copyright (c) 2022, Amin Khozaei <amin.khozaei@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "libra/io/gpio.h"
#include <stdint.h>

struct l_gpio_device
{
	LGpioDirection	direction;
	LGpioEvent		event;
	LGpioMode		mode;
	LGpioPull		pull;
	uint16_t		pin;
};


