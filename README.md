---
layout: page
title: Overview
show_on_index: true
---

#### Precis

This is a lab-based class.  We will do two 2-hour labs each week.
You should be able to complete almost all of the lab in one sitting.

You will write "bare-metal" code  (i.e., no OS, no other code besides
yours and some minimal starter code from us) to control as many hardware
devices and/or do as many low-level tricks on a raspberry pi A+ as we
can fit in the quarter.

After this quarter, you'll know/enact many cool things your peers do not.
You will also have a too-rare concrete understanding of how computation
works on real hardware.   This understanding will serve you in many
other contexts.

We try to work directly with primary-sources (mostly: the Broadcom manual
and the ARM6 manual, both in the `docs/` directory) since understanding
such prose is one of the main super-powers of good systems hackers.

Prerequisites:
   1. A MacOS or Linux machine.  Not windows, not Windows running VMare.
      We've had too many issues.

   2. Able to install and setup the tool chain.  See [Lab1](labs/lab1/README.md).

   3. Gone through K&R at least twice, including once recently.

Who this class is for:
   1. If you are good at coding or, not necessarily good at coding now, but have a lot
      of time to spend on this class AND
   2. You are inherently interested in figuring out how things work.

The people that found the class valuable in the past were entirely drawn
from this demographic.  I would say that if you struggle building stuff
and/or are not already interested in the topic, then it's probably better
to take a different class.

Before class:

   0. Clone this repo: `git clone git@github.com:dddrrreee/cs49n-19spr.git`.
   You may need to install `git` on your laptop.

   1. Make sure you have some way to write a micro-SD card and understand
   how to do so.  Note that newer Macs do not have SD card slots so you
   will need an adaptor.  Make sure when you plug in an SD card you know
   how to find where it is mounted. 

   2. Make sure you have some way to plug in a USB adaptor.   Note,
   as above, newer Macs do not have USB ports so you may need a USB-C
   to USB adaptor.

   3. Figure out how to add a directory to your `PATH`.  This will
   be valuable all quarter. The exact mechanics are shell-dependent.
   For `bash` look in `.bashrc`, for `tcsh` look in `.tcshrc` etc.

Grading:

   1. Each lab will have a check-off that you need to complete, if not
   in the lab itself, within 7 days.

   2. There is a final project.  You will have an expense account at
   `sparkfun.com` and `adafruit.com` to buy whatever devices you like
   and build something interesting.

   3. Participation: if you have something to say, can help people out,
   figure out interesting things, etc you'll do fine.

   4. Class is not optional: from past experience, if someone falls
   behind, it's very hard to catch up.  So other than a few pre-excused
   absences, you must be at each lab.

### Overview

Rather than me talking alot, this class will mainly be you doing alot.
The trouble with talking classes (I've taken and taught many) is that
it's easy to fool yourself into thinking you understand something,
but when you open a code buffer and have to actually type the next
language token, you realize that, no, you do not.  Talking classes are
also typically boring.  We will go the other way: you're mostly going to
write code and wire up hardware since it absolutely requires you know
what the next thing to do is.  In addition, the labs are deliberately
structured as a sequence of concrete problems, which makes it easier to
get in a hyper-focused flow state since there's always the next thing
to do: hard enough to be interesting, not so hard you get tired.

Each class you will come in with nothing other than some background
reading, hack for a few hours and then have a new trick or ability
that you didn't before.  I've tried to focus on topics where if you do
them on your own it's easy to get stuck since you are missing a single
key fact.  In the lab setting you will do the interesting work yourself,
but we will be able to supply the few sentences that make the difference
between completing in a couple of hours vs perhaps days.

This class goes faster than the typical freshman seminar, and there is
significantly more coding.  The benefit is that at the end you will
have a much better grounding in what is actually going on when code
runs on a real machine.  You will also have abilities most of your peers
(at least at Stanford) lack.  It's exactly the kind of thing you should
learn early, since it will pay off in each subsequent year --- the longer
the rectangle, the larger its area.  This is a class I wish I had taken
when I was younger.

As stated above, this class is structured where you will write the code
to control 1-2 hardware devices (sonar, accelerometers, IR sensors, etc)
each week.  This code will be "bare metal" in that there is no operating
system, and no other code besides what you write.  Some benefits of
bare-metal:

   1. You don't have to waste time fighting a massive OS that won't get
    out of it's own way.

   2. You have total control over the machine, and can do things
   infeasible on a general purpose OS (e.g., it's easy to do
   nanosecond-accurate timed operations).

   3. There is no mystery: all code is yours.   The grounding you gain
   from getting to the "that's all there is?" level is non-trivial.
   It's no longer "turtles all the way down" but, rather, a few assembly
   files you've written yourself and could rewrite for a new machine
   without much drama.

We use the Raspberry pi A+ because it has a relatively small form
factor, is relatively cheap, and has reasonable documentation (the
ARMv6 manual, the broadcom docs, and blog posts).  Despite it's somewhat
hobby-ist/cute-sy reputation among the hard-core, the issues you deal with
here show up on the rest of the alternatives, and once you understand
how to do things here, you should be able to do them on other hardware
platforms.

The labs are [here](labs/README.md).  They will fluctuate depending
on the experience of people in the class, or what makes sense based on
interest level, but the ones there should give you a rough feeling of
what we will be doing.
