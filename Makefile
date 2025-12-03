CC=gcc
CFLAGS=-Wall -Wextra -O3
BIN=xa
DIR="${HOME}/.bins/"

SRC=$(wildcard *.c)
HDR=$(wildcard *.h)

build: $(SRC) $(HDR)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

clean:
	rm -f $(BIN)

install: build
	install $(BIN) $(DIR)
