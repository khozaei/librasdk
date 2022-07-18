#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <io/gpio-chardev.h>
#include <io/rpi/pins.h>

#define GPIO_EXAMPLE_CHIP_NAME "/dev/gpiochip0"


int main()
  {
  int device_fd = gpio_device_open(GPIO_EXAMPLE_CHIP_NAME, O_RDWR);
  if (device_fd < 0)
    {
    printf("failed to open the device\n");
    return -1;
    }
  struct li_gpio_lines_request req;
  memset(&req, 0, sizeof(struct li_gpio_lines_request));
  strcpy(req.consumer, "example app");
  req.num_lines = 3;
  req.lines[0] = LI_GPIO_17;
  req.lines[1] = LI_GPIO_27;
  req.lines[2] = LI_GPIO_22;
  req.config.flags = LI_GPIO_CONFIG_OUTPUT;
  req.config.num_configs = 1;
  req.config.configs[0].id = LI_GPIO_LINE_CONFIG_ID_FLAGS;
  req.config.configs[0].mask = 0b010; // it only effects the req.lines[1] line
  req.config.configs[0].flags = LI_GPIO_CONFIG_INPUT | LI_GPIO_CONFIG_BIAS_PULL_DOWN;
  int lines_fd = li_gpio_lines_request(device_fd, &req);
  if (lines_fd < 0)
    {
    printf("failed to request the lines\n");
    gpio_fd_close(device_fd);
    return -1;
    }
  struct li_gpio_line_values values;
  memset(&values, 0, sizeof(struct li_gpio_line_values));
  values.mask = 0b101; // it only affects the req.lines[0] and req.lines[2] lines
  values.bits = 0b100; // value for req.lines[0] is 0 and for req.lines[2] is 1
  if (gpio_lines_value_set(lines_fd, &values) < 0)
    {
    printf("failed to set");
    gpio_fd_close(device_fd);
    gpio_fd_close(lines_fd);
    return -1;
    }
  values.mask = 0b111; // it affects all lines
  if (gpio_lines_value_get(lines_fd, &values) < 0)
    {
    printf("failed to get");
    gpio_fd_close(device_fd);
    gpio_fd_close(lines_fd);
    return -1;
    }
  printf("values.bits: %llu\n", values.bits);
  values.mask = 0b001; // it only affects the req.lines[0] line
  values.bits = 0b001;
  if( gpio_lines_value_set(lines_fd, &values) < 0)
    {
    printf("failed to set");
    gpio_fd_close(device_fd);
    gpio_fd_close(lines_fd);
    return -1;
    }
  values.mask = 0b111; // it affects all lines
  if (gpio_lines_value_get(lines_fd, &values) < 0)
    {
    printf("failed to read");
    gpio_fd_close(device_fd);
    gpio_fd_close(lines_fd);
    return -1;
    }
  printf("values.bits: %llu\n", values.bits);
  gpio_fd_close(device_fd);
  return 0;
  }