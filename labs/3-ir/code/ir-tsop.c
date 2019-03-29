/*
  	vishay TSOP 34838 IR sensor.

	- pulldown b/c when open ~ 0.


   Don't print while getting input!!

   one example protocol:
          http://irq5.io/2012/07/27/infrared-remote-control-protocols-part-1/
   no signal = 1.

                Signal Part     off (µs)        on (µs)
                        leader  9000            4500
                        0 bit   560             560
                        1 bit   560             1690
                        stop    560             –

                we wait for a 9000off.
                then start pulling bits.
        
                not sure what stop is (timeout on on i believe).


   for the current protocol: 
	wait for 40,000us and then start pulling bits til the next 40,000.
	[I believe the ones in class send button up/ button down]
*/

#include "rpi.h"

struct readings {
	unsigned v;
	unsigned usec;
};

void notmain(void) {
    uart_init();
    const unsigned out= 2;

    gpio_set_input(out);
    gpio_set_pulldown(out);

    unsigned v = gpio_read(out);
    printk("about to run IR!: initial = %d\n", v);

	struct readings *r = kmalloc(sizeof r[0] * 1000);
	while(1) {
		// ...
    }
}
