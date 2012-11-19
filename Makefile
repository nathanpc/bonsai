CC=gcc
CFLAGS=-c -Wall -std=c99

all: bonsai

bonsai: bonsai.o process.o
	$(CC) bonsai.o process.o -o bonsai

bonsai.o: bonsai.c
	$(CC) $(CFLAGS) bonsai.c

process.o: process.c
	$(CC) $(CFLAGS) process.c

clean:
	rm -rf *.o bonsai