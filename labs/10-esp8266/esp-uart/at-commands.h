#ifndef __AT_COMMANDS_H__
#define __AT_COMMANDS_H__

// code to help with at commands.
//
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



#define XX(cmd, str) static void at_ ## cmd(void) { at_cmd(#str); }

XX(reset,               AT+RST)
XX(test,                AT)
XX(disconnect_from_ap,  AT+CWQAP)
XX(check_disconnected,  AT+CIPSTATUS)
XX(get_version,         AT+GMR)
XX(set_mode_client,     AT+CWMODE=1)
// access point only.
XX(set_mode_ap,         AT+CWMODE=2)
// client and ap
XX(set_mode_dual, AT+CWMODE=3)
XX(get_mode, AT+CWMODE?)
XX(scan_for_wifi, AT+CWLAP)

// for server?
XX(get_ip, AT+CIFSR)

// get our ip
XX(get_our_ip, AT+CIPSTA?)
XX(get_our_ap_ip, AT+CIPAP?)
XX(get_ap_config, AT+CWSAP_DEF?)

#include <string.h>

static int count_colon(char *buf) {
    return strchr(buf, ':') != 0;
}

int at_get_connected_clients(void) {
    return at_cmd_fn("AT+CWLIF", count_colon);
}


static void at_connect_to(const char *network, const char *password) {
    // at_cmd("AT+CWJAP=\"esp_123\",\"1234test\"");
    char buf[1024];

    sprintf(buf, "AT+CWJAP=\"%s\",\"%s\"", network, password);
    printf("about to run <%s>\n", buf);

#if 0
    strcpy(buf, "AT+CWJAP=\"");
    strcat(buf, network);
    strcat(buf, "\",\"");
    strcat(buf, password);
    strcat(buf, "\"");
#endif
    at_cmd(buf);
    // at_cmd("AT+CWJAP=\"esp_123\",\"1234test\"");
}

#endif
