CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lncurses

# Default target
all: game

# Build the game
game: main.c
	$(CC) $(CFLAGS) -o game main.c $(LIBS)

# Clean build artifacts
clean:
	rm -f game

# Run the game
run: game
	./game

.PHONY: all clean run
