CC=gcc
CFLAGS=-g -DDEBUG -Wall -Ilib
OBJS=isdir.o mygrep.o

.PHONY : all clean $(OBJS)

all: $(OBJS)
	$(CC) -o mygrep $(OBJS)

mygrep.o: mygrep.c
	$(CC) -c $(CFLAGS) mygrep.c

isdir.o: lib/isdir.h lib/isdir.c
	$(CC) -c $(CFLAGS) lib/isdir.c

clean:
	rm -f $(OBJS) mygrep
