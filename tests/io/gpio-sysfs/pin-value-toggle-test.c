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
  set_value_p(LI_GPIO_2, LI_VALUE_HIGH);
  if (pin_value_toggle(LI_GPIO_2) == 0)
    {
    int a;
    get_value_p(LI_GPIO_2, &a);
    close_p(LI_GPIO_2);
    if(a == 0)
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