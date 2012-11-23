CC=gcc
CFLAGS=-c -Wall -std=gnu99
OBJ=bonsai.o process.o misc.o mime.o

all: bonsai

bonsai: $(OBJ)
	$(CC) $(OBJ) -o bonsai

bonsai.o: bonsai.c
	$(CC) $(CFLAGS) bonsai.c

process.o: process.c
	$(CC) $(CFLAGS) process.c

misc.o: misc.c
	$(CC) $(CFLAGS) misc.c

mime.o: mime.c
	$(CC) $(CFLAGS) mime.c

clean:
	rm -rf *.o bonsai
