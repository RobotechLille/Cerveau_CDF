/*
 * Serial library
 * Code écrit par Xavier Redon
 */

////
// Include files
////
#include <fcntl.h>
#include <linux/serial.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "serial.h"

////
// Functions
////

//
// Open serial port device
//
int serialOpen(char* device, int mode)
{
    int flags = (mode == SERIAL_READ ? O_RDONLY : (mode == SERIAL_WRITE ? O_WRONLY : O_RDWR));
    int fd = open(device, flags | O_NOCTTY);
    if (fd < 0) {
        perror(device);
        exit(-1);
    }
    return fd;
}

//
// Serial port configuration
//
void serialConfig(int fd, int speed)
{
    struct termios new;
    bzero(&new, sizeof(new));
    new.c_cflag = CLOCAL | CREAD | speed | CS8;
    new.c_iflag = 0;
    new.c_oflag = 0;
    new.c_lflag = 0;     /* set input mode (non-canonical, no echo,...) */
    new.c_cc[VTIME] = 0; /* inter-character timer unused */
    new.c_cc[VMIN] = 1;  /* blocking read until 1 char received */
    if (tcsetattr(fd, TCSANOW, &new) < 0) {
        perror("serialInit.tcsetattr");
        exit(-1);
    }
}

//
// Serial port termination
//
void serialClose(int fd)
{
    close(fd);
}
