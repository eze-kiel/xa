CC=gcc
CFLAGS=-Wall -Wextra -O3
BIN=xa
DIR="${HOME}/.bins/"

build: main.c
	$(CC) $(CFLAGS) -o $(BIN) main.c

clean: xa
	rm -f $(BIN)

install: build
	install $(BIN) $(DIR)
