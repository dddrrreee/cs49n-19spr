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

const unsigned lsm6ds33_addr = 0b1101011; // this is the gyro/accel;

/************************************************************************8888
 * I2C helpers.
 */
enum {
      REG_WHO_AM_I          = ??,
      VAL_WHO_AM_I          = ??
};

static unsigned read_i2c_reg8(unsigned char reg) {
	i2c_write(lsm6ds33_addr, &reg, 1);
	unsigned char uc = 0;
	i2c_read(lsm6ds33_addr, &uc, 1);
	return uc;
}
static void write_i2c_reg8(unsigned char reg, unsigned char v) {
	unsigned char data[2];
	data[0] = reg;
	data[1] = v;
	i2c_write(lsm6ds33_addr, data, 2);
}


void notmain(void) {
	uart_init();

	delay_ms(100);   // allow time for device to boot up.
	i2c_init();
	delay_ms(100);   // allow time to settle after init.
    
    if(read_i2c_reg8(REG_WHO_AM_I) != VAL_WHO_AM_I)
        panic("failed\n");
    else
        printk("success!!\n");

    clean_reboot();
}
