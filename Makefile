CC=gcc
CFLAGS=-c -Wall -std=gnu99 -g3
OBJ=bonsai.o process.o misc.o mime.o

all: bonsai

bonsai: $(OBJ)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $<

clean:
	rm -rf *.o bonsai
