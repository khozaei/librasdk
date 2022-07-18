#include <unistd.h>
#include <fcntl.h>
#include <io/gpio-sysfs.h>
#include <io/rpi/pins.h>

#include "utils.c"

int main()
  {
  open_p(LI_GPIO_2);
  int result = pin_edge_exists(LI_GPIO_2);
  close_p(LI_GPIO_2);
  return result;
  }