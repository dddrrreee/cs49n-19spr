/*
 * trivial driver for an addressable LED strip [only tested on WS2812B]
 * 
 * key sentence  missing from most documents:
 *
 *	Addressing is indirect: you don’t pass a NeoPixel’s address
 *	via SPI. Instead the order of data in the frame provides the
 *	addressing: the first three color values are taken by the first
 *	NeoPixel in line, the second set of values by the next NeoPixel,
 *	the third by the third and so on.
 *		--- https://developer.electricimp.com/resources/neopixels
 *
 * 	I.e., it's not a FIFO buffer, the first thing you write gets claimed
 *	by the first LED, etc.
 *
 * 
 * to send a:
 *	- 0 = T0H, T0L
 * 	- 1 = T1H, T1L
 * 	- FLUSH = how long to wait with voltage low to flush it out.
 */
#include "rpi.h"
#include "neopixel.h"

// number of pixels on your light strip.  you will have to figure this out.
const unsigned npixels = 60;
// the pin used to control the light strip.
const unsigned pix_pin = 21;

// crude routine to write a pixel at a given location.
// XXX: do we have to do the membarriers?
void place_cursor(neo_t h, int i) {
    neopix_write(h,i-2,0xff,0,0);
    neopix_write(h,i-1,0,0xff,0);
    neopix_write(h,i,0,0,0xff);
    neopix_flush_writes(h);
}

void notmain(void) {
    uart_init();

    // if you don't do this, the granularity is too large for the timing
    // loop.  pull into neopix?
    enable_cache(); 
    neo_t h = neopix_init(pix_pin, npixels);

    // this should pass.  you need to define the code to write for a specific number of 
    // nanaseconds.
	neopixel_check_timings(h);

	unsigned start = timer_get_usec();
    while(1) {
	    for(int j = 0; j < 10; j++) {
            for(int i = 0; i < npixels; i++) {
                place_cursor(h,i);
                delay_ms(10-j);
            }
        }
    }
	clean_reboot();
}
