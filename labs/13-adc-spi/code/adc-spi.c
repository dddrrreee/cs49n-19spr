/*
 * trivial program to interact with the mcp3008 ADC converter using SPI.
 * 
 * You don't have to modify this initially: change mcp3008.h and mcp3008.c
 */
#include "rpi.h"
#include "adc-mcp3008.h"


// simple loop that prints the max and min value read for each 1 second interval.
void read_interval(void) {
    for(int n = 0; n < 10; n++) {
        unsigned s = timer_get_usec();
        unsigned min_adc,max_adc;

        max_adc = min_adc =  adc_mcp3008_read(CH0);

        while((timer_get_usec() - s) < 1000*1000) {
		    unsigned result = adc_mcp3008_read(CH0);
            if(result > max_adc)
                max_adc = result;
            if(result < min_adc)
                min_adc = result;
        }
        printk("sec=%d, max reading = %d, min reading = %d\n", n, max_adc, min_adc);
    }
}

// default value seems to be around 500.
void read_mic(void) { 
    while(1) {
        unsigned result = adc_mcp3008_read(CH0);
        if(result > 600)
            printk("got %d\n", result);
    }
}

// simple test: just read raw values, print if not zero.
void read_raw(void) {
    unsigned nothing = 0;
    while(1) {
        unsigned result = adc_mcp3008_read(CH0);
        if(result) 
            printk("got %d\n", result);
        else {
            nothing++;
            if(nothing % 100 == 0)
                printk("got %d\n", result);
        }
        delay_ms(10);
    }
}

void notmain() {
    uart_init();
    adc_mcp3008_init();
    enable_cache();

    // read_raw();
    read_mic();

	clean_reboot();
}
