/*
 * "higher-level" neopixel stuff.
 * our model:
 *  1. you write to an array with as many entries as there are pixels as much as you 
 *     want
 *  2. you call <neo_pixel_flush> when you want to externalize this.
 *
 * Note:
 *  - out of bound writes are ignored.
 *  - do not allow interrupts during the flush!
 */
#include "rpi.h"
#include "neopixel.h"
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

static void neopix_flush(neo_t h) { 
    ws2812b_flush(h->pin);
}

neo_t neopix_init(uint8_t pin, uint32_t npixel) {
    // replace  this with your kmalloc!
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
    ws2812b_sendpixel(h->pin, r,g,b);
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
        ws2812b_sendpixel(pin, t.r,t.g,t.b);
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
#include "timing-checks.h"

void neopixel_check_timings(neo_t h) {
    check_timings(h->pin);
}
