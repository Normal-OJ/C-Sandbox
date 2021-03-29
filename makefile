# fake target
.PHONY: all clean
# variables
CC = gcc
CFLAGS = -lpthread -lseccomp -Wall

all: sandbox.c
	$(CC) -o sandbox $^ $(CFLAGS)

test: sandbox
	./sandbox 0 1 /dev/null /dev/null /dev/null 1000  1024000 1 1024000 10 result

clean:
	rm -f sandbox
	rm -f stdin stdout stderr result main
