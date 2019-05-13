---
layout: page
title: Using interrupts to blend sonar with PWM.
show_on_index: true
---

### Overview

Interrupts can make your code a mess.  Used in a narrow, limited way, they can make it
simpler.  We're going to try to do the latter today.   

In general, doing two tasks at the same time makes your code fragile.  As one recent
example, as you may recall from the sonar lab, simultaneously varing an LED intensity 
using PWM while measuring sonar is difficult:
   1. PWM requires turning the LED on/off at regular, precise intervals.
   2. The sonar code does big, unpredictable delays while it measures.
Integrating these two in one thread of control is tricky.  (There is a trick for this
case; if you finish the lab LMK and I'll explain enough to get you started.)

However, with interrupts it can be pretty easy, especially if one of
your tasks is short (as in the case of the PWM --- turn on or turn off).
You'll: 

  1. Every 10ms use your sonar code to measure distance.
  2. Use this distance to set PWM intensity from `0..20` (farther = fainter).
  3. In the interrupt handler, turn the LED on or off based on the intensity measure.

You copy the timer code and adapt it.

Check-in:

 - A smoothly scaling LED that uses interrupts to smoothly dim/brighten 
 with distance.

 - Extension (harder): have the sonar functionality use the interrupt
 rather than the LED.  This will require figuring out how to get
 interrupts from GPIO pins.

### Part 1: smoothly scaling an LED.


Before you do anything with PWM, let's first make sure you can vary an LED smoothly.
Write code to scale it from an intensity of `0..60` in 1 second jumps.   You can look at
a PWM duty cycle as approximating some ratio `p/q` and at each step deciding to turn on
or off depending on which action minimizes the error.  For exmaple, if you want to do
a 70% PWM after `n` steps then turn it on if:

        abs(7/10- on/n) > abs(7/10 - (on+1)/n)

And leave it off otherwise.  At the moment we don't have floating point,
so we need to multiple by `q*n` to simplify:

        abs(p*n - q*on) < abs(p*n - q*(on+1))

### Part 2: Use interrupts to PWM smoothly.

You need a way to have one job to run while another is hogging the CPU.
Make a copy of the code from last interrupt lab and do the PWM work in the
interrupt handler and reset the PWM values in the sonar thread every 100ms.

### Extension: Use GPIO interrupts for sonar

Rather than just put the LED pwm in your interrupt handler, you can
put the sonar functionality in there.

   1. Periodically you will trigger the sonar (say every 100ms).
   The code for this is in the interrupt handler, and just checks
   how long since it was last triggered and, if more than 100ms
   starts.  This code should be very very short.

   2. You use the pi to generate an interrupt when the echo 
   input goes from low (no signal) to hi (signal).  

Making a GPIO pin generate an interrupt is useful for many other
things in the class, so this is good functionality to figure.

First, configure the pi to:
   1. Use the pin as an `input` and as a `pulldown` (so you don't pick
   up random environmental noise from the pin, which can act as an 
   antennae).

   2. Have the pin raise an interrupt when it detects a rising edge.

   3. As with the timer, we also have to enable general interrupts,
   so configure the general interrupts (chapter 7) to be tied to 
   the GPIO pin you picked.  The IRQ table on page 113 is kind of 
   confusing, so you'll have to think about what it probably means
   (hint: 49).

   4. In the interrupt handler: you'll have to figure out if your
   pin caused the interrupt (`EVENT_DETECT`) and, then, clear it.
   
Test this code by running power through an LED (to bring down the current)
and touching your input wire to the LED leg.   If you print out in the
interrupt handler when it triggers, this should match when you touch.
Singe speed of light is much faster than people, you may get multiple
prints for a single touch.

Now restructure your sonar code to use this interrupt to detect when
an pulse has returned and caused echo to go high.  This is actually the
easiest part.
