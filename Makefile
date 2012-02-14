CC=gcc
CFLAGS=-g -DDEBUG -Wall

.PHONY : all mygrep.o clean

all: mygrep.o
	$(CC) -o mygrep mygrep.o

mygrep.o: mygrep.c
	$(CC) -c $(CFLAGS) mygrep.c

clean:
	rm -f mygrep.o mygrep
