#ifndef __COMMON_H_
#define __COMMON_H_

#include <stdio.h>   // printf...
#include <stdlib.h>  // Des trucs
#include <fcntl.h>   // O_*
#include <unistd.h>  // read(), write()
#include <termios.h> // Baudrate

#define SIGNAL_PLS 127

// Com tout et n'importe quoi
int openRetry(const char *path, int flags);
void serialConfig(int fd, int speed);

// COM ARDUINO
int openArduino(const char *path);
void floatToStr(float f, char r[8]);
void sendChar(int ard, char c);
void sendFloat(int ard, float f);
char readChar(int ard);
float readFloat(int ard);

#endif

