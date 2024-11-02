#ifndef BASICS_H
#define BASICS_H

#include <stdint.h>

void print_bytes(uint8_t * data, size_t length);
uint8_t * hex_to_base64(uint8_t * hex, size_t length);

#endif

