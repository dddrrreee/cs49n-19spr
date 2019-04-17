/*
 * hall effect sensor: a1104eua-t (allegro microsystems)
 *
 * "The output of these devices (pin 3) switches low when the magnetic
 * field at the Hall element exceeds the operate point threshold (BOP). At
 * this point, the output voltage is VOUT(SAT). When the magnetic field
 * is reduced to below the release point threshold (BRP), the device
 * output goes high. The difference in the magnetic operate and release
 * points is called the hysteresis (Bhys) of the device. This built-in
 * hysteresis allows clean switching of the output even in the presence
 * of external mechanical vibration and electrical noise."
 *
 * Pay attention to the voltages on:
 *	- Vcc --- what is going into the device.
 *	- Vout --- what is coming out of the device.
 */
#include "rpi.h"

// initialize the hall effect sensor.
//
//  1. recalL: you need to set GPIO pins as input or output before using them.
//  2. When circuit is "open" it can be noisy --- is it high or low when open?
//     If high need to configure a pin to be a "pullup", if low, "pulldown" to 
//     reject noise.  we provided gpio routines to do so (see rpi.h), but you
//     should eventually implement your own.
//
void hall_init_a1104(int input) {
    unimplemented();
}

// use gpio_read() to return 1 if magnet touching, 0 otherwise.
int hall_read(int hall) {
    unimplemented();
}

void notmain(void) {
    uart_init();

    printk("starting hall effect!\n");
    int hall = 21;
    hall_init_a1104(hall);

    printk("ready: hall effect!\n");
    for(int i = 0; i < 10; i++) {
        while(hall_read(hall) == 0) {}
        printk("sensed magnet! count = %d\n", i);

        while(hall_read(hall) == 1) {}
        printk("no magnet!\n");
    }
    printk("stopping hall effect!\n");
    clean_reboot();
}
