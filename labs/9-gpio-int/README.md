---
layout: page
title: Using interrupts to detect GPIO events.
show_on_index: true
---

### Overview

Most likely, you'll want to use interrupts to wake you up when a GPIO pin gets an event.  
We'll build that today by setting up GPIO interrupts for "rising edge detection"
(when a pin goes from 0 to 1) and "falling edge detection" (when a pin goes from 1 to
0).

You'll use your touch sensor.  There is some trivial starter code in `gpio-int`.

Check-in:

 - Show that you can get interrupts for rising and falling edges.

 - Extend you LED to get dimmer when you touch a left sensor and brighter when you touch 
   a right sensor.

### Part 1: GPIO interrupts.

You'll implement four functions:
  - `gpio_int_rising_edge(pin)`: enabling rising edge detection for `pin`.  Page 97
    discusses it and the address to write to is on page 90.

  - `gpio_int_falling_edge(pin)`: enabling rising edge detection for
  `pin`.  Page 99 discusses it and the address to write to is on page 90.

  - `gpio_event_detected(pin)`: returns 1 if an event was detected (p96).

  - `gpio_event_clear(pin)`: clears the event set on `pin` (p96: write the same
    address as `gpio_event_detected`).

Don't panic.  These are pretty much like all the other GPIO functions you've built: 
get an address, write a bit offset.   No big deal.

However, just as with timer interrupts, enabling is a two-step process: we first
enable the specific GPIO interrupt(s) we care about using the first two routines
above, and then tell the pi that we care about GPIO interrupts generally.  If you 
look on page 113:
  - We want to enable `gpio_int[0]`, which is `49` in terms of general interrupts.   
  - To set this, we bitwise-or a 1 to the `(49-32)` position in `Enable_IRQs_2`
  (which covers interrupts `[32-64)`).

This appears magic, especially since the documents are not clear.  But,
mechanically, it's just a few lines.   You should look at the timer
interrupt code to see how to set the general interrupts.

At this point you'll have interrupts enabled.

The interrupt handler works by checking if an event happened, and then using the 
`pin` value to determine if it was a rising or falling edge:
    
    if(a GPIO event was detected for pin) 
       if <pin> = 0, then
           it was a falling edge (a transition from high to low, i.e., 1 -> 0).  
       else 
           it was a rising edge (a transition from low to high).
      clear the event


As with timer interrupts, you need to clear the event that generated the interrupt.

### Part 2: Use interrupts to PWM smoothly.

Use the interrupt to vary LED brightness based on how long someone touches a touch
sensor.  Also use it to hook up two different touch sensors and use one to lower,
the other to raise.
