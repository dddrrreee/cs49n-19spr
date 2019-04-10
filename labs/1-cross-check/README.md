---
layout: page
title: Quick overview.
show_on_index: true
---

Lab: automatically cross-check your gpio against everyone else's.
======================================================================

A goal of this course is that you will write every single line of
(interesting) low level code you use.  A good result of this approach is
that you will understand everything and, unlike most OS courses, there
will not be a lot of opaque, magical code that you have no insight into,
other than sense of unease that it does important stuff beyond your ken.

An obvious potential result of this strategy is that since you are writing
all code your system depends on, a single bug can make the rest of the
quarter miserable.

This lab will have you build the inital pieces needed to automatically
check that your homework code is correct.

After completing the lab you will:

  1. Be able to check that your `gpio` code is equivalant to
     everyone else's in the class by tracing all reads and writes
	 it does and comparing them to everyone else's implementation.
	 If even one person gets it right, then showing equivalance means
	 you got it right too.  And, nicely, automatically detect if any
	 subsequent modifications you do break the code.

   2. Be able to quickly, correctly write a more full-featured
	`gpio` implementation by checking its equivalance against the
	fairly complicated code used in the initial cs107e class.

After this lab you can then most of the rest of the homework without too
much issue.

***Important***:
   - I know some of you modified your blink code last time without doing a
   `git pull` first.  This will lead to conflicts.  These can be resolved
   and you should figure out how to do so.  However, since we only have
   two hours,  in the interests of time, it may be more efficient to
   instead do a clean checkout and copy your working code into this lab.

#### Sign-off

   1. Your `gpio_set_output`, `gpio_set_on`,
   `gpio_set_off` give the same result as other peoples.  You can
   run them in this order and just hash the end result.

   2. Add an implementation and check for `gpio_set_input` and verify
   you get the same result as everyone else.

#### 0. Seperate out your gpio code from `blink.c`

In order to make testing cleaner and for later labs, we need to pull your
gpio code out of `blink.c` into its own implementation file `gpio.c`.
As a small bonus, doing so will get you more familiar with Makefiles
and header files, which will help your whole career.

Mechanically:

  0. We are going to modify your Lab 0 code, so first copy the files in
     `0-blink/blink3` directory into `1-cross-check/blink`.

     My general suggestion: (almost never) update-in-place a working
     implementation to add new functionality.  Instead, make a copy
     (so you always have a working, internally consistent version) and
     modify the copy instead.  Some people will rely on `git commits`
     and rollbacks, but I have seen this go very awry.  (Intuition: It's
     safer, simpler, more robust to have a seperate working copy than
     assume you can take a non-working copy and apply a set of verbs ---
     e.g., `git` rollbacks --- to get to something working.)

  1. Write a header file `gpio.h` with the `gpio` prototypes needed
     by `blink.c`

  2. Put all `gpio` function implementations  in a file `gpio.c` and 
     remove them from `blink.c`.

  3. Alter your `Makefile` to compile and link the new file together.

  4. ***IMPORTANT***: rerun your blink to make sure it still works!

#### 1. Make a fake implementation of `put32` and `get32`

In order to make testing easy, we want to be able to run your r/pi code ---
unaltered --- on your Unix laptop.  What do you need to do so?  

  1. Compile it for Unix.   Fortunately, because of the way we wrote `
     gpio.c` you can easily compile it both for RPI and to run on your
     Unix laptop by simply switching compilers.  

  2. Override what happens when it writes to GPIO memory or calls ARM-specific
     assembly code.   

     Since your `gpio.c` is simple, all you'll have to do is provide fake
     implementations of `get32` and `put32` so that we can override what
     happens when your code writes to GPIO memory.

First steps:
  0. Do a pull if you haven't already. 
  1. Look in `fake-put-get.c` and read the comments.  You will implement
     `put32` and `get32`.
  2. Before you start, run `make` and make sure everything compiles.
     Note: the code will use your `gpio.h` and `gpio.c` from Part 0 so
     if you get compile errors it's probably because you have stuff in the 
     wrong place.

You'll now build a fake memory so that you can implement `put32` and
`get32` (again, look in `fake-put-get.c` file).

For `put32(addr,v)`: 

	1. Create an entry for `addr` in your fake memory if `addr` doesn't exist.

	2. Write `v` to `addrs` entry.

	3. Call `print_write(addr,v)`.

On `get32(addr)`:

	1. If `addr` does not exist, insert `(addr, random())` (but do not print anything).

	2. Get the value `v` associated with `addr`.

	3. Call `print_read(addr,v)` to print out the address and value.

	4. Return `v`.


To test it:
	1. Run `./test-put-get > out` on Unix.	It should run without
	   crashing and, importantly, print out the values for each
	   `put32` and `get32` in the exact order they happened.
    2. Get the checksum of the output (`cksum out`) and compare to your partner.
    3. If these values match, you know your code worked the same as your partner's.
    4. Now post to the newsgroup so everyone can compare.
    5. If everyone matches, and one person got it right, we've proven that
       everyone has gotten it right (at least for the values tested).

#### 3. Check your code against everyone else (5 minutes)

After you checked your fake `put32` and `get32` we now want to check that
your `gpio` code works the same as everyone else.  Given the `get32` and
`put32` modifications above, a simple, stringent approach is to check
that two `gpio` implementations are the same:

	1. They read and write the same addresses in the same order with
	the same values.

	2. They return the same result.    (For our implementations we
	did not return any result, so this just means that your code
	never crashes.)

If both checks pass then we know that both implementations are equivalent
--- at least for the tested inputs.

For this section:
	1. Uncomment out the rule for `test-gpio` in `Makefile` and run `make`.
       This will compile the test harness `test-gpio.c`.

    2. You can test each function individually by running `test-gpio 0`,
       `test-gpio 1`, etc.  (Look in the `test-gpio.c` file.)

    3. Again compare the results to your partner and post to the newsgroup.


#### 4. Add `gpio_set_input` and cross-check it.

You'll see we can write an implementation even without having a way to run it.
   1.  Implement the `gpio_set_input` function: add it to `gpio.c` and its prototype 
   to `gpio.h`.
   2. Add a test to the test harness.
   3. Make it so that `test-gpio 4` runs it in isolation and `test-gpio 5` runs
      all tests.
   4. Cross-check your results.
