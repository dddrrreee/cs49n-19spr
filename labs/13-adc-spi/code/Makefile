# change this name to whatever your program is called
NAME=adc-spi

# add any additional .o's here
OBJS = $(NAME).o  adc-mcp3008.o

# check that your path is defined.
ifndef CS49N_LIBPI_PATH
$(error CS49N_LIBPI_PATH is not set: this should contain the absolute path to where this directory is.  Define it in your shell's initialiation.  For example, .tcshrc for tcsh or .bashrc for bash)
endif

START = $(CS49N_LIBPI_PATH)/cs49n-start.o
MEMMAP = $(CS49N_LIBPI_PATH)/memmap

LPI_FAKE_PATH = $(CS49N_LIBPI_PATH)/../libpi-fake
LPI_FAKE = $(CS49N_LIBPI_PATH)/../libpi-fake/libpi-fake.a

# useful set of defines.
include $(CS49N_LIBPI_PATH)/includes.mk

CFLAGS += -Wstack-usage=512 -Wno-unused-function -Wno-unused-variable
CFLAGS += -Werror -O2 -falign-functions=16 -I../part1-simple 

# why do we have to do this separately?
CFLAGS += -I$(CS49N_LIBPI_PATH)/cs49n-src/

all: libpi $(NAME).bin run 

# is there a better way to do this?
libpi:
	@make -C $(CS49N_LIBPI_PATH)

.PHONY: libpi run

run: 
	pi-install $(NAME).bin

$(NAME).bin: $(START) $(MEMMAP) $(OBJS)
	$(LD) $(START) $(OBJS) -T $(MEMMAP) -o $(NAME).elf  $(LPI)
	$(OD) -D $(NAME).elf > $(NAME).list
	$(OCP) $(NAME).elf -O binary $(NAME).bin

# pull in dependency info for *existing* .o files
# *unbelievable*: make clean doesn't skip this include even though the 
# target is after.
#   https://www.gnu.org/software/make/manual/html_node/Goals.html
ifneq ($(MAKECMDGOALS),clean)
-include $(OBJS:.o=.d)
endif

# should re-do this to use the libpi makefile.

%.d: %.c Makefile
	$(CC) $(CFLAGS) -c -MT $@ -MMD -MP -MF $@ $< -o $*.o

%.o : %.c
%.o : %.c %.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<

.s.o: Makefile
	$(AS) $(ASFLAGS) $< -o $@

.S.o: Makefile
	$(CC) -c $(CPP_ASFLAGS) $< -o $@

tags:
	@rm -f tags
	ctags *.[chsS] $(CS49N_LIBPI_PATH)/*.[chsS]

# remove compilation products
clean:
	rm -f fake-$(NAME) *.o *.d $(NAME).bin *.elf *.list *.img Makefile.bak *~ tags
	make -C $(LPI_FAKE_PATH) clean

.PHONY: run libpi tags clean test
