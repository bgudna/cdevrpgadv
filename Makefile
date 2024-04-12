# basic makefile for later updating
# don't forget to make the build folder :)

all: test

test: main.c
	gcc main.c hlutur.c places.c ymislegt.c -o build/main -lncurses