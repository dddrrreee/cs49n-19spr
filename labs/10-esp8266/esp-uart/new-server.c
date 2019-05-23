// engler
// control esp with ttl-uart
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
#include "at-commands.h"

void at_send(char *buf) {
    char cmd[1024];
    int n = strlen(buf);
    sprintf(cmd, "AT+CIPSENDEX=0,%d\r\n", n);
    esp_puts(at_fd, cmd);
    at_response();
    esp_puts(at_fd, buf);
    at_response();
}

// modifies buf to extract message.  maybe should copy?
char *at_extract_msg(char *buf) {
    // extract the message we received.
    char *s = strchr(buf, ':');
    assert(s);
    return s+1;
}

void part0(void) {
    at_cmd("AT");
}

// call this to setup the wifi access point.  will print out the IP address.
// change it so you can specify the <name> and <password> for the wifi network.
void esp_setup_wifi(void) {
    at_cmd("AT+RESTORE");
    at_cmd("AT+RST");
    at_drain();

    at_cmd("AT");
    at_cmd("AT+GMR");
    at_cmd("AT+RST");

//    at_cmd("ATE0");
    // add your own network name here.
    at_cmd("AT+CWSAP=\"YOURWIFI\",\"1234test\",5,3");
    at_cmd("AT+CWMODE=3");

    // on my esp this prints out: +CIFSR:APIP,"192.168.4.1"
    // so 192.168.4.1 is the device's address
    at_cmd("AT+CIFSR");
}

// setup a UDP server.  will have to adapt the tcp server.
void esp_udp_server(char *name) {
    // start a UDP server.
    // run using: netcat -u  192.168.4.1 4444
    // https://gist.github.com/mgub/3f4d2f074305d4d84344
    // have to use the ip address from AT+CIFSR
    at_cmd("AT+CIPSTART=0,\"UDP\",\"192.168.4.1\",4444,4444,2");
    unimplemented();
}

// should be a working example.
void esp_tcp_server(void) {
    // at_reset();
    // at_drain();

    at_cmd("AT+CWMODE=3");
    at_cmd("AT+CIPMUX=1");

   // start a tcp server.
   // run using: netcat 192.168.4.1 4444
   at_cmd("AT+CIPSERVER=1,4444");

   // set timeout.
   at_cmd("AT+CIPSTO?");
   at_cmd("AT+CIPSTO=7200");
   at_cmd("AT+CIPSTO?");

    at_cmd("AT+CIFSR");
    fprintf(stderr, "UNIX: run \"netcat <ip address> 4444\" to connect\n");

    // wait til we have a connection.
    char buf[1024];
    while(1) {
        if(!esp_async_gets(at_fd, buf, sizeof buf-1))
            continue;

        remove_cr(buf);
        if(strstr(buf, ",CONNECT")) {
            fprintf(stderr, "Connected: <%s>\n", buf);
            break;
        }
        fprintf(stderr, "Got: <%s> n = %d\n", buf, (int)strlen(buf));
        sleep(1);
    }

#if 0
    int cnt;
    while((cnt = at_get_connected_clients()) != 0)
        fprintf(stderr, "no connections\n");
    fprintf(stderr, "UNIX:have a %d connections.\n", cnt);
#endif

    // echo back what we received.
    while(1) {
        if(!esp_async_gets(at_fd, buf, sizeof buf-1))
            continue;
        remove_cr(buf);
        fprintf(stderr, "UNIX: received: <%s>, n=%d\n", buf, (int)strlen(buf));
        char msg[1024];
        if(strstr(buf, ",CLOSED")) {
            fprintf(stderr, "received a close!\n");
            exit(0);
        }
            
        sprintf(msg, "esp echoed: <%s>\n", at_extract_msg(buf));
        at_send(msg);
    }
}

// usage: server [/devpath]
int main(int argc, char *argv[]) { 
	const char *portname = 0;

    if(argc == 2)
        portname = argv[1];
    else if(argc > 2)
        panic("too many arguments %d\n", argc);
    esp_connect(&portname, 10);
    esp_setup_wifi();
    esp_tcp_server();

	fprintf(stderr, "esp-uart: Done!\n");
	return 0;
}
