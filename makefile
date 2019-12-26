# fake target
.PHONY: all clean
# variables
CC = gcc
CFLAGS = -lpthread -lseccomp -Wall

all: sandbox.c
	$(CC) -o sandbox $^ $(CFLAGS)

clean:
	rm -f sandbox