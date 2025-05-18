CC=gcc
CFLAGS=-I./include
SRC=./src/*.c
MAIN=main.c
OUT=dodgecode

all:
	$(CC) $(MAIN) $(SRC) $(CFLAGS) -o $(OUT)