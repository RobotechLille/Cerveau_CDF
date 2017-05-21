#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "pince.h"
#include "serial.h"

int main() {
    printf("Hello world!\n");
    int fd = serialOpen("com/ardPince", SERIAL_BOTH);
    serialConfig(fd, B9600);
    char a = 0x00;
    write(fd, &a, 1);
    serialClose(fd);
}
