CC=gcc
CFLAGS = -Wall -Wextra -g

all: basics.o tests

basics.o: libs/basics.c libs/basics.h	# implement Base64, XOR, AES ECB...
	$(CC) $(CFLAGS) -c $<

tests: test/tests.c basics.o
	$(CC) -c test/tests.c
	$(CC) -o $@ basics.o tests.o
	#$(CC) $(CFLAGS) $< -I./libs/

# TODO: memtest with valgrind, clang-tidy, linting...

clean:
	rm *.o

.PHONY: all clean
