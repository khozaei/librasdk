#include <io/gpio-chardev.h>

#define GPIO_EXAMPLE_CHIP_NAME "/dev/gpiochip0"

int main()
  {
  int device_fd = gpio_device_open(GPIO_EXAMPLE_CHIP_NAME, O_RDWR);
  if (device_fd < 0)
    return -1;
  struct li_gpio_chip_info info;
  if (li_gpio_chip_info_get(device_fd, &info) < 0)
    {
    gpio_fd_close(device_fd);
    return -1;
    }
  if (gpio_fd_close(device_fd) < 0)
    {
    return -1;
    }
  return 0;
  }