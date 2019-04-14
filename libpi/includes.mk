
ARM = arm-none-eabi
CC = $(ARM)-gcc
LD  = $(ARM)-ld
AS  = $(ARM)-as
OD  = $(ARM)-objdump
OCP = $(ARM)-objcopy
LPI = $(CS49N_LIBPI_PATH)/libpi.a

ifndef CS49N_LIBPI_PATH
$(error CS49N_LIBPI_PATH is not set: this should contain the absolute path to where this directory is.  Define it in your shell's initialiation.  For example, .tcshrc for tcsh or .bashrc for bash)
endif

CFLAGS = -O -Wall -nostdlib -nostartfiles -ffreestanding  -march=armv6 -std=gnu99 -I$(CS49N_LIBPI_PATH) -I.
ASFLAGS = --warn --fatal-warnings -mcpu=arm1176jzf-s -march=armv6zk

CPP_ASFLAGS =  -nostdlib -nostartfiles -ffreestanding   -Wa,--warn -Wa,--fatal-warnings -Wa,-mcpu=arm1176jzf-s -Wa,-march=armv6zk  
