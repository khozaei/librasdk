#include <unistd.h>
#include <fcntl.h>
#include <io/gpio-sysfs.h>
#include <io/rpi/pins.h>

#include "utils.c"

int main()
  {
  open_p(LI_GPIO_2);
  if (pin_edge_set(LI_GPIO_2, LI_EDGE_RISING) == 0)
    {
    int edge;
    get_edge_p(LI_GPIO_2, &edge);
    close_p(LI_GPIO_2);
    if (edge == LI_EDGE_RISING)
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