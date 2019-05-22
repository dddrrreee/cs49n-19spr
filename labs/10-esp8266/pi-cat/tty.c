#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>

#include "demand.h"

#include "tty.h"

// XXX: if anyone figures out a cleaner way to do this, lmk.   I don't
// have a mac, so stopped as soon as we had something that worked on 
// linux & macos.
// params:
//	- <timeout> is in seconds (< 1 ok)
// 	- <speed> is baud rate.
int set_tty_to_8n1(int fd, unsigned speed, double timeout) {
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0)
        panic("tcgetattr failed\n");
    memset (&tty, 0, sizeof tty);

    // https://github.com/rlcheng/raspberry_pi_workshop
    cfsetspeed(&tty, speed);

    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars

    // XXX: wait, does this disable break or ignore-ignore break??
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    assert(timeout < 100 && timeout > 0);
    // 0.1 seconds read timeout
    tty.c_cc[VTIME] = (int)timeout *10;            

	/*
	 * Setup TTY for 8n1 mode, used by the pi UART.
	 */

    // Disables the Parity Enable bit(PARENB),So No Parity 
    tty.c_cflag &= ~PARENB; 	
    // CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit 
    tty.c_cflag &= ~CSTOPB;   	
    // Clears the mask for setting the data size     
    tty.c_cflag &= ~CSIZE;	 	
    // Set the data bits = 8
    tty.c_cflag |=  CS8; 		
    // No Hardware flow Control 
    tty.c_cflag &= ~CRTSCTS;
    // Enable receiver,Ignore Modem Control lines 
    tty.c_cflag |= CREAD | CLOCAL; 	
    	
    // Disable XON/XOFF flow control both i/p and o/p
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);    
    // Non Cannonical mode 
    tty.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  
    // No Output Processing
    tty.c_oflag &= ~OPOST;	

    if(tcsetattr (fd, TCSANOW, &tty) != 0)
        panic("tcsetattr failed\n");
    return fd;
}

#define _SVID_SOURCE
#include <dirent.h>
static const char *ttyusb_prefixes[] = {
	"ttyUSB",	// linux
	"cu.SLAB_USB", // mac os
	0
};

static int filter(const struct dirent *d) {
	for(const char **p = ttyusb_prefixes; *p; p++)
		if(strncmp(*p,d->d_name, strlen(*p)) == 0)
			return 1;
	return 0;
}
// open the TTY-usb device:
//	- use <scandir> to find a device with a prefix given by ttyusb_prefixes
//	- returns an open fd to it
// 	- write the absolute path into <pathname> if it wasn't already
//	  given.
int open_tty(const char **portname) {
	const char *p = *portname;

	int fd = -1;

	// was given on the command line, just try to open it.
	if(p) {
        	fd = open(p, O_RDWR | O_NOCTTY | O_SYNC);
		if(fd < 0)
			panic("couldn't open tty port <%s>\n", p);
		return fd;
	}

	// otherwise search in /dev for a tty device.
	struct dirent **namelist;
	int n;
	n = scandir("/dev", &namelist, filter, alphasort);
	if(n < 0)
		perror("scandir");
	if(n == 0)
		panic("did not find any tty in /dev\n");
	if(n > 1) {
		fprintf(stderr, "found more than one tty?\n");
               	while (n--)
               		fprintf(stderr, "%s\n", namelist[n]->d_name);
		exit(1);
	}

	char buf[1024];
	sprintf(buf, "/dev/%s", namelist[0]->d_name);
	p = strdup(buf);
	fprintf(stderr, "FOUND: <%s>\n", p);
	// exit(1);

	fd = open(p, O_RDWR | O_NOCTTY | O_SYNC);
       	if(fd < 0)
       		panic("opening dev <%s>\n", p);

	*portname = p;
	return fd;
}
