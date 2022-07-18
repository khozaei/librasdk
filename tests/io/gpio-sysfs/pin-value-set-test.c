#include <unistd.h>
#include <fcntl.h>
#include <io/gpio-sysfs.h>
#include <io/rpi/pins.h>
#include <string.h>

#include "utils.c"

int main()
  {
  open_p(LI_GPIO_2);
  set_dir_p(LI_GPIO_2, LI_DIRECTION_OUTPUT);
  if (pin_value_set(LI_GPIO_2, LI_VALUE_HIGH) == 0)
    {
    int v;
    get_value_p(LI_GPIO_2, &v);
    close_p(LI_GPIO_2);
    if(v == LI_VALUE_HIGH)
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