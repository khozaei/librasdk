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

void print_info__(struct li_gpio_line_info info)
  {
  printf("name: %s, consumer: %s, flags: %u, is_output: %d, is_input: %d\n", info.name, info.consumer, info.flags, (info.flags & LI_GPIO_CONFIG_OUTPUT) ? 1 : 0, (info.flags & LI_GPIO_CONFIG_INPUT) ? 1 : 0);
  }

int main()
  {
  int device_fd = gpio_device_open(GPIO_EXAMPLE_CHIP_NAME, O_RDWR);
  if (device_fd < 0)
    {
    printf("failed to open the device\n");
    return -1;
    }
  struct li_gpio_line_info info;
  li_gpio_line_info_get(device_fd, LI_GPIO_2, &info);
  print_info__(info);
  gpio_fd_close(device_fd);
  return 0;
  }