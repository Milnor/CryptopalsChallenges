CC=gcc
CFLAGS = -Wall -Wextra -g

all: basics.o testing.o solver unittests

basics.o: libs/basics.c libs/basics.h	# implement Base64, XOR, AES ECB...
	$(CC) $(CFLAGS) -c $<

testing.o: libs/testing.c libs/testing.h
	$(CC) -o $@ -c $<

solver: solver.c basics.o testing.o
	$(CC) -c solver.c
	$(CC) -o $@ solver.o basics.o testing.o

unittests: test/unittests.c basics.o testing.o
	$(CC) -o $@.o -c test/unittests.c
	$(CC) -o $@ basics.o unittests.o testing.o
	#$(CC) $(CFLAGS) $< -I./libs/

# TODO: clang-tidy, clang-analyzer, ASAN...

memtest: solver unittests
	valgrind -s ./$^

clean:
	rm *.o unittests solver

.PHONY: all clean memtest
