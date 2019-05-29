#ifndef __WS2812B__
#define __WS2812B__
/*
    Datasheet in docs/WS2812B

    One way to communicate more information is to use a fancier protocol. The other is to use
    time --- there's a lot of information you can encode in various numbers of nanoseconds. 
    The WS2812B uses time:
        To send a 1 bit: write a 1 for T1H, then a 0 for T0H.
        To send a 0 bit: write a 1 for T1L, then a 0 for T0L.
    The protocol:
        - Each pixel needs 3 bytes of information (G,R,B) [in that order] for the color.  
        - The first 3 bytes you sent get claimed by the first pixel, the second 3 bytes 
          by the second, and so forth.
        - You then hold the communication pin low for FLUSH cycles to tell the pixel you
          are done.
    
    The data sheet for WS2812B says the following timings: 
        T0H    0: .35us +- .15us
        T1H    1: .9us +- .15us
        T0L    0: .9us +- .15us
        T1L    1: .35us +- .15us
        Trst = 80us 

    The above timings work. However, for long light strings, they can consume significant
    time.   Ideally we'd like to push the times as low as possible so that we can set/unset
    more pixels per second.   The following *seem* to work on the light strings I have,
    but could break on others (or could break on mine with the wrong set of pixels).   Use
    with caution:
        T0H = 200ns + eps
        T1H = 750ns + eps
        T0L = 750ns + eps
        T1L = 200ns + eps
 */

#ifndef OPTIMIZE_NEO_TIMING

static const unsigned
        T1H = ns_to_cycles(900),        // Width of a 1 bit in ns
        T0H = ns_to_cycles(350),        // Width of a 0 bit in ns
        T1L = ns_to_cycles(350),        // Width of a 1 bit in ns
        T0L = ns_to_cycles(900),        // Width of a 0 bit in ns
        FLUSH = ns_to_cycles(50 *1000);    // how long to hold low to flush

#else

// reduce the timings down to right on the edge of acceptable.  for a long
// light strip this might add up.
static const unsigned
	// assumes we can do 20 cycles tolerance.
	EPS = 20,
        T1H = ns_to_cycles(750+EPS),        // Width of a 1 bit in ns
        T0H = ns_to_cycles(200+EPS),        // Width of a 0 bit in ns
        T1L = ns_to_cycles(200+EPS),        // Width of a 1 bit in ns
        T0L = ns_to_cycles(750+EPS),        // Width of a 0 bit in ns
        FLUSH = ns_to_cycles(10 *1000);    // how long to hold low to flush
#endif

static void 
sendpixel(unsigned pin, uint8_t r, uint8_t g, uint8_t b) {
    // XXX: b/c sendbyte is not inlined, coulde lead to a non-trivial
    // big delay between the send bytes --- is this safe?  should
    // inline sendbyte?
    sendbyte(pin, g);
    sendbyte(pin, r);
    sendbyte(pin, b);
}
#endif
