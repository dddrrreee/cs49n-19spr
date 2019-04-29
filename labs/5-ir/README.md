### Simple networking.

For this lab you're going to use a TSOP38238 infrared (IR) receiver and
a IR led to communicate with your partner's pi.    You'll first transmit
and then receive on your pi to make sure you have the hardware and your
code works, then you'll communicate with your partner.

As before, you should copy code from a recent lab into this directory, update
the `Makefile` and find the necessary datasheet online.  

### Check-off

For lab:
   - Show that you can blink a light on your partner's pi using a touch sensor.

### Local blink.

First, get the TSOP datasheet online.  Figure out:
  1. How to wire the device.
  2. What power it needs.
  3. Wire it up, and wire up the IR LED.

Second, implement two functions:
 1. `tsop_init(input)`: called with the GPIO pin to use for input.
     It sets these up and initializes the device.

 2. `tsop_read(input)`: returns if the device is receiving a signal or not.
    You may have to read multiple times to ensure a stable value.
    (Note: make sure you figure out what the default value is!)

Third, do a blink on your own pi:
 1. Turn the IR LED on/off and read from your TSOP.
 2. Hook up a normal LED and turn it on when you read a signal from the TSOP.

Trouble-shooting:
 - IR is very directional and easily blocked.  If you don't get signal it can
   be (1) because the LED is blocked or (2) the TSOP is not pointing at the LED.

### Remote blink

Get a partner:
  1. Flip a coin: loser has to send a signal, winner received and blinks the visible
     LED.
  2. Switch, to make sure it works.
  3. Now hook up the touch sensor and send a signal when contact is made.
