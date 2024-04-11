# basic makefile for later updating
# don't forget to make the build folder :)

all: test

test: main.c
	gcc -Wall -std=c99 -Wextra main.c hlutur.c places.c ymislegt.c -o build/main