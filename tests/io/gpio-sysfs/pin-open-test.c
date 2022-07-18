#include <unistd.h>
#include <fcntl.h>
#include <io/gpio-sysfs.h>
#include <io/rpi/pins.h>

#include "utils.c"

int main()
  {
  if (pin_open(LI_GPIO_2) == 0)
    {
    if(access("/sys/class/gpio/gpio2/value", F_OK ) == 0)
      {
      close_p(LI_GPIO_2);
      return 0;
      }
    else
      {
      return 1;
      }
    }
  else
    {
    return 1;
    }
  }