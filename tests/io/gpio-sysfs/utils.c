#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <io/gpio-sysfs.h>

void open_p(int pin)
  {
  int fd = open("/sys/class/gpio/export", O_WRONLY);
  char buff[5];
  sprintf(buff, "%d", pin);
  write(fd, buff, strlen(buff));
  close(fd);
  }

void close_p(int pin)
  {
  int fd = open("/sys/class/gpio/unexport", O_WRONLY);
  char buff[5];
  sprintf(buff, "%d", pin);
  write(fd, buff, strlen(buff));
  close(fd);
  }

void set_dir_p(int pin, int dir)
  {
  char buff[50];
  sprintf(buff, "/sys/class/gpio/gpio%d/direction", pin);
  int fd = open(buff, O_WRONLY);
  char buffer[5];
  if (dir == LI_DIRECTION_INPUT)
    strcpy(buffer, "in");
  else
    strcpy(buffer, "out");
  write(fd, buffer, strlen(buffer));
  close(fd);
  }

void get_dir_p(int pin, int* dir)
  {
  char buff[50];
  sprintf(buff, "/sys/class/gpio/gpio%d/direction", pin);
  int fd = open(buff, O_RDONLY);
  char buffer[5];
  int r = read(fd, buffer, 5);
  *dir = buffer[0] == 'i' ? LI_DIRECTION_INPUT : LI_DIRECTION_OUTPUT;
  close(fd);
  }

void get_value_p(int pin, int* val)
  {
  char buff[50];
  sprintf(buff, "/sys/class/gpio/gpio%d/value", pin);
  int fd = open(buff, O_RDONLY);
  char buffer[5];
  read(fd, buffer, 5);
  *val = buffer[0] == '0' ? LI_VALUE_LOW : LI_VALUE_HIGH;
  close(fd);
  }

void set_value_p(int pin, int val)
  {
  char buff[50];
  sprintf(buff, "/sys/class/gpio/gpio%d/value", pin);
  int fd = open(buff, O_WRONLY);
  char buffer[2];
  sprintf(buffer, "%d", val);
  write(fd, buffer, 1);
  close(fd);
  }

void get_edge_p(int pin, int* edge)
  {
  char buff[50];
  sprintf(buff, "/sys/class/gpio/gpio%d/edge", pin);
  int fd = open(buff, O_RDONLY);
  char buffer[10];
  int r = read(fd, buffer, 10);
  switch(buffer[0])
    {
    case 'n': *edge = LI_EDGE_NONE; break;
    case 'r': *edge = LI_EDGE_RISING; break;
    case 'f': *edge = LI_EDGE_FALLING; break;
    case 'b': *edge = LI_EDGE_BOTH; break;
    }
  close(fd);
  }

void set_edge_p(int pin, int edge)
  {
  char buff[50];
  sprintf(buff, "/sys/class/gpio/gpio%d/edge", pin);
  int fd = open(buff, O_WRONLY);
  char buffer[10];
  switch(edge)
    {
    case LI_EDGE_NONE: strcpy(buffer, "none"); break;
    case LI_EDGE_RISING: strcpy(buffer, "rising"); break;
    case LI_EDGE_FALLING: strcpy(buffer, "falling"); break;
    case LI_EDGE_BOTH: strcpy(buffer, "both"); break;
    }
  write(fd, buffer, strlen(buffer));
  close(fd);
  }