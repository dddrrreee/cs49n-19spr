CFLAGS = -Wall -Og  -I../pi-cat -Werror  -Wno-unused-function 
CC = gcc
SRC = esp-uart.c ../pi-cat/tty.c 
OBJS = $(SRC:.c=.o)
NAME = server

# ok.  this is filtering out.
# OBJS := $(filter-out test.o, $(OBJS))


all:  $(NAME) run

# $(NAME) run client

run:
	./$(NAME) /dev/ttyUSB0

$(NAME):  $(OBJS)  $(NAME).o
	$(CC) $(CFLAGS) $(OBJS) $(NAME).o -o $(NAME) -lm
	# ./$(NAME) /dev/ttyUSB0
    

client:  $(OBJS)  client.o
	$(CC) $(CFLAGS) $(OBJS) client.o -o client -lm
	./client /dev/ttyUSB1

.PHONE: run  clean depend

clean :
	rm -f *.o *~ Makefile.bak $(NAME) $(OBJS) client

depend:
	makedepend *.[ch] 
