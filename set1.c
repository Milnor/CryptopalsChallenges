#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void print_bytes(uint8_t * data, size_t length)
{
    printf("Data of length %ld:\n", length);
    for (size_t i = 0; i < length; i++)
    {
        printf("%x", data[i]);
    }
    printf("\n");
}

/* 1.1 Convert hex to base64 */
uint8_t * hex_to_base64(uint8_t * hex, size_t length)
{
    // Base64 Mapping
    // 0-25    A-Z
    // 26-51   a-z
    // 52-61   0-9
    // 62-63   +,/
    // Padding =
    char base64_lookup[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char padding = '=';
    // Overhead is add'l 33-37% the size of original binary data (Wikipedia)
    size_t output_size = length + (length * 4)/10;
    print_bytes(hex, length);

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
            // printf("current_byte=%x, value=%B\n", current_byte, value); 
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
        //else if (2 == bits_used)
        //{
            // Combine 4 leftover bits with two from current_byte
            //value = leftover_bits + (current_byte >> 4);
            //base64[output_index++] = base64_lookup[value];
            //bits_used = 2;
            //leftover_bits = (0b00001111 & current_byte) << 2;
        //}
        else
        {
            fprintf(stderr, "[-] Bit scanning out of alignment");
            exit(EXIT_FAILURE);
        }

    }

    return base64;

}

/* 1.2 Fixed XOR */

int main()
{
    uint8_t hex_string[] = "\x49\x27\x6d\x20\x6b\x69\x6c\x6c\x69\x6e\x67\x20\x79\x6f\x75\x72\x20\x62\x72\x61\x69\x6e\x20\x6c\x69\x6b\x65\x20\x61\x20\x70\x6f\x69\x73\x6f\x6e\x6f\x75\x73\x20\x6d\x75\x73\x68\x72\x6f\x6f\x6d"; // 48 bytes

    char base64_string[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

    printf("Converted to Base64: %s\n", hex_to_base64(hex_string, 48));
    printf("         Answer key: %s\n", base64_string);

}

