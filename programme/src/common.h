#ifndef __COMMON_H_
#define __COMMON_H_

#include <stdio.h>   // printf...
#include <stdlib.h>  // Des trucs
#include <fcntl.h>   // O_*
#include <unistd.h>  // read(), write()
#include <termios.h> // Baudrate

#define SIGNAL_PLS 127

int openRetry(const char *path, int flags);
void serialConfig(int fd, int speed);


#endif

