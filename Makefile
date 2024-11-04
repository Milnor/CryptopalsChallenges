CC=gcc
CFLAGS = -Wall -Wextra -g

all: basics.o tests

basics.o: libs/basics.c libs/basics.h	# implement Base64, XOR, AES ECB...
	$(CC) $(CFLAGS) -c $<

tests: test/tests.c basics.o
	$(CC) -c test/tests.c
	$(CC) -o $@ basics.o tests.o
	#$(CC) $(CFLAGS) $< -I./libs/

# TODO: clang-tidy, linting...

memtest: tests
	valgrind -s ./$<

clean:
	rm *.o tests

.PHONY: all clean
