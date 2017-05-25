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

int openArduino(const char* path)
{
    printf("Connexion à %s...\n", path);
    int ard = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
    if (ard < 0) {
        printf("Échec !\n");
        exit(1);
    }
    serialConfig(ard, B9600);
    sleep(1);
    printf("OK !\n");
    return ard;
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

void floatToStr(float f, char r[8])
{
    char s[255];
    sprintf(s, "%8f", f);
    int i;
    for (i = 0; i < 8; i++) {
        r[i] = s[i];
    }
}

void sendChar(int ard, char c)
{
    write(ard, &c, 1);
}

void sendFloat(int ard, float f)
{
    char consigne[8];
    floatToStr(f, consigne);
    write(ard, &consigne, 8);
}

char readChar(int ard)
{
    char c;
    while (read(ard, &c, 1) <= 0)
    {
        continue;
    }
    return c;
}

float readFloat(int ard)
{
    char retour[9];
    int i;
    for (i = 0; i < 8; i++) {
        retour[i] = readChar(ard);
    }
    retour[8] = '\0';
    return atof(retour);
}

