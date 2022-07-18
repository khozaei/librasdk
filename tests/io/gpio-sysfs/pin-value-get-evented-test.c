#include <unistd.h>
#include <fcntl.h>
#include <io/gpio-sysfs.h>
#include <io/rpi/pins.h>
#include <pthread.h>

#include "utils.c"

void *threadFN()
  {
  usleep(100);
  set_value_p(LI_GPIO_2, LI_VALUE_HIGH);
  return NULL;
  }

int main()
  {
  open_p(LI_GPIO_2);
  set_edge_p(LI_GPIO_2, LI_EDGE_BOTH);
  set_value_p(LI_GPIO_2, LI_VALUE_LOW);
  pthread_t thread_id;
  pthread_create(&thread_id, NULL, threadFN, NULL);
  int value;
  if (pin_value_get_evented(LI_GPIO_2, &value, 1000) >= 0)
    {
    close_p(LI_GPIO_2);
    if (value == 1)
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