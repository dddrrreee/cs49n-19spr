# The labs

The labs are organized as four different chunks containing four related
labs.  Each lab will result in a new thing.  Each chunk will synthesize
these pieces together into a unified result.

------------------------------------------------------------------
### Devices (4 labs)

This chunk of the class involes you writing a few simple low-level
devices and using modern techniques to check them.  Some of these labs
are quick (< 60 minutes), some are longer.

  0. [blink](0-blink/): get everyone up to speed and all necessary
     software installed.  You do the usual `blink` program by writing your 
     own GPIO implementations based on the Broadcom document GPIO description.

  1. [cross-check](1-cross-check/): you will use read-write logging
     to verify that your GPIO code is equivalent to everyone else's.
     If one person got the code right, everyone will have it right.

     A key part of this class is having you write all the low-level,
     fundamental code your OS will need.  The good thing about this
     approach is that there is no magic.  A bad thing is that a single
     mistake makes more a miserable quarter.  Thus, we show you modern
     tricks for ensuring your code is correct.

  2. [hello][2-hello/): As you've noticed, debugging without even having
     `printf` is a hassle   Before we do a bunch of devices, we first
     get `printf` working by writing a couple GPIO functions and then
     putting then `libpi`.

  3. [hall](3-hall-effect/):  for this lab you'll bang out your first
     device: a hall-effect sensor that detects proximity of a magnet.
     We work directly off of the data sheets, so you get practice
     deciphering these.    They will be confusing.  This is expected.
     You will cross check your code against everyone else.

  4. [sonar](4-sonar/): for this lab you'll write a more complicated
     driver to measure distance using sonar.  Again, we'll work directly
     from a datasheet.


  5. [ir](5-ir/): we use a simple IR device to reverse engineer a remote
     control protocol.    We then transmit this using an IR emitting LED
     and verify that it is accurate.  As usual, you cross check your code
     against everyone elses.
