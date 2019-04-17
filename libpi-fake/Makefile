# CHECK = ../../../staff/3-hall-effect/hall-effect-a1104/hall.c
# CHECK = ../../../staff/4-sonar/sonar-hc-sr04/sonar.c

NAME = libpi-fake.a 
CFLAGS = -Wall -Og  -I$(CS49N_LIBPI_PATH)
CC = gcc
SRC = fake-pi.c   fake-gpio.c driver.c
OBJS = $(SRC:.c=.o)

all : $(NAME)

$(NAME): $(OBJS)
	  ar cr  $(NAME) $(OBJS) 

clean :
	rm -f *.o *~ Makefile.bak $(NAME) 

