CC      = cc
CFLAGS  = -Iinclude -Wall -Wextra -O2 -std=c11
SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:.c=.o)

# Each test links only the modules it needs.
TEST_BINS = test_poly test_mlwe test_otse test_hpke

all: mixnet

mixnet: main.c $(OBJ)
	$(CC) $(CFLAGS) main.c $(OBJ) -o mixnet

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test_poly: tests/test_poly.c src/poly.o
	$(CC) $(CFLAGS) $^ -o $@

test_mlwe: tests/test_mlwe.c src/mlwe.o src/poly.o src/sample.o
	$(CC) $(CFLAGS) $^ -o $@

test_otse: tests/test_otse.c src/otse.o src/poly.o src/sample.o
	$(CC) $(CFLAGS) $^ -o $@

test_hpke: tests/test_hpke.c src/hpke.o src/mlwe.o src/otse.o src/poly.o src/sample.o
	$(CC) $(CFLAGS) $^ -o $@

test: $(TEST_BINS)
	@for t in $(TEST_BINS); do echo "== $$t =="; ./$$t || exit 1; done

clean:
	rm -f $(OBJ) mixnet $(TEST_BINS)

.PHONY: all clean test
