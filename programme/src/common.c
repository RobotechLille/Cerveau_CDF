#include "common.h"

// serialConfig
#include <linux/serial.h>
#include <strings.h>
#include <sys/ioctl.h> //ioctl() call defenitions

// TIP Toujours ouvrir les pipes en lecture en premier
int openRetry(const char* path, int flags)
{
    int fd;
    do {
        fd = open(path, flags);
    } while (fd < 0);
    return fd;
}

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
        perror("serialConfig.tcsetattr");
        exit(1);
    }
}

// TODO DTR ← 0; sleep period; DTR ← 1
// void resetArduino(const char* path)
// {
//     int fd;
//     fd = open(path, O_RDWR | O_NOCTTY); //Open Serial Port
//     serialConfig(fd, 115200);
// 
//     int RTS_flag;
//     RTS_flag = TIOCM_RTS;
//     ioctl(fd, TIOCMBIS, &RTS_flag); //Set RTS pin
//     getchar();
//     ioctl(fd, TIOCMBIC, &RTS_flag); //Clear RTS pin
//     close(fd);
// }
