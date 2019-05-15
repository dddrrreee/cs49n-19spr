/*
    simple GPIO interrupts.
*/
#include "rpi.h"
#include "timer-interrupt.h"

static const unsigned pin = 21;
static volatile unsigned n_rising_edge, n_falling_edge, last_touch;

// bitwise-or in a value into addr:  addr |= val
static void or32(volatile void *addr, unsigned val) {
    mb();
    put32(addr, get32(addr) | val);
    mb();
}
static void OR32(unsigned addr, unsigned val) {
    or32((volatile void*)addr, val);
}

enum {
    GPIO_BASE = 0x20200000,
    // define 
    //  GPIO_EVENT_DETECT0
    //  GPIO_RISING_EDGE 
    //  GPIO_FALLING_EDGE
    //  GPIO_HIGH_ENABLED0
    //  GPIO_LOW_ENABLED0 
    GPIO_RISING_EDGE = GPIO_BASE + 0x4C,
};

enum { GPIO_INT0 = 49, GPIO_INT1, GPIO_INT2, GPIO_INT3 };

void gpio_int_set(unsigned addr, unsigned pin) {
    if(pin >= 32)
        return;

    // set the right bit in <addr>
    unimplemented();

    // enable the right general interrupt using or32
    //   from p 113: have to enable a gpio_int[0], which = 49.  This means we use 
    //   enable_IRQs_2:
    //        - Enable_IRQs_1 covers [0..31],
    //        - Enable_IRQs_2 covers [32..63]
    unimplemented();


    // leave this memory barrier.
    mb();
}

// p97 set to detect rising edge (0->1).  hardware detects by sampling based on the clock.
// it looks for "011" (low, hi, hi) to suppress noise.  i.e., its triggered a bit after
// the transition actually happens.  if you want more precise, you should us
// async rising edge (p99)
void gpio_int_rising_edge(unsigned pin) {
    gpio_int_set(GPIO_RISING_EDGE, pin);
}

// p98: detect falling edge (1->0).  sampled using the system clock.  it looks for
// "100" --- i.e., is triggered a bit after the transition actually happens.  if you want 
// more precise, you should use async falling edge. (p99)
void gpio_int_falling_edge(unsigned pin) {
    unimplemented();
}

// p98: if the pin is high after you clear its status, the status remains hi.  i.e.,
// i think you have to manually set the gpio pin to 0 (?)
void gpio_int_high(unsigned pin) {
    unimplemented();
}

// p99: same as high.
void gpio_int_low(unsigned pin) {
    unimplemented();
}

// p96: a 1<<pin is set in EVENT_DETECT if <pin> triggered an interrupt.
// if you configure multiple events to lead to interrupts, you will have to 
// read the pin to determine which caused it.
int gpio_event_detected(unsigned pin) {
    mb();
    if(pin >= 32)
        return 0;

    unsigned r;
    // put your code here.
    unimplemented();

    mb();
    return r;
}

// p96
void gpio_event_clear(unsigned pin) {
    mb();
    if(pin >= 32)
        return;

    unimplemented();

    mb();
}

// client has to define this.
void interrupt_vector(unsigned pc) {
    // if a gpio event was detected: 
    //  - increment n_falling_edge if it was a falling edge
    //  - increment n_rising_edge if it was rising, 
    //  - and set last_touch to how long the sensor was last touched for.
    if(gpio_event_detected(pin)) {
        unimplemented();
    }
}

void notmain() {
    uart_init();
    int_init();
    system_enable_interrupts();
    printk("enabled!\n");

    gpio_set_input(pin);
    gpio_set_pulldown(pin);
    gpio_int_rising_edge(pin);
    gpio_int_falling_edge(pin);

    printk("initial read = %d\n", gpio_read(pin));
    unsigned n = 0;
    while(n < 20) {
        unsigned t = n_rising_edge + n_falling_edge;
        if(t != n) {
            printk("n rising edge = %d, n falling edge = %d, last_touch=%dms\n", 
                        n_rising_edge, n_falling_edge, last_touch/1000);
            n = t;
        }
    }
    clean_reboot();
}
