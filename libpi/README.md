Basic idea:
  - Put your source code files in this directory.
  - Put their names in the `put-your-src-here.mk` so that the `Makefile`
  will compile them.
  - Don't modify the `Makefile`.

The subdirectories hold `.c` and `.o` files we give you:
  - `cs49n-objs`: these are `.o` files we provide.  Feel free to implement your own!
  - `cs49n-src`: these are pi-specific source files.  You can write your own, but 
    these are not super interesting.
  - `cs49n-libc`: `.c` implementation of standard library functions (often ripped off from
    `uClibc`).
  - `objs`: this is where all the compiled code gets put.
