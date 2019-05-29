/*
 * the low-level "driver" code for LED strip.  the higher level support
 * stuff is in neopix.c
 *
 * addressable LED strip [only tested on WS2812B]
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
#include "cycle-count.h"
#include "neopixel.h"
#include "CPU-speed.h"

#define gpio_set_on "error"
#define gpio_set_off "error"

static void sendbyte(unsigned pin, uint8_t b);

// alitove 
#include "WS2812B.h"

struct neo_pixel {
	uint8_t r,g,b;
};

// does this need to be a constant?  actually for timings, perhaps?
struct neo_handle {
	uint8_t pin;
	uint32_t npixel;

	// struct hack
	struct neo_pixel pixels[];
};

/*************************************************************************
 * This is the entire protocol, other than the exact ns timings.
 *
 */

static inline void delay_ncycles(unsigned s, unsigned n) {
	// XXX: this alignment was screwing things up!!
	// asm volatile (".align 4");

	// this handles wrap-around, but doing so adds two instructions,
	// which makes the delay not as tight.
	while((read_cycle_cnt() - s) < n) 
		;
}

// duplicate set_on/off so we can inline to reduce overhead.
// they have to run in < the delay we are shooting for.
#define GPIO_BASE 0x20200000
static inline void gpio_set_on_raw(unsigned pin) {
	volatile unsigned *gpio_set0  = (void*)(GPIO_BASE + 0x1C);
        *gpio_set0 = (1 << pin);
}
static inline void gpio_set_off_raw(unsigned pin) {
	volatile unsigned *gpio_clr0  = (void*)(GPIO_BASE + 0x28);
        *gpio_clr0 = (1 << pin);
}


// write 1 for <ncycles> --- getting here and exiting takes cycles, so you
// will need to add compensation.
static inline void timed_on(unsigned pin, unsigned ncycles) {
    unimplemented();
}

// write 0 for <ncycles> --- getting here and exiting takes cycles, so you
// will need to add compensation.
static inline void timed_off(unsigned pin, unsigned ncycles) {
    unimplemented();
}

// these implement the T1H,T0H,T0L,T1L
static inline void t1h(unsigned pin) { 
    unimplemented();
}
static inline void t0h(unsigned pin) { 
    unimplemented();
}
static inline void t1l(unsigned pin) { 
    unimplemented();
}
static inline void t0l(unsigned pin) { 
    unimplemented();
}
static inline void treset(unsigned pin) { 
    unimplemented();
}

static inline void sendbit(unsigned pin, uint8_t b) {
    unimplemented();
}

#if 0
// actually, i think unrolling might not be good.
// also the inlining.  is huge.
static void sendbyte_unroll(uint8_t b) {
	sendbit((b>>7)&1);
	sendbit((b>>6)&1);
	sendbit((b>>5)&1);
	sendbit((b>>4)&1);
	sendbit((b>>3)&1);
	sendbit((b>>2)&1);
	sendbit((b>>1)&1);
	sendbit((b>>0)&1);
}
#else
static void sendbyte(unsigned pin, uint8_t b) {
	for(int i = 7; i >= 0; i--)
		sendbit(pin, (b>>i) & 1);
}
#endif


/*************************************************************************
 * This is implemented for you.
 * XXXX: do not modify unless you know what you are doing.
 * XXXX: do not modify unless you know what you are doing.
 * XXXX: do not modify unless you know what you are doing.
 * XXXX: do not modify unless you know what you are doing.
 * XXXX: do not modify unless you know what you are doing.
 * XXXX: do not modify unless you know what you are doing.
 */

static void neopix_flush(neo_t h) { 
	treset(h->pin);
}

neo_t neopix_init(uint8_t pin, uint32_t npixel) {

    static unsigned buf[1024];
    neo_t h = (void*)buf;
    assert(sizeof *h + sizeof h->pixels[0] * npixel < sizeof buf);
	// neo_t h = (void*)kmalloc(sizeof *h + sizeof h->pixels[0] * npixel);
	h->npixel = npixel;
	h->pin = pin;
	memset(h->pixels, 0, sizeof h->pixels[0] * npixel);

	gpio_set_function(pin, GPIO_FUNC_OUTPUT);
	cycle_cnt_init();
	return h;
}

// this just makes the definition visible in other modules.
void neopix_sendpixel(neo_t h, uint8_t r, uint8_t g, uint8_t b) {
	sendpixel(h->pin, r,g,b);
}


// doesn't need to be here.
void neopix_fast_clear(neo_t h, int n) {
        for(int i = 0; i < n; i++)
                neopix_sendpixel(h, 0,0,0);
        neopix_flush(h);
}

void neopix_clear(neo_t h) {
        neopix_fast_clear(h, h->npixel);
}

void neopix_write(neo_t h, uint32_t pos, uint8_t r, uint8_t g, uint8_t b) {
	if(pos >= h->npixel)
		return;
	// assert(pos < h->npixel);
	h->pixels[pos] = (struct neo_pixel) { .r = r, .g = g, .b = b };
}
void neopix_flush_writes(neo_t h) {
	unsigned n = h->npixel;
	unsigned pin = h->pin;

	struct neo_pixel *p = h->pixels;

	for(unsigned i = 0; i < n; i++, p++) {
		struct neo_pixel t = *p;
		sendpixel(pin, t.r,t.g,t.b);
	}
	neopix_flush(h);

	// clear after to minimize timing perturbation risk.  i think is 
	// fine either way tho.
	memset(h->pixels, 0, sizeof h->pixels[0] * n);
}

/*
 * t1h: expected 630 cycles, got 631 cycles
 * t0h: expected 245 cycles, got 249 cycles
 * t1l: expected 245 cycles, got 251 cycles
 * t0l: expected 630 cycles, got 633 cycles
 * treset: expected 35000 cycles, got 35005 cycles
 * 100x check:expected 350 ns, got 360 ns
 * 100x check:expected 900 ns, got 900 ns
 * 100x check:expected 900 ns, got 920 ns
 * 100x check:expected 350 ns, got 380 ns
 * 100x check:expected 50000 ns, got 50000 ns
 */
#include "neopixel-checks.h"


