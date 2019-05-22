---
layout: page
title: doing wifi with an esp8266
show_on_index: true
---

### Overview

Big picture for today's lab:
   0. Get your ESP8266 device configured with your own private network.
   1. Communicate with it.

Documents for the device are in the `10-esp8266/docs/` directory.  We communicate with
the device using `AT` commands, so the example document is a good place to start.

### Deliverables:

The above.


### How to start

What to do:
   1. Look in `esp-uart/server.c`.
   2. Run `part0()` and see that you get something.  You should see:

            AT
            
            OK
            esp-uart: Done!

   3. Modify the code as instructed.
