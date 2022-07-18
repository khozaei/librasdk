#include <io/gpio-chardev.h>
#include <io/rpi/pins.h>

#define GPIO_EXAMPLE_CHIP_NAME "/dev/gpiochip0"

int main()
  {
  int device_fd = gpio_device_open(GPIO_EXAMPLE_CHIP_NAME, O_RDWR);
  if (device_fd < 0)
    return -1;
  struct li_gpio_lines_request req;
  req.lines[0] = LI_GPIO_2;
  req.num_lines = 1;
  req.config.flags = LI_GPIO_CONFIG_OUTPUT;
  req.config.num_configs = 0;
  int lines_fd = li_gpio_lines_request(device_fd, &req);
  if (lines_fd < 0)
    {
    gpio_fd_close(device_fd);
    return  -1;
    }
  struct li_gpio_line_values values;
  values.mask = 1;
  values.bits = 1;
  if (gpio_lines_value_set(lines_fd, &values) < 0)
    {
    gpio_fd_close(lines_fd);
    gpio_fd_close(device_fd);
    return -1;
    }
  values.bits = 0;
  if (gpio_lines_value_get(lines_fd, &values) < 0)
    {
    gpio_fd_close(lines_fd);
    gpio_fd_close(device_fd);
    return -1;
    }
  int r;
  if (values.bits == 1)
    r = 0;
  else
    r = -1;
  gpio_fd_close(lines_fd);
  gpio_fd_close(device_fd);
  return r;
  }