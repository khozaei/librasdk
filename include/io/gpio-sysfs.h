/// @file
/// @brief controlling GPIO using sysfs
#pragma once
#define LI_DIRECTION_INPUT  2
#define LI_DIRECTION_OUTPUT 3
#define LI_VALUE_HIGH       1
#define LI_VALUE_LOW        0
#define LI_EDGE_NONE        4
#define LI_EDGE_RISING      5
#define LI_EDGE_FALLING     6
#define LI_EDGE_BOTH        7

/**
* @brief opens a pin.
* @note be sure to close it after you`re done.
* @param pin one of the pins defined in /io/rpi/pins.h.
* @returns 0 success.
* @returns -1 failed to open the sysfs file.
* @returns -2 failed to write into the sysfs file.
*/
int pin_open(int pin);
/**
* @brief closes a pin.
* @param pin one of the pins defined in /io/rpi/pins.h.
* @returns 0 success.
* @returns -1 failed to open the sysfs file.
* @returns -2 failed to write into the sysfs file.
*/
int pin_close(int pin);
/**
* @brief sets pin direction.
* @note you need to have right permissions to execute this.
* @param pin one of the pins defined in /io/rpi/pins.h
* @param dir can be either LI_DIRECTION_INPUT or LI_DIRECTION_OUTPUT.
* @returns 0 success.
* @returns -1 failed to open the sysfs file.
* @returns -2 failed to write into the sysfs file.
*/
int pin_direction_set(int pin, int dir);
/**
* @brief gets pin direction.
* @note you need to have right permissions to execute this.
* @param pin one of the pins defined in /io/rpi/pins.h
* @param dir will be either LI_VALUE_LOW or LI_VALUE_HIGH after success.
* @returns 0 success and you can read dir now.
* @returns -1 failed to open the sysfs file.
* @returns -2 failed to read the sysfs file.
*/
int pin_direction_get(int pin, int* dir);
/**
* @brief sets pin value.
* @note you need to have right permissions to execute this.
* @param pin one of the pins defined in /io/rpi/pins.h.
* @param value can be either LI_VALUE_LOW or LI_VALUE_HIGH.
* @returns 0 success.
* @returns -1 failed to open the sysfs file.
* @returns -2 failed to write into the sysfs file.
*/
int pin_value_set(int pin, int value);
/**
* @brief gets pin value.
* @note you need to have right permissions to execute this.
* @param pin one of the pins defined in /io/rpi/pins.h.
* @param value will be either LI_VALUE_LOW or LI_VALUE_HIGH after success.
* @returns 0 success you can read value.
* @returns -1 failed to open the sysfs file.
* @returns -2 failed to read the sysfs file.
*/
int pin_value_get(int pin, int* value);
/**
* @brief toggles pin value.
* @note you need to have right permissions to execute this.
* @param pin one of the pins defined in /io/rpi/pins.h.
* @returns 0 success
* @returns -1 failed to open the sysfs file.
* @returns -2 failed to read the sysfs file.
*/
int pin_value_toggle(int pin);
/**
* @brief checks if a pin support edge.
* @param pin one of the pins defined in /io/rpi/pins.h.
* @returns 0 supports edge.
*/
int pin_edge_exists(int pin);
/**
* @brief sets pin edge.
* @note you need to have right permissions to execute this.
* @param pin one of the pins defined in /io/rpi/pins.h.
* @param edge can be either LI_EDGE_NONE, LI_EDGE_RISING, LI_EDGE_FALLING or LI_EDGE_BOTH.
* @returns 0 success.
* @returns -1 failed to open the sysfs file.
* @returns -2 failed to write into the sysfs file.
*/
int pin_edge_set(int pin, int edge);
/**
* @brief gets pin edge.
* @note you need to have right permissions to execute this.
* @param pin one of the pins defined in /io/rpi/pins.h.
* @param edge will be either LI_EDGE_NONE, LI_EDGE_RISING, LI_EDGE_FALLING or LI_EDGE_BOTH after succcess.
* @returns 0 success you can read edge.
* @returns -1 failed to open the sysfs file.
* @returns -2 failed to read the sysfs file.
*/
int pin_edge_get(int pin, int* edge);
/**
* @brief blocks the execution until value is changed or timeout expires.
* @note make sure that you set the proper edge for the pin.
* @note you need to have right permissions to execute this.
* @param pin one of the pins defined in /io/rpi/pins.h.
* @param value will be either LI_VALUE_LOW or LI_VALUE_HIGH after success.
* @param timeoutms returns after timeout expires (milliseconds).
* @returns 0 success and you can read value.
* @returns -1 failed to open the sysfs file.
* @returns -2 failed to read the sysfs file.
* @returns -3 timeout.
*/
int pin_value_get_evented(int pin, int* value, int timeoutms);