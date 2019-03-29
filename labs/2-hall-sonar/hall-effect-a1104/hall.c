/*
 * hall effect sensor: a1104eua-t (allegro microsystems)
 *
 * Pay attention to the voltages on:
 *	- Vcc
 *	- Vout.
 */
#include "rpi.h"

void notmain(void) {
    uart_init();

    printk("starting hall effect!\n");

    // put your code to print out when magnet close / far here.
    //  1. recalL: you need to set GPIO pins as input or output before using them.
    //  2. When circuit is "open" it can be noisy --- is it high or low when open?
    //     If high need to configure a pin to be a "pullup", if low, "pulldown" to 
    //     reject noise.  we provided gpio routines to do so (see rpi.h), but you
    //     should implement your own.

    printk("stopping hall effect!\n");
}
