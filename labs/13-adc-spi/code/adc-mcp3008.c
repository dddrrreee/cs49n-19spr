/*
 * mcp3008 ADC converter.
 * https://z4ziggy.wordpress.com/

 * nice writeup:
 *      http://blog.falafel.com/mcp3008-analog-to-digital-conversion/
 *  	https://cdn-shop.adafruit.com/datasheets/MCP3008.pdf
 * 	https://learn.adafruit.com/reading-a-analog-in-and-controlling-\
					audio-volume-with-the-raspberry-pi
 *
 *
 * SPI:
 *  	GPIO_PIN7  = SPI0_CE1_N
 *   	GPIO_PIN8  = SPI0_CE0_N
 *	GPIO_PIN9  = SPI0_MISO  [master in, slave out]
 *	GPIO_PIN10 = SPI0_MOSI  [master out, slave in]
 *	GPIO_PIN11 = SPI0_SCLK
 *
 *	MCP3008 VDD to Raspberry Pi 3.3V
 *	MCP3008 VREF to Raspberry Pi 3.3V
 *	MCP3008 AGND to Raspberry Pi GND
 *	MCP3008 DGND to Raspberry Pi GND
 *	MCP3008 CLK to Raspberry Pi pin 18
 *	MCP3008 DOUT to Raspberry Pi pin 23
 *	MCP3008 DIN to Raspberry Pi pin 24
 *	MCP3008 CS/SHDN to Raspberry Pi pin 25
 *
 *
 *  "It is also required that the microcontroller SPI port be
 *  configured to clock out data on the falling edge of clock and
 *  latch data in on the rising edge."

 * For you guys:
 *  p15 has the pins and what the numbers for byte 1 are.
 *
 *   p19: chanel encoding for [sigl | D2 | D1 | D0]
 *
 *   p21 in docs/mcp3008.pdf gives the input/output
 *
 *  transmit:
 *      [0000 0001] [sigl | D2 | D1 | D0 | xxxx] [xxxx xxxx]
 *  
 *  receive:
 *      [xxxx xxxx] [xxxx x0 b9 b8] [b7 b6 b5 b4 b3 b2 b1 b0]
 */
#include "rpi.h"
#include "spi.h"
#include "adc-mcp3008.h"

/*
	It is also required that the microcontroller SPI port be
	configured to clock out data on the falling edge of clock and
	latch data in on the rising edge.

	p15 has the pins and what the numbers for byte 1 are.

	p21 in docs/mcp3008.pdf gives the input/output

	transmit:
	 	[0000 0001] [sigl | D2 | D1 | D0 | xxxx] [xxxx xxxx]
	
	receive:
		[xxxx xxxx] [xxxx x0 b9 b8] [b7 b6 b5 b4 b3 b2 b1 b0]

	// we send: start [sigl/diff?] D2, D1, D0
	// we get out D2, sig diff, 
 */
// p19: chanel encoding for [sigl | D2 | D1 | D0]

static int is_legal_ch(int ch) {
	switch(ch) {
	case CH0: case CH1: case CH2: case CH3: 
	case CH4: case CH5: case CH6: case CH7: 
		return 1;
	default: panic("invalid channel: %x\n", ch);
	}
}

// look at the datasheet in docs, or the following page pulls out relevant pieces:
//     http://www.hertaville.com/interfacing-an-spi-adc-mcp3008-chip-to-the-raspberry-pi-using-c.html
void adc_mcp3008_wr_cmd(uint8_t tx[], int ch) {
    // given they want to communicate over <ch> write the three
    // bytes needed into tx.
    tx[0] = ???;
    tx[1] = ???;
    tx[2] = ???;
}

unsigned adc_mcp3008_rd_cmd(uint8_t rx[]) { 
    // combine the command into a 10 bit result and return it.
    return ???;
}

unsigned adc_mcp3008_read(int ch) {
	assert(is_legal_ch(ch));

	uint8_t rx[3], tx[3];
	adc_mcp3008_wr_cmd(tx, ch);
	spi_transfer(rx,tx,3);
	return adc_mcp3008_rd_cmd(rx);
}

static void stabilize(void) { 
	delay_ms(30); 	// *seems* like we may have to let ADC settle.
	dev_barrier();
}

void adc_mcp3008_init(void) {
	printk("mcp3008: about to init\n");
	stabilize();
        	spi_init(SPI_CE1, 1024); /* 250KHz: what the pi is clocked at */
	stabilize();
	printk("mcp3008: success\n");
}
