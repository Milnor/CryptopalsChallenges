#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "basics.h"

void print_bytes(uint8_t * data, size_t length)
{
    printf("Data of length %ld:\n", length);
    for (size_t i = 0; i < length; i++)
    {
        printf("%x", data[i]);
    }
    printf("\n");
}

uint8_t * hex_to_bytes(char * hex)
{
    if (strlen(hex) % 2 != 0)
    {
        fprintf(stderr, "[-] Odd number of characters in hex string\n");
        exit(EXIT_FAILURE);
    }

    int length = strlen(hex) / 2;
    uint8_t * bytes = malloc(length);

    int prev = -1;
    for (int i = 0; i < length; i++)
    {
        if (prev == i)                 // This check is never triggered,                 
        {                              // but when I remove it, i
            printf("i got stuck!\n");  // gets stuck at 1 and there is
            exit(1);                   // an infinite loop.
        }
        //printf("i = %d, length = %d, hex[] = %02X\n", i, length, hex+(i*2));
        uint8_t temp = 0;
        if (1 > sscanf(hex+(i*2), "%2X", (unsigned int *)&temp))
        {
            fprintf(stderr, "[-] sscanf() failed to match\n");
            exit(1);
        }
        bytes[i] = temp;
        prev = i;
    }

    printf("Length of string: %ld\n", strlen(hex));
    printf("Length of array:  %d\n", length);
    printf("Hex string: %s\n", hex);
    printf("Byte array: ");
    for (int i = 0; i < length; i++)
    {
        printf("%x", bytes[i]);
    }
    printf("\n");

    return bytes;
}

uint8_t * hex_to_base64(uint8_t * hex, size_t length)
{
    char base64_lookup[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    //char padding = '=';
    // Overhead is add'l 33-37% the size of original binary data (Wikipedia)
    size_t output_size = length + (length * 4)/10;

    uint8_t * base64 = calloc(output_size, sizeof(uint8_t));

    uint8_t value = 0; // numeric value of hextet
    uint8_t bits_used = 0;
    uint8_t leftover_bits = 0;
    uint8_t output_index = 0;
    for (size_t i = 0; i < length; i++)
    {
        uint8_t current_byte = hex[i];
        if (0 == bits_used)
        {
            // Use the high order six bits
            value = current_byte >> 2;
            base64[output_index++] = base64_lookup[value];
            bits_used = 6;
            leftover_bits = (0b00000011 & current_byte) << 4;
        }
        else if (6 == bits_used)
        {
            // Combine leftovers with new current_byte
            value = leftover_bits + (current_byte >> 4);
            base64[output_index++] = base64_lookup[value];
            bits_used = 4;
            leftover_bits = (0b00001111 & current_byte) << 2;
        }
        else if (4 == bits_used)
        {
            // Combine leftovers with just two new bits from current_byte
            value = leftover_bits + (current_byte >> 6);
            base64[output_index++] = base64_lookup[value];
            
            // Now grab the lower order six bits of current_byte
            value = 0b00111111 & current_byte;
            base64[output_index++] = base64_lookup[value]; 
            bits_used = 0;
            leftover_bits = 0;
        }
        else
        {
            // This should never happen
            fprintf(stderr, "[-] Bit scanning out of alignment");
            exit(EXIT_FAILURE);
        }

    }

    return base64;
}


uint8_t * fixed_xor(char * input, char * key)
{
    if (strlen(input) != strlen(key))
    {
        fprintf(stderr, "[-] Input buffer length and XOR key length mismatch");
        exit(EXIT_FAILURE);
    }

    size_t length = strlen(input) / 2;

    uint8_t * output = malloc(length);
    uint8_t * input_as_bytes = hex_to_bytes(input);
    uint8_t * key_as_bytes = hex_to_bytes(key);

    for (size_t i = 0; i < length; i++) 
    {
        output[i] = input_as_bytes[i] ^ key_as_bytes[i];
    }

    free(input_as_bytes);
    free(key_as_bytes);

    return output;    
}

