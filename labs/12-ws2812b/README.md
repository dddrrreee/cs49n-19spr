---
layout: page
title: Controlling a  WS2812B light string.

show_on_index: true
---

### Overview

Today you'll write the code to control a WS2812B light string.
The WS2812B is fairly common, fairly cheap.  Each pixel has 8-bits of
color (red-green-blue) and you can turn on individual lights in the array.

Each light strip has an input direction and an output direction (look
at the arrows).  You can attach multiple in series by connecting the
output of the first one to the input of the second.  They can be cut
into pieces, connected, etc.

### Check-in

Two pieces of code:

   1.  Implement the code needed for `part1-simple`; you should be able to turn on 
   the first and second pixel in the light array to the color you want.

   2.  Show you can run the code in `part2-neopixel`, which wraps the code you wrote
   in part1 into a simple interface.   Do something novel with the lights.
   

###  Part 0

To hook up the light string:
   1. Strip the red (power) and white (ground) wires and connect them
      to the 3.3v power and ground of the pi.  Unfortunately I didn't realize these
      were stranded wire, so you will have to (sadly) also strip some jumper cables and
      twist-connect them.
   2. Hook the pi's 3.3v and ground up to the female connectors on the light strip as well,
      using male-to-male to go to your breadboard.
   3. Hook up the green (signal) wire to pin 21.
   4. Bootload the program `part0-hw-test/neorace.bin` --- it should run your light string.

Note: A confusing thing is that the power for the WS2812B must be pretty
close to the power used to drive the data pin.  For long light strings
you'd want to power them with a external power supply --- however this
will require you either use a "level shifter" to raise the power used
by the pi to signal (since its pins are 3.3v for output) or a transitor
to connect/disconnect the pi 5v to the signal wire of the LED.

### Part 1: turn on one pixel.

As a "hello world" you will implement the code to turn on one pixel:
  1. Look in `part1-simple/WS2812B.h` and `part1-simple/simple.c`
  2. Much of the code is already written for you, you just have to 
     implement the few routines marked `unimplemented()`.
  3. Change the code in `simple.c` to turn on different colors and different
     pixels.
  4. If you run `check_timings` it will test how tight your routine's times are --- 
     verify these are close!  

The WS2812B's are a good example of how to communicate information
using time.  One way to communicate things fancier than on/off is to use
a fancier protocol (e.g., I2C, SPI).  Another is to just use delays ---
if you have good resolution (low variance, precise control over time and
ability to measure it), each nanosecond of delay can signify information.
The WS2812B's use a crude variant of this.

The WS2812B protocol to send a bit of information:
  1. To send a `1` bit: write a `1` for `T1H` nanoseconds (defined in datasheet), then a `0`
     for T1L nanoseconds.
  2. To send a `0` bit: write a `1` for `T0H` nanoseconds, then a `0` for `T0L` nanoseconds.
  3. To send a byte: send the 7th bit (as above), the 6th, the 5th...
  4. The timings are fairly tight.  The datasheet states +/- 150 nanoseconds.  Thus
     you will have to write your code very carefully.  You will also have to be 
     smart about measuring it --- your measurements take time and will pollute the 
     times you measure.

The protocol to set N pixels:
  1. There will be some number of pixels on each light string.  (You can count them.)
  2. Each pixel needs 3 bytes of information (G, R, B --- in that order) for the 
     color.
  3. Send each byte using the protocol above.
  4. The first 3 bytes you send will get claimed by the first pixel, the second 3
     bytes by the second, etc.


### Part 2: run the neopixel code.

Here you'll use the provided neopixel code to implement some kind of interesting
pattern:
 1. Compile the code in `part2-neopixel`.  
 2. It should compile and pass timing check.
 3. Modify the main code so that it does something more interesting.


#### Additional reading.


Some useful documents:
  * Adafruit [best practices](https://learn.adafruit.com/adafruit-neopixel-uberguide/best-practices)
  * [Protocol](https://developer.electricimp.com/resources/neopixels)
  * Nicely worked out [tolerances for the delays](https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/)
  * Nice photos and setup of a [level shifter](https://learn.adafruit.com/neopixel-levelshifter/shifting-levels)
  * Sparkfun cursory breakdown of the [protocol](https://learn.sparkfun.com/tutorials/ws2812-breakout-hookup-guide)
  * Discussion if you want to [drive many at once](https://learn.adafruit.com/adafruit-neopixel-uberguide/basic-connections)


