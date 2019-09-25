# make					# compile all binary
# make clean			# remove ALL binaries and objects

CFLAGS = -g -O -Wall
LIBS = -lm

CC = gcc
NAME = start
SRC = $(wildcard *.c)
DEPS = $(wildcard *.h)

all: main

clean:
	rm -f *.o NAME
.c.o:
	$(CC) -c $(INCLUDES) $(CFLAGS) $<

main: $(SRC)
	$(CC) $(SRC) $(DEPS) $(LIBS) -o $(NAME)