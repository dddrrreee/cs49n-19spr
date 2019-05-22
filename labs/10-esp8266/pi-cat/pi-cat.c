// engler
// Simple echo program to send/receive with the pi.
//
// You shouldn't have to modify any code in this file.  Though, if you find
// a bug or improve it, let me know!
//
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

#if 1

void echo(int fd, const char *portname) {
    while(1) {
        // must reset rfds and tv each time.
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        FD_SET(fd, &rfds);

        // setting tv_sec = tv_usec=0 makes select() non-blocking.
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 1000;

        int retval, n;
        char buf[1024];

        if((retval = select(fd+1, &rfds, NULL, NULL, &tv)) < 0) {
            sys_die(select, failed?);
        } else if(!retval) {
            struct stat s;
            if(stat(portname, &s) < 0) {
                fprintf(stderr, "pi connection closed.  cleaning up\n");
                exit(0);
            }
            // can we lose stuff?
            // usleep(1000);
        } else if(FD_ISSET(0, &rfds)) {
            fprintf(stderr, "input data is available now.\n");
            if((n = read(0, buf, sizeof buf)) <= 0)
                sys_die(read, failed);
            buf[n] = 0;
            // fprintf(stderr, "got: <%s>\n", buf);
            if(write(fd, buf, n) < 0)
                sys_die(write, failed?);
            fprintf(stderr, "wrote: <%s>\n", buf);
        } else {
            assert(FD_ISSET(fd, &rfds));
            if((n = read(fd, buf, sizeof buf)) <= 0)
                sys_die(read, failed);
            buf[n] = 0;
            fprintf(stderr, "PI: %s", buf);
        }
    }
    exit(0);
}

#if 0
static void echo(int fd, const char *portname) {
    fprintf(stderr, "listening on port <%s>\n", portname);

    while(1) {
        unsigned char buf [4096];
    
        int n = read (fd, buf, sizeof buf - 1);
        if(!n) {
                struct stat s;
                if(stat(portname, &s) < 0) {
                    fprintf(stderr, "pi connection closed.  cleaning up\n");
                    exit(0);
                }
                // gonna have to override this if you want to test.
                usleep(1000);
        } else if(n < 0) {
            fprintf(stderr, "ERROR: got %s res=%d\n", strerror(n),n);
            fprintf(stderr, "pi connection closed.  cleaning up\n");
            exit(0);
        } else {
            buf[n] = 0;
            // XXX printf does not flush until newline!
            fprintf(stderr, "%s", buf);
            if(done(buf)) {
                fprintf(stderr, "\nSaw done\n");
                fprintf(stderr, "\nbootloader: pi exited.  cleaning up\n");
                exit(0);
            }
        }
    }
}
#endif

#else
// simple state machine to indicate when we've seen a special string
// from the pi telling us to shutdown.
static int done(char *s) {
	static unsigned pos = 0;
	const char exit_string[] = "DONE!!!";
	const int n = sizeof exit_string - 1;

	for(; *s; s++) {
		assert(pos < n);
		if(*s != exit_string[pos++]) {
			pos = 0;
			return done(s+1); // check remainder
		}
		// maybe should check if "DONE!!!" is last thing printed?
		if(pos == sizeof exit_string - 1)
			return 1;
	}
	return 0;
}

// read and echo the characters from the tty until it closes (pi rebooted)
// or we see a string indicating a clean shutdown.
static void echo(int fd, const char *portname) {
        while(1) {
                char buf [4096];
                int n = read (fd, buf, sizeof buf - 1);
                if(!n) {
                        struct stat s;
			// change this to fstat.
                        if(stat(portname, &s) < 0) {
                                fprintf(stderr, 
					"pi connection closed.  cleaning up\n");
                                exit(0);
                        }
			// gonna have to override this if you want to test.
                        usleep(1000);
                } else if(n < 0) {
                        fprintf(stderr, "pi connection closed.  cleaning up\n");
                        exit(0);
		} else {
                        buf[n] = 0;
                        // XXX printf does not flush until newline!
                        fprintf(stderr, "%s", buf);

			if(done(buf)) {
                        	fprintf(stderr, "\nbootloader: pi exited.  cleaning up\n");
                        	exit(0);
			}
                }
	}
}
#endif

// usage: my-install [-silent] [/<dev path>]  progname
int main(int argc, char *argv[]) { 
	const char *portname = 0;

    if(argc == 2)
        portname = argv[1];
    else if(argc > 2)
        panic("too many arguments %d\n", argc);
    
	// open tty
	int fd = open_tty(&portname);

    // this worked
    
    // defined: 
    //  /usr/include/asm-generic/termbits.h
    // fd = set_tty_to_8n1(fd, B9600, 1);
    //fd = set_tty_to_8n1(fd, B115200, 1);
    // fd = set_tty_to_8n1(fd,  B230400, 1);
    // fd = set_tty_to_8n1(fd,  B460800, 1);

    // can almost do: just a weird start character.
    // fd = set_tty_to_8n1(fd,  B1000000, 1);

    // all garbage?
    // fd = set_tty_to_8n1(fd,  B921600, 1);

    // FASTEST we could do.
    // cool.
    // fd = set_tty_to_8n1(fd,  B576000, 1);

 	// set it to be 8n1  and 115200 baud
    fd = set_tty_to_8n1(fd, B115200, 1);
	echo(fd, portname);
	fprintf(stderr, "pi-cat: Done!\n");
	return 0;
}
