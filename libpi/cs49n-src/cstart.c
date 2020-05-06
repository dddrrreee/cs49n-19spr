#include "rpi.h"
#include "rpi-fp.h"

void _cstart() {
	extern int __bss_start__, __bss_end__;
	void notmain();
    void cb_init(void);

    int* bss = &__bss_start__;
    int* bss_end = &__bss_end__;
 
    while( bss < bss_end )
        *bss++ = 0;

#ifdef USE_FP
    enable_fpu();
#endif
    notmain(); 
	rpi_reboot();
}
