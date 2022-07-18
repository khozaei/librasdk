#include <unistd.h>
#include <fcntl.h>
#include <io/gpio-sysfs.h>
#include <io/rpi/pins.h>

#include "utils.c"

int main()
  {
  open_p(LI_GPIO_2);
  if (pin_close(LI_GPIO_2) == 0)
    {
    if(access("/sys/class/gpio/gpio2/value", F_OK ) == 0)
      return 1;
    else
      return 0;
    }
  else
    {
    return 1;
    }
  }