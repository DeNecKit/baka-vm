CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -Wswitch-enum
SRCS=src/*.c

.PHONY: all

all:
	$(CC) $(SRCS) $(CFLAGS) -o bakavm
	./bakavm
