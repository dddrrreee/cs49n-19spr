---
layout: page
title: Sensor party
show_on_index: true
---

### Overview
	
For this lab you're going to implement two sensor drivers on your own ---
a simple magnetic sensing device (a "hall effect" sensor) and a distance
measuring device that uses sonar.

There's some simple skeleton code, but you're going to do all the work
yourself, including getting the data sheets and anything else you
need.  This process will foreshadow what you'll have to do when you
write drivers for devices on your own (with the simplification that
in these two cases we know its possible to get the needed information!).

### Implementing code for the hall effect Allegro A1104 sensor

First, find the data sheet.   Google search for 'allegro A1104 datasheet'
or some variation of that.  You're looking for the manufacturer's doc for
what to do to use the sensor.

When you find it, you'll notice it's fairly confusing.  This is the norm.
The game is to skim around ignoring the vast majority of information
(which will be irrelevant to you) and find the few bits of information you
need: 

  1. What the pins do (input power, output, ground).
  2. What voltage you need for power (`Vcc`).
  3. What reading you get when the the device is "open" (not signaling
	anything).
  4. what reading you get when the device is closed / signaling (`Vout`).

If any of these values are out of range of the pi, you can't connect
directly.

Notice what the range of values are: if "low" is close to zero, but not
guaranteed to be zero, you'll have to set the input pin as a `pulldown
resistor` to clean up the signal.

1. Write the code to configure the GPIO pins as needed.   You can
	use the functions listed in `cs49n/libpi/gpioextras.h' initially.

2. Test with a magnet.

Troubleshooting:
  1. Check wiring.   Check if sensor is reversed.
  2. Where you touch the sensor matters.
  3. The magnet orientation matters.
  3. The input voltage matters.

### Implementing the HC-SR04 sonar driver.

Again, get the datasheet.  The first one is a 3-page sheet that isn't
super helpful (common); there's a longer one if you keep digging.
Also, the sparkfun comments for the device have useful factoids (but
also wrong ones, which --- you guessed it --- is common).

Again, you're going to be skimming through the data sheet looking
for the information you need.

Some hints, you'll need to:
   0. do the pins need to be set as `pulldown`?

   1. init the device (pay attention to time delays here)

   2. do a send (again, pay attention to any needed time 
   delays)
        
   3. measure how long it takes and compute round trip
   by converting that time to distance using the datasheet
   formula
        
   4. again, you can start with the code in 
   `gpioextra.h` but then replace it with your
    own (using the broadcom pdf in the docs/ directory).
         
Troubleshooting:

  1. readings can be noisy --- you may need to require multiple
  high (or low) readings before you decide to trust the signal.

  2. there are conflicting accounts of what value voltage you
  need for `Vcc`.

  3. the initial 3 page data sheet you'll find sucks; look for
  a longer one. 

### Advanced.

Write versions of all the code you used from `gpioextras.h`.
You should be building up a GPIO library that can do most of 
what the pi provides.
