#include <unistd.h>
#include <fcntl.h>
#include <io/gpio-sysfs.h>
#include <io/rpi/pins.h>
#include <string.h>
#include <stdio.h>

#include "utils.c"

int main()
  {
  open_p(LI_GPIO_2);
  int t = pin_direction_set(LI_GPIO_2, LI_DIRECTION_OUTPUT);
  if (t == 0)
    {
    int read_dir;
    get_dir_p(LI_GPIO_2, &read_dir);
    close_p(LI_GPIO_2);
    if (read_dir == LI_DIRECTION_OUTPUT)
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