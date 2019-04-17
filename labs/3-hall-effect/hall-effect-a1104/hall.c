/*
 * hall effect sensor: a1104eua-t (allegro microsystems)
 *
 * Pay attention to the voltages on:
 *	- Vcc
 *	- Vout.
 */
#include "rpi.h"

// common pattern: devices usually have various bits of internal state.  wrap
// it up in a structure.  we return a copy to we don't have to malloc/free it.
typedef struct {
    int input;
} hall_t;

// initialize the hall effect sensor.
//
//  1. recalL: you need to set GPIO pins as input or output before using them.
//  2. When circuit is "open" it can be noisy --- is it high or low when open?
//     If high need to configure a pin to be a "pullup", if low, "pulldown" to 
//     reject noise.  we provided gpio routines to do so (see rpi.h), but you
//     should implement your own.
//
hall_t hall_init_a1104(int input) {
    hall_t h = { . input = input };
    unimplemented();
    return h;
}

// return 1 if magnet touching, 0 otherwise.
int hall_read(hall_t *h) {
    unimplemented();
}

// may want to debounce in case it's bouncing around.
void hall_read_while_eq(hall_t *h, int v) {
    while(hall_read(h) == 0)
        ;
}

void notmain(void) {
    uart_init();

    printk("starting hall effect!\n");

    hall_t h = hall_init_a1104(21);
    printk("ready: hall effect!\n");

    for(int i = 0; i < 10; i++) {
        hall_read_while_eq(&h, 0);

        printk("sensed magnet! count = %d\n", i);
        hall_read_while_eq(&h, 1);
        printk("no magnet!\n");
    }

    printk("stopping hall effect!\n");
}
