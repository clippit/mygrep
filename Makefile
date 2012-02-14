CC = gcc
CFLAGS = -Wall -Ilib
ifneq ($(DEBUG),)
	CFLAGS += -g -DDEBUG
else
	CFLAGS += -O2
endif 

OBJS=isdir.o mygrep.o

.PHONY : all clean $(OBJS)

all: $(OBJS)
	$(CC) -o mygrep $(OBJS)

mygrep.o: mygrep.c
	$(CC) -c ${CFLAGS} mygrep.c

isdir.o: lib/isdir.h lib/isdir.c
	$(CC) -c ${CFLAGS} lib/isdir.c

clean:
	rm -f $(OBJS) mygrep
