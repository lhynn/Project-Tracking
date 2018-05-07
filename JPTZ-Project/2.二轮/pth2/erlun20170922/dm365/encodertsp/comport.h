#ifndef COM_H
#define COM_H

#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


int  set_com_config(int fd, int baud_rate, int data_bits, char parity, int stop_bits);
int  open_port(const char* dev_path);
void clear_send(int fd);
int  check_port_open(const char *dev,unsigned baud);
int  open_gpio(const char *dev);

#endif // COM_H
