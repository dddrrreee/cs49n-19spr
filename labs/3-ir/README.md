---
layout: page
title: IR sensing.
show_on_index: true
---

### Overview
	
Today we're going to (try to!) use an IR sensor to reverse engineer the
key presses from a remote so that you can control your pi using one.

We are using the Vishay TSOP 34838 IR sensor.  The datasheet is in the
directory.  It has a pretty similar interface to a hall effect sensor.

First steps:
   1. Write the code to read from the input pin.  
  

   2. Remotes communicate by sending values for a speicific amount of
   time.  So to start off record the value sent and the time it was sent.

   3. COMMON MISTAKE: don't print *while* timing, since that messes up
	the timing.  Instead when the IR has been low for "a long time",
	emit all the timings you've seen.

   4. A key press transition is typically signaled with a 
	"very long" on-value.
       Then a set of timed off/on pulses.    You should be able to 
	see this pattern in the values you print and it should be 
	"somewhat" stable.

### How they send values

On the remotes I have for class it looks like they send a 0 or 1 bit
by always sending 1 but for differing amounts of time.   They seperate
the bits by sending 0 for a set amount of time.  (You might read up on
Manchester encoding.)

For these remotes:

    - skip: send 0 for about 600usec.
    - 0: send 1 for about 600usec.
    - 1: send 1 for about 1600 usec.

So to send 01 they would:

    1. Send a skip (0 for 600 usec).
    2. Send a 0 (1 for 600 usec);
    3. Send a skip (0 for 600 usec);
    4. Send a 1 (1 for 1600usec).
    5. (maybe) send a skip (0 for 600usec).

It also looks like they indicate the start of a transmission by sending
1 for about 40,000 usec and then an initial value.

So given this information you should be able to reverse engineer the
value for each key.
   1. pick up the header.
   2. read the values.
   3. convert the values to 0 and 1, shifting them into an unsigned
  
The main issue is handling error.  We'll just do something simple:
   1. When deciding if a value is a skip, return true if it's within
   a given number of usec (e.g., if its between 400 to 800usec).

   2. When deciding if transmission was a 1 or a 0, pick the value by 
   deciding if its above or below the halfway point between the two 
   values (e.g., if the timing is above (1600+600)/2 then its a 1 and
   is a 0 otherwise).

   3. If you are more clever than this somewhat brain-dead approach,
   be more clever!
