// fake implementations of pi-code so that we can check hall in user space.
#include "fake-pi.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void uart_init(void) { 
    trace("uart\n");
}
void rpi_reboot(void) {
    trace("in reboot\n");
    exit(0);
}
void clean_reboot(void) {
    trace("clean reboot\n");
    rpi_reboot();
}
int printk(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
        printf("PI:");
        int res = vprintf(fmt, args);
    va_end(args);
    return res;
}

void delay_us(unsigned us) {
    trace("delay_us = %dusec\n", us);
}
void delay_ms(unsigned ms) {
    trace("delay_ms = %dms\n", ms);
}


/************************************************************************
 * building fake time is kind of not-great.  we just advance the clock
 * by a random amount, capped via time_delta.
 */

// if this is 0 just pick random times.  otherwise advance by e \in [0..n).
static int time_delta = 0;
void fake_timer_max_delta(unsigned delta) {
    time_delta = delta;
}

// this won't work if there's a maxium allowed time.
unsigned timer_get_usec(void) {
    static unsigned t;

    if(time_delta)
        t += (random() % time_delta);
    else
        t = random();
    trace("returning time=%dusec\n", t);
    return t;
}

