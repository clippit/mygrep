CC=gcc

mygrep:
	$(CC) -o mygrep mygrep.c

clean:
	rm -f mygrep
