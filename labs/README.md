# The labs

The labs are organized as four different chunks containing four related
labs.  Each lab will result in a new thing.  Each chunk will synthesize
these pieces together into a unified result.

------------------------------------------------------------------
### Devices (4 labs)

This chunk of the class involes you writing a few simple low-level
devices and using modern techniques to check them.

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

  2. [hall+sonar](2-hall+sonar/): we bang out two quick devices:
     a hall-effect sensor that detects proximity of a magnet, and a sonar
     device that measures distance using reflected high frequency sound.
     We work directly off of the data sheets, so you get practice
     deciphering these.    They will be confusing.  This is expected.
     You will cross check your code against everyone else.

  3. [ir](3-ir/): we use a simple IR device to reverse engineer a remote
     control protocol.    We then transmit this using an IR emitting LED
     and verify that it is accurate.  As usual, you cross check your code
     against everyone elses.

  4. [Simplification] (4-simple/).   The first draft is often pretty bad.
     We will rewrite all the previous labs as simple, as clean, as
     obviously correct as we can.  Your cross-checking will pay off:
     its embarassing to take working code, "clean it up", and break it.
     You will have a hash of the correct output, and have the makefil
     automatically recheck that your new implementation gives the
     same result.

--------------------------------------------------------------------
### Old code, new tricks.

Now that you have clean, solid code, we use tricks so that it can do new things.

  5. [network](5-esp8622/) we use the ESP8622 chip so communicate
     wirelessly.  We give you a software uart implementation to make
     this easier, you will replace this next lab.

  6. [software-uart](6-sw-uart/): you will write your own software UART so
     you see exactly how the protocol works and make it fast.

  7. [virtualization] (7-virtualization/): this lab will show how to
     virtualize hardware.  We will use simple tricks to transparently flip
     how your pi code is compiled so you can run it on Unix, only shipping
     the GPIO reads and writes to a small stub on the pi.  As a result,
     you have full Unix debugging for pi code (address space protection,
     valgrind, etc) while getting complete fidelity in how the pi will
     behave (since we ship the reads and writes to it directly).  You will

--------------------------------------------------------------------
### Interrupts

  8. [interrupts](8-interrupts/): you will walk through a simple,
     self-contained implementation of pi interrupts (for
     timer-interrupts), kicking each line until you understand what,
     how, why.  You will use these to then implement a version of `gprof`
     (Unix statistical profiler) in about 30 lines.

     Perhaps the thing I love most about this course is that because we
     write all the code ourselves, we aren't constantly fighting some
     large, lumbering OS that can't get out of its own way.  As a result,
     simple ideas only require simple code.  This lab is a great example:
     a simple idea, about twenty minutes of code, an interesting result.
     If we did on Unix could spend weeks or more fighting various corner
     cases and have a result that is much much much slower and, worse,
     in terms of insight.

  9. [using-interrupts](9-using-interrupts): you get a feel for how
    interrupts can be used to simplify code structure (counter-intuitive!)
    by adapting the interrupt code from the previous lab to make this
    code better.

--------------------------------------------------------------------
### All code is yours (4 labs)

This chunk of the class will have your write all the code that has been
a black box so far: the software UART and both the pi and Unix side of
the bootloader.

  10. [bootloader](10-bootloader/): you will implement your own
     bootloader to transfer the code from your laptop to the pi.
     The most common bootloader out there uses the `xmodem` protocol.
     This approach is overly complicated.  You will do a more stripped
     down (and I believe) more likely to be correct protocol.

  11. [checking](11-checking): you will extend the cross-checking lab to 
      handle both of these.  you will then rewrite the code to make it 
      simple.

At this point, all key code on the pi is written by you.

--------------------------------------------------------------------
### Fancier devices (4 labs)

We've been using simple devices that use some combination of off/on
and time to communicate information.  As devices do fancier things,
this simple approach doesn't work as well.  In this set of labs we do
two of the most common protocols: SPI and I2c.  In the end you combine
everything into a fancy equalizer.

   12. [SPI: adc](12-spi-adc): you implement a simple SPI device, and then implement the
   SPI protocol.

   13. [I2C](13-i2c): you implement a simple I2C microphone.

   14. [neopixel](14-neopix): you implement a neopixel device, which doesn't need a
   fancy protocol, but does require nanosecond level timing.

   15. [equalizer](15-equivalizer): you combine the microphone and neopixel
   from the previous labs to make a simple equalizer.  We use interrupts
   to get precise sound measurements so that our FFT works.

--------------------------------------------------------------------
### Final project lab (3 labs)

  - Free-form lab work on final project.

  - Final project presentations.
