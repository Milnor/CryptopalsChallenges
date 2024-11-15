# CryptopalsChallenges
complete challenges from cryptopals.com in C

## TODO
* [x] fix bug in last sextet of base64 encode
* [x] refactor out duplicate code in tests.c
* [x] remove commented-out code
* [x] docstrings in basics.h
* [x] convert output of fixed XOR -- test *should* pass
* [ ] add code coverage to Makefile
* [ ] add linting to Makefile
* [ ] diagnose problem with Doxygen output
* [x] Set 1 Challenge 3 looks fun
* [x] lots of compiler warnings and valgrind errors to fix
* [ ] add debug print macro or --verbose option
    - show off the flags/Easter eggs encoded in the sample data
* [ ] refactor: add ERR\_PRINT macro
* [x] Set 1 Challenge 4
* [x] Set 1 Challenge 5
* [ ] Set 1 Challenge 6
* [ ] Set 1 Challenge 7
* [ ] Set 1 Challenge 8

## Dependencies

Perhaps I should Dockerize the build environment, but for now I'll just
list what tools I've installed (besides gcc):
* `clang-tidy`
* `doxygen`
* `graphviz` for function graphs in Doxygen
* `make`
* `valgrind`
