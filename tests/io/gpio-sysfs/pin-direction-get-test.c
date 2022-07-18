#include <unistd.h>
#include <fcntl.h>
#include <io/gpio-sysfs.h>
#include <io/rpi/pins.h>
#include <string.h>

#include "utils.c"

int main()
  {
  open_p(LI_GPIO_2);
  set_dir_p(LI_GPIO_2, LI_DIRECTION_INPUT);
  int dir;
  if (pin_direction_get(LI_GPIO_2, &dir) == 0)
    {
    close_p(LI_GPIO_2);
    if (dir == LI_DIRECTION_INPUT)
      return 0;
    else
      return 1;
    }
  else
    {
    close_p(LI_GPIO_2);
    return 1;
    }
  }