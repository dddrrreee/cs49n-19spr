---
layout: page
title: Using I2C
show_on_index: true
---

### Overview

Today we're going to communicate with an accelerometer (the LIS3DHS) using
I2C.   These devices are useful for controlling stuff based on motion.

    [adafruit product page](https://www.adafruit.com/product/2809)
    for the LIS3DH Triple-Axis Accelerometer (+-2g/4g/8g/16g).

While many cheap devices use SPI, it has a lot of wires.  I2C just
needs two.  At a high level I2C devices have a set of (perhaps pretend)
8-bit registers that you use I2C to read from and write to.
  - reads: give the device address you want to talk to (there can be 
   multiple on the same wires) and the register number you want to read,
  get back an
  8-bit quantity.  For example: `v = i2c_read(addr, reg)`.

  - writes: give device address, register number you want to write to
    and 8-bit value you want to write.
	For example: `i2c_write(addr, reg, v)`.

### Readings

Some useful links:
   - Background for the LIS3DHS accelerometer: [application note]
      (docs/en.CD00290365.pdf) gives examples for how to use the chip.
	The full spec is --- [chip spec](docs/en.CD00274221.pdf) ---
	which goes into more details, but is harder to understand.

   - General I2C background: [sparkfun tutorial](https://learn.sparkfun.com/tutorials/i2c)

   - I found the ESP RTOS code easy to follow: [code]( https://github.com/SuperHouse/esp-open-rtos/tree/master/extras/lis3dh)

### What to do

First wire up your pi to the accelerometer. The program `accel-binary.img`
in code is a working binary.  Use it to test your wiring.

The general code is setup for you, you'll have to go through and implement
anything marked `???`.

   1. First write the code to ping the device verify it (1) responds and 
	(2) gives the id you expect.

   2. Finish writing the code to config to device.

   3. Finish writing the code to read data 

It's setup so you can just uncomment out more and more code to 
make progress.

If you finish, there's lots of tricks to play.  E.g.,:

  - Change the scaling for gravity (should see the numbers halve if you 
	double the scale, etc)
  - Change from low power to high power.
  - When reading data, can do a fancy i2c read that is basically a memcpy 
  - Convert to float (not you'll have to recompile for floating point).
