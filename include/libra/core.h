/*
 * Copyright (c) 2022, Amin Khozaei <amin.khozaei@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef  LIBRA_CORE_H
#define  LIBRA_CORE_H
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define __weak __attribute__((weak))
#define UNUSED(X) (void)(X);

enum l_platfrom
{
	RASPBERRY_PI,
	UNKNOWN
};

typedef enum l_platfrom LibraPlatform;

extern LibraPlatform libra_platform;

bool l_identify_platform(void);

#endif
