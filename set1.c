#include <stdio.h>
#include <stdint.h>

void print_bytes(uint8_t * data, size_t length)
{
    printf("Data of length %ld:\n", length);
    for (size_t i; i < length; i++)
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

    // Overhead is add'l 33-37% the size of original binary data (Wikipedia)

    print_bytes(hex, length);

    return NULL;

}

/* 1.2 Fixed XOR */

int main()
{
    uint8_t hex_string[] = "\x49\x27\x6d\x20\x6b\x69\x6c\x6c\x69\x6e\x67\x20\x79\x6f\x75\x72\x20\x62\x72\x61\x69\x6e\x20\x6c\x69\x6b\x65\x20\x61\x20\x70\x6f\x69\x73\x6f\x6e\x6f\x75\x73\x20\x6d\x75\x73\x68\x72\x6f\x6f\x6d"; // 48 bytes

    char base64_string[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

    hex_to_base64(hex_string, 48);

}

