#include <libra/io/gpio-chardev.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/poll.h>

#include "internal/gpio.h"

int gpio_device_open(const char *device, int open_mode)
  {
  int fd = open(device, open_mode);
  if (fd < 0)
    return -1;
  return fd;
  }

int gpio_fd_close(int fd)
  {
  if (close(fd) < 0)
    return -1;
  return 0;
  }

int8_t li_gpio_chip_info_get(int device_fd, struct li_gpio_chip_info *chip_info)
  {
  struct gpiochip_info chip;
  memset(&chip, 0, sizeof(struct gpiochip_info));
  if (ioctl(device_fd, GPIO_GET_CHIPINFO_IOCTL, &chip) < 0)
    {
    return -1;
    }
  else
    {
    memcpy(chip_info->name, chip.name, GPIO_MAX_NAME_SIZE);
    memcpy(chip_info->label, chip.label, GPIO_MAX_NAME_SIZE);
    chip_info->lines = chip.lines;
    return 0;
    }
  }

int8_t li_gpio_line_info_get(int device_fd, uint8_t offset, struct li_gpio_line_info *line_info)
  {
  struct gpio_v2_line_info line;
  memset(&line, 0, sizeof(struct gpio_v2_line_info));
  line.offset = offset;
  if (ioctl(device_fd, GPIO_V2_GET_LINEINFO_IOCTL, &line) < 0)
    {
    return -1;
    }
  else
    {
    memcpy(line_info->name, line.name, GPIO_MAX_NAME_SIZE);
    memcpy(line_info->consumer, line.consumer, GPIO_MAX_NAME_SIZE);
    line_info->flags = line.flags;
    return 0;
    }
  }

void __copy_gpio_config(struct gpio_v2_line_config *dest, struct li_gpio_config *src)
  {
  dest->flags = src->flags;
  dest->num_attrs = src->num_configs;
  for (int i = 0; i < dest->num_attrs; i++)
    {
    dest->attrs[i].mask = src->configs[i].mask;
    dest->attrs[i].attr.id = src->configs[i].id;
    switch (src->configs[i].id)
      {
      case LI_GPIO_LINE_CONFIG_ID_FLAGS: dest->attrs[i].attr.flags = src->configs[i].flags; break;
      case LI_GPIO_LINE_CONFIG_ID_OUTPUT_VALUES: dest->attrs[i].attr.values = src->configs[i].values; break;
      case LI_GPIO_LINE_CONFIG_ID_DEBOUNCE: dest->attrs[i].attr.debounce_period_us = src->configs[i].debounce_period_us; break;
      }
    }
  }

int8_t li_gpio_lines_request(int device_fd, struct li_gpio_lines_request *request)
  {
  struct gpio_v2_line_request line_req;
  struct li_gpio_lines_request req = *request;
  memset(&line_req, 0, sizeof(struct gpio_v2_line_request));
  memcpy(line_req.offsets, request->lines, sizeof(uint32_t) * req.num_lines);
  memcpy(line_req.consumer, req.consumer, GPIO_MAX_NAME_SIZE);
  line_req.num_lines = req.num_lines;
  line_req.event_buffer_size = req.event_buffer_size;
  __copy_gpio_config(&line_req.config, &req.config);
  if (ioctl(device_fd, GPIO_V2_GET_LINE_IOCTL, &line_req) < 0)
    return -1;
  else
    return line_req.fd;
  }

int8_t gpio_li_lines_config_set(int lines_fd, struct li_gpio_config *config)
  {
  struct gpio_v2_line_config conf;
  memset(&conf, 0, sizeof(struct gpio_v2_line_config));
  __copy_gpio_config(&conf, config);
  if (ioctl(lines_fd, GPIO_V2_LINE_SET_CONFIG_IOCTL, &conf) < 0)
    return -1;
  else
    return 0;
  }

int8_t gpio_lines_value_get(int lines_fd, struct li_gpio_line_values *line_values)
  {
  struct gpio_v2_line_values values;
  memset(&values, 0, sizeof(struct gpio_v2_line_values));
  values.mask = line_values->mask;
  if (ioctl(lines_fd, GPIO_V2_LINE_GET_VALUES_IOCTL, &values) < 0)
    {
    return -1;
    }
  else
    {
    line_values->bits = values.bits;
    return 0;
    }
  }

int8_t gpio_lines_value_set(int lines_fd, struct li_gpio_line_values *line_values)
  {
  struct gpio_v2_line_values values;
  memset(&values, 0, sizeof(struct gpio_v2_line_values));
  values.mask = line_values->mask;
  values.bits = line_values->bits;
  if (ioctl(lines_fd, GPIO_V2_LINE_SET_VALUES_IOCTL, &values) < 0)
    return -1;
  else
    return 0;
  }

int8_t gpio_lines_value_watch(int lines_fd, uint32_t timeoutms)
  {
  struct pollfd pfd[1];
  memset(&pfd, 0, sizeof(struct pollfd) * 1);
  pfd[0].fd = lines_fd;
  pfd[0].events = POLLIN | POLLPRI;
  int ret = poll(pfd, 1, timeoutms);
  if (ret < 0)
    return -1;
  if (ret == 0)
    return -2;
  if (pfd[0].revents & POLLIN)
    return 0;
  return -1;
  }
