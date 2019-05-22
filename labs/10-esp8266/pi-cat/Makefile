CFLAGS = -Wall -Og 
CC = gcc
SRC = pi-cat.c test.c tty.c
OBJS = $(SRC:.c=.o)

# ok.  this is filtering out.
OBJS := $(filter-out test.o, $(OBJS))

NAME = pi-cat 

all : $(NAME) run

run:
	./pi-cat /dev/ttyUSB1

$(NAME):  $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lm

clean :
	rm -f *.o *~ Makefile.bak $(NAME)

depend:
	makedepend *.[ch] 
