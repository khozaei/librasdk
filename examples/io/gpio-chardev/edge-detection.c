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

int main() {
  int device_fd = gpio_device_open(GPIO_EXAMPLE_CHIP_NAME, O_RDWR);
  if (device_fd < 0)
    {
    printf("failed to open the device\n");
    return -1;
    }
  struct li_gpio_lines_request req;
  memset(&req, 0, sizeof(struct li_gpio_lines_request));
  req.num_lines = 1;
  req.lines[0] = LI_GPIO_27;
  req.config.flags = LI_GPIO_CONFIG_INPUT | LI_GPIO_CONFIG_EDGE_RISING | LI_GPIO_CONFIG_EDGE_FALLING;
  struct li_gpio_line_values values;
  memset(&values, 0, sizeof(struct li_gpio_line_values));
  int lines_fd = li_gpio_lines_request(device_fd, &req);
  if (lines_fd < 0)
    {
    printf("failed to request the lines");
    return -1;
    }
  while(1)
    {
    lines_fd = li_gpio_lines_request(device_fd, &req);
    if (lines_fd < 0)
      {
      printf("failed to request the lines");
      return -1;
      }
    printf("waiting...\n");
    if (!gpio_lines_value_watch(lines_fd, -1))
      {
      values.mask = 0b1;
      if (gpio_lines_value_get(lines_fd, &values) < 0)
        {
        printf("failed to get value\n");
        gpio_fd_close(lines_fd);
        return -1;
        }
      printf("values.bits: %llu\n", values.bits);
      gpio_fd_close(lines_fd);
      }
    else
      {
      printf("failed to poll\n");
      gpio_fd_close(lines_fd);
      return -1;
      }
    }
  gpio_fd_close(device_fd);
  return -1;
}