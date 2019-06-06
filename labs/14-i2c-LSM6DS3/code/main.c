/*
 * General i2c background:
 * 	https://learn.sparkfun.com/tutorials/i2c

 * Chip background:
 *	docs/en.CD00290365.pdf:  
 *		"Application note" --- all quotes, page numbers from here.
 *	docs/en.CD00274221.pdf:
 *		chip spec.  more complete, harder to reason *	about.
 *
 * Wiring:
 *    We are using v1 I2c.   
 *	SDA = gpio pin2 
 *	SCL = gpio pin3
 *
 * Steps:
 *	1. ping device verify it (1) responds and (2) gives the id you expect.
 *	2. write the code to config to device.
 *	3. write the code to read data 

 * When everything works:
 *	- change the scaling for gravity (should see the numbers halve
 *	  if you double the scale, etc)
 *	- change from low power to high power.
 *	- when reading data, can do a fancy i2c read that is basically 
 *	  a memcpy 
 *	- convert to float (not you'll have to recompile for floating point).
 *

 * For future work (do it on your own now, but if you want to go all in later):
 *  structure breakdowns and values at:
 *	github.com/SuperHouse/esp-open-rtos/blob/master/extras/lis3dh/lis3dh.c
 *  generally good discussion there, too:
 * 	https://github.com/SuperHouse/esp-open-rtos/tree/master/extras/lis3dh
 */
#include "rpi.h"
#include "rpi-stdlib.h"
#include "i2c.h"

/*
 * From the app notes:
 * 	If the SDO pin is not connected or connected to ground, set sad0 to 
 *	0 (address 0x18). If the SDO pin is connected to 3V3, set sad0 to 1 
 *	(address 0x19)
 *
 * [See if you can brute force this!]
 */
static const unsigned lis3dh_addr = 0x18; 

/************************************************************************8888
 * I2C helpers.
 */

static unsigned read_i2c_reg8(unsigned char reg) {
	i2c_write(lis3dh_addr, &reg, 1);
	unsigned char uc = 0;
	i2c_read(lis3dh_addr, &uc, 1);
	return uc;
}
static void write_i2c_reg8(unsigned char reg, unsigned char v) {
	unsigned char data[2];
	data[0] = reg;
	data[1] = v;
	i2c_write(lis3dh_addr, data, 2);
}


/*************************************************************************
 * LIS3DHS specific routines helpers.
 */


#define BIT(x) (1 << (x))
#if 0
enum {
	// move if 0 down as you start implementing more and more
	WHO_AM_I = ??,

	REG_WHO_AM_I = 0xf,

	CTRL_REG0 = ??,

	CTRL_REG1 = ??,		// all the rest are in order.
	CTRL_REG2,
	CTRL_REG3,
	CTRL_REG4,
	CTRL_REG5,
	CTRL_REG6,
	REFERENCE,
	STATUS_REG,
	OUTX_L,
	OUTX_H,
	OUTY_L,
	OUTY_H,
	OUTZ_L,
	OUTZ_H,
};
#endif

#if 0
// definitions for ctrl reg1
#define CTRL_REG1_ODR1  BIT(5)
#define CTRL_REG1_LPEN  ??
#define CTRL_REG1_ZEN   ??
#define CTRL_REG1_YEN   ??
#define CTRL_REG1_XEN   ??
#endif

typedef struct {
        int16_t         x;
        int16_t         y;
        int16_t         z;
} raw_sample;

typedef struct {
        uint8_t reg1,
                reg2,
                reg3,
                reg4,
                reg5,
                reg6,

                int1_ths,
                int1_duration,
                int1_cfg,
                fifoCtrlReg;
} config;

static config config_new(void) {
	config c;
	memset(&c, 0, sizeof c);
	return c;
}

#if 0
// sequence is given on p 14
int getSample(raw_sample *s) {
	?? 
        return 1;
}

void config_low_power(void) {
	config c = config_new();

        // Enable 10 Hz, low power, with XYZ detection enabled
	// when yours works, change this to handle other ones!
	// 	p 10 (sec 3.2) explains the semantics / implications of modes.
        c.reg1 = CTRL_REG1_ODR1
                | CTRL_REG1_LPEN
                | CTRL_REG1_ZEN
                | CTRL_REG1_YEN
                | CTRL_REG1_XEN;

	// Startup sequence Application note (p13, sec4)
        write_i2c_reg8(CTRL_REG1, c.reg1);
        write_i2c_reg8(CTRL_REG2, c.reg2);
        write_i2c_reg8(CTRL_REG3, c.reg3);
        write_i2c_reg8(CTRL_REG4, c.reg4);
        write_i2c_reg8(CTRL_REG6, c.reg6);

	// we don't use high pass, nor interrupts, so (i *believe*) we skip
	// we don't enable interrupts so skip steps 7, 8,9,10

	// do last
        write_i2c_reg8(CTRL_REG5, c.reg5);

	// esp-rtos claims if sensor was in power down mode it takes at least 
	// 100 ms to start in another mode.
    delay_ms(100);
}

static int hasDevice(void) {
        for(int tries = 0; tries < 10; tries++) {
                if(read_i2c_reg8(REG_WHO_AM_I) == WHO_AM_I)
                        return 1;
                delay_ms(100);
        }
        return 0;
}

/*********************************************************************
 * Trivial program to sample and print xyz acceleration, checking if its above
 * a threshold.
 */

static int abs(int x) { return x< 0 ? -x : x; }
static int exceeds_thres(raw_sample *s, unsigned thres) {
	return abs(s->x) > thres || abs(s->y) > thres || abs(s->z) > thres;
}
static void simple_output(unsigned thres, unsigned delay, unsigned n) {
	for(int i = 0; i < n; i++) {
		raw_sample s;
		if(!getSample(&s))
			debug("no sample??\n");
		else {
			printf("got sample: x=%d, y=%d, z=%d", s.x,s.y,s.z);
			if(exceeds_thres(&s, thres))
				printf("   <------------ over threshold %d", 
									thres);
			printf("\n");
		
		}
		delay_ms(delay);
	}
}

#endif

void notmain(void) {
	uart_init();

	delay_ms(100);   // allow time for device to boot up.
	i2c_init();
	delay_ms(100);   // allow time to settle after init.

#if 0
    if(!hasDevice())
        panic("Don't have device\n");

    config_low_power();
    simple_output(32000, 100,128);
#endif

    clean_reboot();
}
