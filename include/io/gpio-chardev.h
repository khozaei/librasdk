/// @file
/// @brief controlling the gpio using gpio charecter device.
#pragma once
#include <stdint.h>
#include <linux/const.h>
#include <linux/types.h>
#include <fcntl.h>

/**
 * @brief holds the information about a gpio chip.
 * @name: name of gpio device.
 * @label: label of gpio device.
 * @number: of lines on gpio device.
 */
struct li_gpio_chip_info
  {
  char name[32];
  char label[32];
  __u32 lines;
  };

/**
 * @brief holds information about a gpio line.
 * @name: the name of this GPIO line, may be empty.
 * @consumer: a name set by consumer of this pin. may be empty.
 * @flags: flags on this gpio device. refer to #li_gpio_config_flags for more information.
 */
struct li_gpio_line_info
  {
  char name[32];
  char consumer[32];
  __u32 flags;
  };

/**
 * @brief flags on a gpio line.
 * @LI_GPIO_CONFIG_USED: line is in use by a consumer and is not available for new requests.
 * @LI_GPIO_CONFIG_ACTIVE_LOW: line active state is physical low.
 * @LI_GPIO_CONFIG_INPUT: line is an input.
 * @LI_GPIO_CONFIG_OUTPUT: line is an output.
 * @LI_GPIO_CONFIG_EDGE_RISING: line detects rising (inactive to active) edges.
 * @LI_GPIO_CONFIG_EDGE_FALLING: line detects falling (active to inactive) edges.
 * @LI_GPIO_CONFIG_OPEN_DRAIN: line is an open drain output.
 * @LI_GPIO_CONFIG_OPEN_SOURCE: line is an open source output.
 * @LI_GPIO_CONFIG_BIAS_PULL_UP: line has pull-up bias enabled.
 * @LI_GPIO_CONFIG_BIAS_PULL_DOWN: line has pull-down bias enabled.
 * @LI_GPIO_CONFIG_BIAS_DISABLED: line has bias disabled.
 * @LI_GPIO_CONFIG_EVENT_CLOCK_REALTIME: line events contain REALTIME timestamps.
 */
enum li_gpio_config_flags
  {
	LI_GPIO_CONFIG_USED = _BITULL(0),
	LI_GPIO_CONFIG_ACTIVE_LOW = _BITULL(1),
	LI_GPIO_CONFIG_INPUT	= _BITULL(2),
	LI_GPIO_CONFIG_OUTPUT = _BITULL(3),
	LI_GPIO_CONFIG_EDGE_RISING	= _BITULL(4),
	LI_GPIO_CONFIG_EDGE_FALLING = _BITULL(5),
	LI_GPIO_CONFIG_OPEN_DRAIN = _BITULL(6),
	LI_GPIO_CONFIG_OPEN_SOURCE	= _BITULL(7),
	LI_GPIO_CONFIG_BIAS_PULL_UP = _BITULL(8),
	LI_GPIO_CONFIG_BIAS_PULL_DOWN = _BITULL(9),
	LI_GPIO_CONFIG_BIAS_DISABLED	= _BITULL(10),
	LI_GPIO_CONFIG_EVENT_CLOCK_REALTIME	= _BITULL(11),
  };

/**
 * @brief select witch field of #li_lines_config you want to change.
 * @LI_GPIO_LINE_CONFIG_ID_FLAGS: if you want to use the flags field of selected pins
 * @LI_GPIO_LINE_CONFIG_ID_OUTPUT_VALUES: if you want to use the values field of selected output pins.
 * @LI_GPIO_LINE_CONFIG_ID_DEBOUNCE: if you want to use debounce_period_us field.
 */
enum li_gpio_line_config_id
  {
	LI_GPIO_LINE_CONFIG_ID_FLAGS	= 1,
	LI_GPIO_LINE_CONFIG_ID_OUTPUT_VALUES	= 2,
	LI_GPIO_LINE_CONFIG_ID_DEBOUNCE = 3,
  };

/**
 * @brief set flags or values or debounce_period_us of the selected lines with mask field.
 * @id: select what field you want to set on the selected pins. refer to #li_gpio_line_config_id for more information.
 * @mask: a bitmap of selected pins that should be affected by this config.
 * for example 0b01 means that li_gpio_lines_request.lines[0] is affected.
 * 0b11 means that li_gpio_lines_request.lines[0] and li_gpio_lines_request.lines[1] is affected.
 * @flags: if id is set to %LI_GPIO_LINE_CONFIG_ID_FLAGS, set the flags of selected pins.
 * @values: if id is set to %LI_GPIO_LINE_CONFIG_ID_OUTPUT_VALUES, set the values of selected output pins.
 * @debounce_period_us: if id is set to %LI_GPIO_LINE_CONFIG_ID_DEBOUNCE, the desired debounce period, in microseconds
 */
struct li_lines_config
  {
  __u32 id;
  __aligned_u64 mask;
  union
    {
		__aligned_u64 flags;
		__aligned_u64 values;
		__u32 debounce_period_us;
	  };
  };
/**
 * @brief holds the configuration of gpio lines of a request.
 * @flags: flags applied to each lines that are not affected by configs array. refer to #li_gpio_config_flags for more information.
 * @num_configs: number of configs present in configs array.
 * @configs: individual configs that can be applied to some lines. refer to #li_lines_config struct.
 */
struct li_gpio_config 
  {
  __aligned_u64 flags;
  __u32 num_configs;
  struct li_lines_config configs[10];
  };
/**
 * @brief holds the information for a requesting gpio lines.
 * @lines: a array of lines that you want to request. you must remember the ordering of these lines in this array you will controll the lines by indexes of this array.
 * @consumer: a name of the consumer of these lines.
 * @num_lines: nuumber of lines in lines array.
 * @event_buffer_size: a suggested minimum number of line events that the
 * kernel should buffer. This is only relevant if edge detection is
 * enabled in the configuration. Note that this is only a suggested value
 * and the kernel may allocate a larger buffer or cap the size of the
 * buffer. If this field is zero then the buffer size defaults to a minimum
 * of @num_lines * 16.
 * @config: the configuration for the requested lines. refer to #li_gpio_config struct.
 */
struct li_gpio_lines_request
  {
  __u32 lines[64];
  char consumer[32];
  __u32 num_lines;
  __u32 event_buffer_size;
  struct li_gpio_config config;
  };

/**
 * @brief holds info about rqeuested pin values.
 * @mask: a bitmap of selected pins that you want to get or set the values of.
 * for example 0b01 means that li_gpio_lines_request.lines[0] is affected.
 * 0b11 means that li_gpio_lines_request.lines[0] and li_gpio_lines_request.lines[1] is affected.
 * @bits: a bitmap of values. in case of get this field will be filled with values of the pins and in case of set pin values will be set to these values.
 * for example 0b01 means li_gpio_lines_request.lines[0] is 1 and li_gpio_lines_request.lines[1] is 0.
 */
struct li_gpio_line_values
  {
	__aligned_u64 bits;
	__aligned_u64 mask;
  };

/**
 * @brief opens a gpio charecter device for other operations.
 * @note you must close this with #gpio_fd_close after you`re done.
 * @note you can pass the returned fd to these functions #li_gpio_chip_info_get, #li_gpio_line_info_get and li_gpio_lines_request.
 * @param device points to gpiochip char device for example. "/dev/gpiochip0".
 * @param open_mode which mode you want to open the device. O_RDONLY, O_WRONLY or O_RDWR. in fnctl.h.
 * @returns 0 success.
 * @returns -1 failed to open the device.
 */
int gpio_device_open(const char *device, int open_mode);

/**
 * @brief closes a file descriptor.
 * @param fd the file descriptor that you want to close.
 * @returns 0 success.
 * @returns -1 failed to close the fd.
 */
int gpio_fd_close(int fd);

/**
 * @brief get the information about a gpio chip.
 * @param device_fd a fd returned from #gpio_device_open.
 * @param chip_info [out] you can read this field with the chip information in case of success. refer to #li_gpio_chip_info strcut.
 * @returns 0 success.
 * @returns -1 error.
 */
int8_t li_gpio_chip_info_get(int device_fd, struct li_gpio_chip_info *chip_info);

/**
 * @brief get the information about a gpio line.
 * @param device_fd a fd returned from #gpio_device_open.
 * @param offset the line number you want to get information about. can be one of the rpi pins.
 * @param line_info [out] you can read this field with the line inforamtion in case of success. refer to #li_gpio_line_info strcut.
 * @returns 0 success.
 * @returns -1 error.
 */
int8_t li_gpio_line_info_get(int device_fd, uint8_t offset, struct li_gpio_line_info *line_info);

/**
 * @brief request to consume a line.
 * @note on case of success you can pass the returned fd to #gpio_li_lines_config_set, #gpio_lines_value_get, #gpio_lines_value_set and #gpio_lines_value_watch.
 * @param device_fd a fd returned from #gpio_device_open.
 * @param request describes the request. refer to #li_gpio_lines_request struct.
 * @returns -1 in case of error.
 * @returns the requested lines fd in case of success.
 */
int8_t li_gpio_lines_request(int device_fd, struct li_gpio_lines_request *request);

/**
 * @brief set the config for selected lines.
 * @param lines_fd a fd returned from #li_gpio_lines_request.
 * @param config the configuration for the requested lines. refer to #li_gpio_config struct.
 * @returns 0 success.
 * @returns -1 error.
 */
int8_t gpio_li_lines_config_set(int lines_fd, struct li_gpio_config *config);

/**
 * @brief get the values of request lines.
 * @param lines_fd a fd returned from #li_gpio_lines_request.
 * @param line_values [in|out] holds information about the request and response. refer to li_gpio_line_values for more info.
 * @returns 0 success.
 * @returns -1 error.
 */
int8_t gpio_lines_value_get(int lines_fd, struct li_gpio_line_values *line_values);

/**
 * @brief set the values of request lines.
 * @param lines_fd a fd returned from #li_gpio_lines_request.
 * @param line_values [in] holds information about the request. refer to li_gpio_line_values for more info.
 * @returns 0 success.
 * @returns -1 error.
 */
int8_t gpio_lines_value_set(int lines_fd, struct li_gpio_line_values *line_values);

/**
 * @brief blocks until a edge detection event occurs.
 * @note you should close and reopen the lines_fd after each call of this function.
 * @param lines_fd a fd returned from #li_gpio_lines_request.
 * @param timeoutms timeoutms returns after timeout expires (milliseconds).
 * @returns 0 succes.
 * @returns -1 error.
 * @returns -2 timeout.
 */
int8_t gpio_lines_value_watch(int lines_fd, uint32_t timeoutms);