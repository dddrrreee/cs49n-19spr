---
layout: page
title: SPI and ADC.
show_on_index: true
---

### Overview
You're going to implement the key functions for the mcp3008 analog to digital
converter (ADC).  It's cheap and has 8 inputs.  Pretty common.


ADC's are great b/c there are many simple, cheap devices that give out 
voltages.  Getting them working is much easier than with a digital device
(such as the echo one you messed with).

You'll also have to wire up your board for SPI.  Look at my board.
You can run the included binary to test that your wiring works.
(Turn the rotary counter clockwis, light should dim. Clockwise should increase.)

The MCP3008 code uses SPI to send a 3 byte message and receive a 3 byte reply.
There are some links in the source code.  I put in the SPI calls so you
don't have to worry about them.

You'll have to implement:

   1. the encoding for the channel enum in the header file.
   2. the adc_mcp3008_wr_cmd and adc_mcp3008_rd_cmd routines, which
	encode the 3 byte messages.

Once you get this, you can add more devices (I have a light sensor and 
a temperature control and a mic).
