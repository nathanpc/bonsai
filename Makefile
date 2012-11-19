CC=gcc
CFLAGS=-c -Wall -std=c99

all: bonsai

bonsai: bonsai.o process.o misc.o
	$(CC) bonsai.o process.o misc.o -o bonsai

bonsai.o: bonsai.c
	$(CC) $(CFLAGS) bonsai.c

process.o: process.c
	$(CC) $(CFLAGS) process.c

misc.o: misc.c
	$(CC) $(CFLAGS) misc.c

clean:
	rm -rf *.o bonsai