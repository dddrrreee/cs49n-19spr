// engler
// control esp with ttl-uart.
//  this code is ugly, i know.  last minute.  blah.
//
// XXXX don't modify XXXX
// XXXX don't modify XXXX
// XXXX don't modify XXXX
// XXXX don't modify XXXX
// XXXX don't modify XXXX
// XXXX don't modify XXXX
// XXXX don't modify XXXX
// XXXX don't modify XXXX
// XXXX don't modify XXXX

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
#include "esp-uart.h"

int at_fd;

int esp_async_gets(int fd, char *buf, unsigned nbytes) {
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(at_fd, &rfds);

    // setting tv_sec = tv_usec=0 makes select() non-blocking.
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int r;
    if((r = select(at_fd+1, &rfds, NULL, NULL, &tv)) < 0)
        sys_die(select, failed?);
    if(!FD_ISSET(at_fd, &rfds)) 
        return 0;

    int n;
    if((n = read(at_fd, buf, nbytes)) <= 0)
        sys_die(read, failed);
    assert(n>0);
    buf[n-1] = 0;
    return n;
}

// buf has to be big enough.
int esp_gets(int fd, char *buf, int can_timeout) {
    int n;
    char c;

    while((n = read(fd, &c, 1)) == 1) {
        if(c == '\n') {
            *buf++ = 0;
            return 1;
        }
        if(c != '\r')
            *buf++ = c;
    }
    if(!n) {
        if(can_timeout)
            return 0;
        panic("esp timed out.  have: <%s>\n", buf);
    }
    sys_die(read, "read error: timed out?\n");
}

void esp_puts(int fd, char *buf) {
    for(; *buf; buf++) {
        if(write(fd, buf, 1) != 1)
            sys_die(write, "write failed?");
    }
}

// #define eq(in, str) strncmp(in, str "\r\n", sizeof str+1) == 0
#define eq(in, str) strcmp(in, str) == 0

static int is_err(const char *p) {
    return eq(p, "ERROR");
}

static int is_ok(const char *p) {
    return eq(p, "ready")
        || eq(p, "no change")
        || eq(p, "SEND OK")
        || eq(p, "OK");
}
static int is_busy(const char *p) {
    return eq(p, "busy");
}


char* remove_cr(char *_p) {
    
    for(char *p = _p; *p; p++)
        if(*p == '\r') *p = ' ';
    return _p;
}

void echo_no_cr(char *buf) {
    fprintf(stderr, "esp:");
    for(; *buf; buf++) 
        fprintf(stderr, "%c", (*buf == '\r') ? ' ' : *buf);
}

void at_drain(void) {
    char buf[1024];
    while(esp_gets(at_fd, buf, 1) > 0) {
        fprintf(stderr, "esp-drain: <%s>\n", buf);
        if(strcmp(buf,"ready") == 0) {
            fprintf(stderr, "saw ready!\n");
            return;
        }
    }
}

// if you concat too much...
int at_response_fn(int (*fp)(char *)) {
    int fd = at_fd;
    char buf[1024];
    int cnt = 0;
    while(esp_gets(fd, buf, 0) > 0) {
        fprintf(stderr, "%s\n", buf);
        if(strcmp(buf, "") == 0)
            continue;
        if(is_ok(buf))
            return cnt;
        if(is_err(buf))
            panic("got ERROR");
        if(is_busy(buf))
            panic("got busy");
        if(fp)
            cnt += fp(buf);
    }
    panic("failed\n");
}
int at_response(void) {
    return at_response_fn(0);
}


int at_cmd_fn(char *cmd, int (*fp)(char*)) {
    int fd = at_fd;
    esp_puts(fd, cmd);
    esp_puts(fd, "\r\n");
    return at_response_fn(fp);
}
int at_cmd(char *cmd) {
    return at_cmd_fn(cmd, 0);
}


int esp_connect(const char **portname, int timeout) {
    int fd = open_tty(portname);
    return at_fd = set_tty_to_8n1(fd, B115200, timeout);
}
