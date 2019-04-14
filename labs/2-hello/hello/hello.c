#include "rpi.h"

void notmain(void) {
    // initialize the uart so that we can communicate over the TTL
    uart_init();

    printk("Hello world.\n");

    // this reboots the pi and prints out a special message so that the Unix-side
    // bootloader shuts down.
    clean_reboot();
}
