CC=gcc

.PHONY : all mygrep.o clean

all: mygrep.o
	$(CC) -o mygrep mygrep.o

mygrep.o:
	$(CC) -c mygrep.c

clean:
	rm -f mygrep.o mygrep
