#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <io/gpio-sysfs.h>

int pin_open(int pin)
  {
  int fd = open("/sys/class/gpio/export", O_WRONLY);
  if (fd == -1)
    return -1;
  char buffer[5];
  sprintf(buffer, "%d", pin);
  int size = strlen(buffer);
  if (write(fd, buffer, size) != size)
    {
    close(fd);
    return -2;
    }

  close(fd);
  return 0;
  }

int pin_close(int pin)
  {
  int fd = open("/sys/class/gpio/unexport", O_WRONLY);
  if (fd == -1)
    return -1;
  char buffer[5];
  sprintf(buffer, "%d", pin);
  int size = strlen(buffer);
  if (write(fd, buffer, size) != size)
    {
    close(fd);
    return -2;
    }
  close(fd);
  return 0;
  }

int pin_direction_set(int pin, int dir)
  {
  char a[33];
  sprintf(a, "/sys/class/gpio/gpio%d/direction", pin);
  int fd = open(a, O_WRONLY);
  if (fd == -1)
    return -1;
  char dirBuffer[5];
  int len;
  if (dir == LI_DIRECTION_INPUT)
    {
    strcpy(dirBuffer, "in");
    len = 2;
    }
  else
    {
    strcpy(dirBuffer, "out");
    len = 3;
    }
  if (write(fd, dirBuffer, len) != len)
    {
    close(fd);
    return -2;
    }
  close(fd);
  return 0;
  }

int pin_direction_get(int pin, int* dir)
  {
  char a[33];
  sprintf(a, "/sys/class/gpio/gpio%d/direction", pin);
  int fd = open(a, O_RDONLY);
  if (fd == -1)
    return -1;
  char buffer[5];
  ssize_t r = read(fd, buffer, 5);
  if (r <= 0)
    {
    close(fd);
    return -2;
    }
  *dir =  buffer[0] == 'i' ? LI_DIRECTION_INPUT : LI_DIRECTION_OUTPUT;
  close(fd);
  return 0;
  }

int pin_value_set(int pin, int value)
  {
  char a[33];
  sprintf(a, "/sys/class/gpio/gpio%d/value", pin);
  int fd = open(a, O_WRONLY);
  if (fd == -1)
    return -1;
  char buffer[2];
  sprintf(buffer, "%d", value);
  int size = strlen(buffer);
  if (write(fd, buffer, size) != size)
    {
    close(fd);
    return -2;
    }
  close(fd);
  return 0;
  }

int pin_value_get(int pin, int* value)
  {
  char a[33];
  sprintf(a, "/sys/class/gpio/gpio%d/value", pin);
  int fd = open(a, O_RDONLY);
  if (fd == -1)
    return -1;
  char buffer[5];
  if (read(fd, buffer, 5) <= 0)
    {
    close(fd);
    return -2;
    }
  *value = buffer[0] == '0' ? 0 : 1;
  close(fd);
  return 0;
  }

int pin_value_toggle(int pin)
  {
  char a[33];
  sprintf(a, "/sys/class/gpio/gpio%d/value", pin);
  int fd = open(a, O_RDWR);
  if (fd == -1)
    return -1;
  char buffer[5];
  if (read(fd, buffer, 5) == 0)
    {
    close(fd);
    return -2;
    }
  if (write(fd, buffer[0] == '0' ? "1" : "0", 1) != 1)
    {
    close(fd);
    return -2;
    }
  close(fd);
  return 0;
  }

int pin_edge_exists(int pin)
  {
  char buff[40];
  sprintf(buff, "/sys/class/gpio/gpio%d/edge", pin);
  return access(buff, F_OK);
  }

int pin_edge_set(int pin, int edge)
  {
  char a[33];
  sprintf(a, "/sys/class/gpio/gpio%d/edge", pin);
  int fd = open(a, O_WRONLY);
  if (fd == -1)
    return -1;
  char buffer[10];
  switch (edge)
    {
    case LI_EDGE_NONE: strcpy(buffer, "none"); break;
    case LI_EDGE_RISING: strcpy(buffer, "rising"); break;
    case LI_EDGE_FALLING: strcpy(buffer, "falling"); break;
    case LI_EDGE_BOTH: strcpy(buffer, "both"); break;
    default: close(fd); return -3;
    }
  int size = strlen(buffer);
  if (write(fd, buffer, size) != size)
    {
    close(fd);
    return -2;
    }
  close(fd);
  return 0;
  }

int pin_edge_get(int pin, int* edge)
  {
  char a[33];
  sprintf(a, "/sys/class/gpio/gpio%d/edge", pin);
  int fd = open(a, O_RDONLY);
  if (fd == -1)
    return -1;
  char buff[10];
  ssize_t r = read(fd, buff, 10);
  if (r <= 0)
    {
    close(fd);
    return -2;
    }
  switch (buff[0])
    {
    case 'n': *edge = LI_EDGE_NONE; break;
    case 'r': *edge = LI_EDGE_RISING; break;
    case 'f': *edge = LI_EDGE_FALLING; break;
    case 'b': *edge = LI_EDGE_BOTH; break;
    default: close(fd); return -3;
    }
  close(fd);
  return 0;
  }

int pin_value_get_evented(int pin, int* value, int timeoutms)
  {
  char a[33];
  sprintf(a, "/sys/class/gpio/gpio%d/value", pin);
  int fd = open(a, O_RDONLY);
  if (fd == -1)
    return -1;
  char buffer[5];
  if (read(fd, buffer, 5) <= 0)
    {
    close(fd);
    return -2;
    }
  struct pollfd fds[1];
  fds[0].fd = fd;
  fds[0].events = POLLPRI | POLLERR;
  fds[0].revents = 0;
  int result = poll(fds, 1, timeoutms);
  if (result == 0) {
    close(fd);
    return -3;
  }
  lseek(fd, 0, SEEK_SET);
  if (read(fd, buffer, 5) <= 0)
    {
    close(fd);
    return -2;
    }
  *value = buffer[0] == '0' ? 0 : 1;
  close(fd);
  return 0;
  }