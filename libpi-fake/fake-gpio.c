// we could just implement put32/get32 and link it in.   but we can also 
// implement the gpio_functions so that we can cross-check at a higher level.
// this makes debugging differences alot easier.
#include <stdio.h>
#include <stdlib.h>
#include "fake-pi.h"

static long bias = 2;

// bias it more than 50/50.
void fake_gpio_set_bias(int n) {
    bias = n + 2;
    trace("bias==%ld\n", bias);
}

void gpio_write(unsigned pin, unsigned v) {
    trace("pin=%d, val=%d\n", pin, v);
}
unsigned gpio_read(unsigned pin) {
    static unsigned val;
    unsigned v= val++ & 1;
// (random()%bias != 0);
    trace("pin=%d, returning=%d\n", pin, v);
    return v;
}
void gpio_set_input(unsigned pin) { trace("pin=%d\n", pin); }
void gpio_set_output(unsigned pin) { trace("pin=%d\n", pin); }
void gpio_set_pullup(unsigned pin) { trace("pin=%d\n", pin); }
void gpio_set_pulldown(unsigned pin) { trace("pin=%d\n", pin); }



