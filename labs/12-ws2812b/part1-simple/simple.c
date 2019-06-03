/*
 * simple test to turn on the first pixel in the light string to green.
 */
#include "rpi.h"
#include "WS2812B.h"

void check_timings(int pin);

// the pin used to control the light strip.
const unsigned pix_pin = 21;


void notmain(void) {
    uart_init();

    // have to initialize the cycle counter or it returns 0.
    cycle_cnt_init();

    // output.
    gpio_set_function(pix_pin, GPIO_FUNC_OUTPUT);

    // if you don't do this, the granularity is too large for the timing
    // loop. 
    enable_cache(); 

    // NOTE: when you get your code working, check these timings.
    //	check_timings(pix_pin);

    // turn on one pixel to blue.
    // make sure you can:
    //  1. write different colors.
    //  2. write different pixels.

    ws2812b_sendbyte(pix_pin, 0);      // green
    ws2812b_sendbyte(pix_pin, 0);      // red
    ws2812b_sendbyte(pix_pin, 0xff);   // blue

    ws2812b_flush(pix_pin);

    delay_ms(1000*3);

	clean_reboot();
}

// must be last.
#include "timing-checks.h"
