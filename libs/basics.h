#ifndef BASICS_H
#define BASICS_H

#include <stdint.h>

/*
 * Helper function for debug printing that is currently unused.
 */
void print_bytes(uint8_t * data, size_t length);

/*
 * 1.1 Convert hex to base64.
 */
uint8_t * hex_to_base64(uint8_t * hex, size_t length);

/*
 * 1.2 Fixed XOR.
 */
uint8_t * fixed_xor(char * input, char * key);

#endif

