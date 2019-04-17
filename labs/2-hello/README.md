## Hello r/pi.

As you've probably discovered, debugging without even `printf` is
a hassle.  Before we do a bunch of devices, today's lab first gets
`printf` working so that it's a bit easier.

For historical reasons we call `printf` `printk` since it's running at
privileged level --- in that errors crash the machine versus causing a
segmentation fault --- and our implementation isn't quite `printf`.

Getting `printk` setup will involve doing a 
few steps.  The big picture:
  1. Setup and compile the new `libpi`.
  2. Write a GPIO function that `printk` needs.  
  3. Compile and run `hello` in today's lab (`2-hello/hello`).
  4. Profit.

As a bonus these steps will re-enforce using Makefiles and standard Unix
commands.  Especially: 
 1. If you can't find a file, try to find it using `ls`, `find`, etc.   
 2. Related: If your editor supports it, use tags.    This allows you to 
    simply type a command sequence and jump to the function's or variable's
    definition.
 3. Get used to working in multiple directories: I would strongly,
    strongly encourage you to make doing so much easier by having two
    terminal windows up side by side so you can easily operate in
    multiple contexts.

#### Check-off:

Show:
 1. That you wrote cross-checked your `gpio_set_function`.
 2. That `hello` works.

----------------------------------------------------------------------
#### Step 1: update `libpi` (10 minutes)

Do a `git pull` in your `cs49n` directory, you'll notice there's now
a bunch of code in `libpi`.   You should look through them to see what
you have to work with.

The key pieces:

  - `rpi.h`: this has all the prototype definitions of the provided functions.  It
   may look like a lot, but they are largely pretty superficial.

  - `cs49n-libc`: `.c` implementation of standard library functions
    (often ripped off from `uClibc`).  These include `memcpy`, `memset`
    and, of course, `printk`.

    These implementations are almost entirely generic and contain nothing
    specific to the pi itself.   If you need a missing `libc` function,
    you can just drop a generic one in here.

  - `cs49n-src`: these are pi-specific source files.  Ideally, you'd
    write everything yourself, but since the quarter is short, we give you
    some that are not that interesting to do on your own.  These include
    the `put-get` implementations, `reboot`, and some timer code (in
    `timer.c`).

  - `cs49n-objs`: these are `.o` files we provide.  These are actually
    somewhat interesting, and we may have you write them.  The only thing in their
    now is the `UART` driver, which initializes the pi side to communicate with
    your TTL driver.  You've already been using it.  Today is about getting it to
    work with your `GPIO` implementation.

  - `objs`: this is where all the compiled code gets put.

----------------------------------------------------------------------
#### Step 2: compile `libpi` (10 minutes)

This mainly involves setting the `CS49N_LIBPI_PATH` environment variable, which 
shouldn't be too hard:

   1. `cd` into libpi
   2. `pwd` --- the path this prints out is what you should set `CS49N_LIBPI_PATH` 
      to.
   3. If you use `bash` edit your `~/.bash_profile` and add `export
      CS49N_LIBPI_PATH <path>` where `<path>` is the path from step 2.  If you 
      have `tcsh`, do something similar to `.tcshrc`.
   4. `source ~/.bash_profile` in every single terminal you're going to use or just
      open new ones.
   5. Go back to `libpi` and make it.  It should compile without errors.

----------------------------------------------------------------------
#### Step 3: implement `gpio_set_function` and cross-check it  (40 minutes)

This is the one step where you write some code.  But it's mainly just adapting
the GPIO code you already implemented.

In order for `printk` to work, we have to initialize the UART so that
`printk` can use it to communicate with your laptop(*).  

One way to see what we need is to simply run 
`arm-none-eabi-nm cs49n-objs/cs49n-uart.o` in the `libpi` directory.  It will
print out the symbols defined and referenced in `cs49n-uart.o`.  In our case, it
should print:

             U dmb
             U dsb
             U gpio_set_function
    0000006c T uart_getc
    00000000 T uart_init
    00000090 T uart_putc

The `U` indicates the symbol is referenced by `cs49n-uart.o` but not defined yet:
`dmb`, `dsb` (we provide both in libpi) and `gpio_set_function` which you implement.
The `T` denotes the functions that the file both defines and exports:
`uart_getc`, `uart_init`, and `uart_putc`.

Since it needs `gpio_set_function` let's implement it.  This is just a
more generic version of your `gpio_set_output` and `gpio_set_input`.  You can
see the interface description in `cs49n-src/rpi.h`:

    // set GPIO function for <pin> (input, output, alt...).  settings for other
    // pins should be unchanged.
    void gpio_set_function(unsigned pin, gpio_func_t function);

Write this code, and cross-check it against everyone on Unix.

(*) Strictly speaking, we do not actually have to initialize the UART
if we used the bootloader to run a program: can you say why?).

----------------------------------------------------------------------
#### Step 4: hello (10 minutes)

Now we get `hello` to run.  This should be trivial.

First set-up `libpi` to use your `gpio.c`:
   1. Copy `gpio.c` into `libpi`.
   2. To minimize `git` conflicts, the file `put-your-src-here.mk`
      will holds a list of all the files you add over the quarter.
      For today, simply uncomment the `gpio.o` in it by removing the `#`.
      The `Makefile` is setup so that it will now automatically compile
      and include your `gpio` in `libpi`.

   3. Run `make` in `libpi`: `libpi.a` should build without errors and a
      `gpio.o` should be in the `objs` directory (verify this!).

Second, compile `hello`:
   1. Go to `2-hello/hello` and run `make`.  This should produce a `hello.bin`.
   2. Send it to the pi with the bootloader: It should print `hello` and then exit.  Run it a few
      times to make sure everything is ok.
   3. DONE!

You're now in a much better situation to write a bunch of device code.

At this point you probably understand why we had you do all this cross
checking --- if any of your GPIO code was slightly off, then `printk`
would mostly work, but sometimes not.   This sorta-right state is often
the worst thing possible. Consider what happens if:
  1. You have a bug in your general code and use `printk` to try to isolate it;
  2. `printk` prints the wrong values (or doesn't print anything);

Then:

  3. You'll think your code is computing the wrong thing when it's not
    (leading to chaos) or that it crashed when it didn't (causing you
    to look in the wrong place).

In general, we prefer bugs that fail spectacularly as soon as we hit
them, so that we: (1) cannot possibly miss there was a bug and (2)
know exactly where it is.  Otherwise, it can take days (or longer)
versus minutes to find it.  

----------------------------------------------------------------------
#### Step 5: make sure `blink` still works (10 minutes) 

This should be fast.
   1. Make a `blink` directory in today's lab.
   2. Copy `hello/Makefile`  to `blink`: `cp hello/Makefile blink`.
   3. Modify `Makefile` to compile `blink`.
   4. Copy your `blink.c` over.  Change it to `#include "rpi.h"`.
   5. Get rid of any unneeded code and add a call to `clean_reboot()` at the end.
   6. Compile and run it!

This is a common pattern: every time we do a new thing, make sure old
things work.

----------------------------------------------------------------------
#### Extra

If you finish:

  1. Rewrite your code to be the simplest, cleanest code you can.
     At each modification recheck it against the old checksum values or
     to just the raw output.  (Another benefit of doing modifications
     the way we did: it makes it trivial to compare old code to new.)

  2. Start to understand all the code you use.  Look at `cs49n-start.s`
     which is the assembly that starts up the program, `cstart.c`
     and then the executable code in `hello.list`, which we produced
     by disassembling `hello.bin`.  You'll see that the linker script
     `memmap` defines the variables used by `cstart.c`.
