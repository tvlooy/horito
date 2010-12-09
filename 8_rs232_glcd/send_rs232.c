/**
 * Linux x86
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * This program sends a text to a remote system over RS232
 */

#include <stdio.h>   /* Standard input/output definitions  */
#include <string.h>  /* String function definitions        */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions           */
#include <errno.h>   /* Error number definitions           */
#include <termios.h> /* POSIX terminal control definitions */

int main(void)
{
    // Open the serial port
    // O_NOCTTY = no be the controlling terminal
    // O_NDELAY = no carrier detect
    int fd;
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1) {
        perror("open_port: Unable to open /dev/ttyS0 - ");
        return -1;
    }

    // set communication options
    struct termios options;
    tcgetattr(fd, &options);             /* get current options            */
    cfsetispeed(&options, B9600);        /* set input baud rate            */
    cfsetospeed(&options, B9600);        /* set output baud rate           */
    options.c_cflag |= (CLOCAL | CREAD); /* CLOCAL = local line 
                                          *          do not change "owner"
                                          * CREAD = enable receiver        */
    tcsetattr(fd, TCSANOW, &options);    /* set new options
                                          * TCSANOW = immediately          */

    // write the data to the remote system
    // usleep between characters because the test program
    // doesn't have buffering yet
    char value[] = "Tom Van Looy\rctors.net\r\r<tom@ctors.net>";
    char send[2] = "";
    int i;
    sprintf(send, "%c", 27); // esc (clears the screen)
    for (i = 0; i < strlen(value)+1; i++) {
        write(fd, send, 1);
        sprintf(send, "%c", value[i]);
        usleep(40000);
    }

    close(fd);
    return 0;
}

