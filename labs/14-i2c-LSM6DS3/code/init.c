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
// #include "LSM6DS33.h"
#include <limits.h>

const unsigned lsm6ds33_addr = 0b1101011; // this is the gyro/accel;
typedef unsigned char u8;

/************************************************************************8888
 * I2C helpers.
 */
enum {
    REG_WHO_AM_I      = 0xf,
    VAL_WHO_AM_I      = 0x69,
    CTRL3_C           = 0x12,


    CTRL9_XL          = 0x18,
    STATUS_REG        = 0x1E,
    CTRL1_XL          = 0x10,
    OUTX_L_XL         = 0x28,
};

// accel
typedef enum  {
    // p12 of application note.  xlm_hm_mode=0 -> all are high perf
    lsm6ds33_off    = 0,
    lsm6ds33_13hz   = 0b0001,   // low power (xl_hm_mode=1)
    lsm6ds33_26hz   = 0b0010,   // low power
    lsm6ds33_52hz   = 0b0011,   // low power
    lsm6ds33_104hz  = 0b0100,   // normal power
    lsm6ds33_208hz  = 0b0101,   // normal power
    lsm6ds33_416hz  = 0b0110,   // high performance
    lsm6ds33_833hz  = 0b0111,   // high performance
    lsm6ds33_1660hz = 0b1000,   // high performance
    // next two only legal for accel, not gyro
    lsm6ds33_3330hz = 0b1001,   // high performance
    lsm6ds33_6660hz = 0b1010,   // high performance
} lsm6ds33_hz_t;


static unsigned rd(unsigned char reg) {
	i2c_write(lsm6ds33_addr, &reg, 1);
	unsigned char uc = 0;
	i2c_read(lsm6ds33_addr, &uc, 1);
	return uc;
}
static void wr(unsigned char reg, unsigned char v) {
	unsigned char data[2];
	data[0] = reg;
	data[1] = v;
	i2c_write(lsm6ds33_addr, data, 2);
}

// lowest reg --- hw will auto-increment.
// XXX: i thought you had to write a bit high for this to happen?
int rd_n(uint8_t base_reg, uint8_t *v, uint32_t n) {
    i2c_write(lsm6ds33_addr, (void*) &base_reg, 1);
    return i2c_read(lsm6ds33_addr, v, n);
}

/****************************************************************************
 */

// BDU = block update till all axis ready.  have to keep auto-increment
// enabled (IF_INC)
static void set_bdu(void) {
    //    IF_INC          BDU
    wr(CTRL3_C, (1 << 2) | (1 << 6));
}

static void accel_on(void) {
    // https://www.pololu.com/file/0J1088/LSM6DS33-AN4682.pdf
    // this is from p23 

    wr(CTRL9_XL, ???);   // ACCEL: x,y,z enabled
    set_bdu();      // set BDU to prevent data tearing.
}


static void init_accel(lsm6ds33_hz_t hz) {
    switch(hz) {
    // these only work for "low-power" i think (p10)
    case lsm6ds33_1660hz:
    case lsm6ds33_3330hz:
    case lsm6ds33_6660hz:
        panic("accel: hz setting of %x does not appear to work\n", hz);
    default:
        break;
    }
    if(hz > lsm6ds33_6660hz)
        panic("invalid hz\n");

    assert(rd(REG_WHO_AM_I) == VAL_WHO_AM_I);
    debug("whoami=%x\n", rd(REG_WHO_AM_I));
    accel_on();

    // P46 of LSM6DS33.pdf: 00 = 2g, 01 = 16g, 10 = 4g, 11 = 8g
    // this is set to +/-16g
    // we just use 2g.
    unsigned scale_g = 0;

    // p 46, 9.11 of data sheet.
    //      FS      ODR
    wr(CTRL1_XL, ???);

    delay_ms(20);

    assert(rd(REG_WHO_AM_I) == VAL_WHO_AM_I);
    mb();
}

int accel_data_avail(void) {
    return (rd(STATUS_REG) & 0x1) != 0;
}

// p26 interprets the data.
// if high bit is set, then accel is negative.
//
// read them all at once for consistent
// readings using autoincrement.
static inline void rd_mg_accel(int *x, int *y, int *z) {
    while(!accel_data_avail())
        ;
    u8 regs[6];
    rd_n(OUTX_L_XL, regs, 6); // lowest reg --- hw will auto-increment.

    // the values for:
    //  -  x are in regs[0], regs[1]
    //  -  y are in regs[2], regs[3]
    //  -  z are in regs[4], regs[5]
    unimplemented();
}

void notmain(void) {
	uart_init();

	delay_ms(100);   // allow time for device to boot up.
	i2c_init();
	delay_ms(100);   // allow time to settle after init.
    
    if(rd(REG_WHO_AM_I) != VAL_WHO_AM_I)
        panic("failed\n");
    else
        printk("success!!\n");

    init_accel(lsm6ds33_416hz);

    int x,y,z;
    x = y = z = 0;

    // discard the first 100
    for(int i = 0; i < 100; i++) 
        rd_mg_accel(&x,&y,&z);

    for(int i = 0; i < 200; i++) {
        rd_mg_accel(&x,&y,&z);

        // drain it --- not sure if we need to?
        while(accel_data_avail())
            rd_mg_accel(&x,&y,&z);

        printk("accel x=%d,y=%d,z=%d\n", x,y,z);
        delay_ms(1000);
    }
    clean_reboot();
}
