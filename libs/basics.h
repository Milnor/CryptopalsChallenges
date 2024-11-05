#ifndef BASICS_H
#define BASICS_H

#include <stdint.h>

/*
 * Convert byte array to string of hex digits.
 */
uint8_t * bytes_to_hex(uint8_t * bytes, size_t length); 

/*
 * Convert a string of hex digits to a byte array.
 */
uint8_t * hex_to_bytes(char * hex);


/*
 * Helper function for debug printing that is currently unused.
 */
void print_bytes(uint8_t * data, size_t length);

/*
 * 1.1 Convert hex to base64.
 */
uint8_t * hex_to_base64(uint8_t * hex);

/*
 * 1.2 Fixed XOR.
 */
uint8_t * fixed_xor(char * input, char * key);

#endif

