CC      = cc
CFLAGS  = -Iinclude -Wall -Wextra -O2 -std=c11
SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:.c=.o)

all: mixnet

mixnet: main.c $(OBJ)
	$(CC) $(CFLAGS) main.c $(OBJ) -o mixnet

test_poly: tests/test_poly.c $(OBJ)
	$(CC) $(CFLAGS) tests/test_poly.c $(OBJ) -o test_poly

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) mixnet test_poly

.PHONY: all clean
