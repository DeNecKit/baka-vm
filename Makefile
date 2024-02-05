CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -Wswitch-enum
SRCS=src/*.c

.PHONY: all build run

all: run

build: $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) -o bakavm

run: build
	./bakavm
