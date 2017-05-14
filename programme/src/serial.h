/*
 * Public definitions for serial library
 * Code écrit par Xavier Redon
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__

////
// Constants
////

#define SERIAL_READ 0
#define SERIAL_WRITE 1
#define SERIAL_BOTH 2

////
// Public prototypes
////
int serialOpen(char* device, int mode);
void serialConfig(int fd, int speed);
void serialClose(int fd);

#endif

