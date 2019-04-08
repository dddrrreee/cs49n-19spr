### Extension: hook up a push-button switch

Blinking the LED showed how to do output, we now use a push-button switch
to show how to setup your pi for input.

***CRUCIAL:***
  - ***If you haven't seen a breadboard before, make sure you read how it works***
  - ***Make sure you only connect to the 3v pin on the pi***

#### Part 1: get the hardware working.

We always try to test our hardware setup without software since doing
so minimizes the places we need to debug if things do not work. A standard
novice mistake is to wire up everything, write all the code, and ---
when it invariably does not work --  then get stuck on all the different
combinations of things that could be causing problems.  Our approach
of trying to do everything one-at-a-time, in the smallest step we can
think of, dramatically reduces the IQ needed to figure out mistakes.

When the button we use is pushed, it will connect the left pins to the
right pins (orientation: the flat part of the pins should be top and bottom).
When not pushed, nothing is connected.  See
[sparkfun](https://learn.sparkfun.com/tutorials/sik-experiment-guide-for-arduino---v32/experiment-5-push-buttons)
for a concrete example and description.  Note, that when the button
is not pushed, it's in a "floating" undefined state (likely, but not
necessarily off).  We will handle this problem later using "pullup" and
"pulldown" resistors built in to the pi.  For the moment we just live
in sin and hope for the best.

Concretely:
  - Test that you understand the wiring for the switch by hardwiring it to
  the pi 3v power and ground pins, and connecting to an LED.
  - When you push the button, the LED should go on.
  - Now add a resistor between the pin and the LED leg rather than connecting
   the LED directly and make sure your circuit still works.  We will use the resistor
   in the next step since we don't want to connection 3v directly to a GPIO pin 
   (note: we could also connect the pin on the other side of the LED since it
   is large enough to add enough resistance).

#### Part 2: get the software working:

Now that your hardware (and your understanding of how to set it up) is tested,
we write the software.

To start getting comfortable with code and Makefiles: 
  - Copy your code from part3 and rename `blink.c` as `button.c`.
  - Copy the Makefile from part3 and change it to compile `button.c`.

Write the code:
  - Implement `gpio_set_input` (should be almost identical to `gpio_set_output`.
    You will need to figure out the GPIO address used to read from GPIO pins.
    You can assume (as before) that we never use a GPIO pin higher than 31.
  - Write code to set 20 as ouput (as in part 3) and 21 as input.  Connect 20 
    to the positive end of an LED, and 21 between the switch and the resistor.
  - Write a loop to read the input pin and set the output pin to its value.
  - With your original wiring, pushing the button should make the input go high.

#### Part 3: get fancy.

Hook up multiple buttons and use them to control multiple LEDs, you can also
do simple logic operations (build OR, AND, XOR, etc).
