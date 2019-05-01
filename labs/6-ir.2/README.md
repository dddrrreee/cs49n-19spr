### Simple networking.

For this lab you're going to use a TSOP3223 infrared (IR) receiver and
a IR led to communicate with your partner's pi.    You'll first transmit
and then receive on your pi to make sure you have the hardware and your
code works, then you'll communicate with your partner.

As before, you should copy code from a recent lab into this directory, update
the `Makefile` and find the necessary datasheet online.  

### Check-off

For lab:
   - Show that you can blink a light on your partner's pi using a touch sensor.

### Simple blink.

To make sure you have the wiring right:
 - Make a dumb blink that turns on a LED whenever the TSOP receives a signal.
 - In order for you to see it, you should leave the LED on for 1 second or so
   after it hears something.


### Simple transmit


Using the IR led (blue, translucent) you will send a signal to communicate
with the TSOP.
 - You don't just turn the LED on and off.  The TSOP looks for a specific signal
   of off-on sent at a specific Khz and rejects everything else.
 - So look in the datasheet for the frequency it expects.
 - Compute the microseconds for on-off.

Compute it as:

    usec_period = 1./freq * 1000. * 1000.
    usec on-off = usec_period / 2.

Alternate sending this signal and show you can blink your partner's LED.

### Tight timing.

Print out the times that you receive the signal above.  Do what you can so its
perfectly accurate.  E.g., if a 1 is the LED sends for 1200 usec and a 0 is 2400usec
you should see values very close. 

Suggestions:
  - inline code
  - optimize more.
  - enable caching.
  - possibly add a slop factor.

### Robust transmit

Of course, everyone is sending all sorts of stuff, so you get a lot of garbage.
Let's make it a bit more robust.

You can send a 0 by writing the LED for `0_usec` and a 1 by writing for `1_usec`.
Implement:
    - a `tsop_write(v, usec)` that will do this.
    - a `tsop_read(usec_0, usec_1)` that will read and reject anything that it
    too far outside the range.

Make sure your values do not  conflict with other people.  

***NOTE***:
    - there is a minimum number of cycles that the signal has to be.  Compute this.

### Blink with touch
   
Hook up your touchpad so that:
    1. when you touch it, it sends a 1.
    2. When you stop touching it, it will send a 0.

Show that this works even when other people are doing stuff.
