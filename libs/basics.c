#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "basics.h"

#define MAX_BUFFER  256

void print_bytes(uint8_t * data, size_t length)
{
    printf("Data of length %ld:\n", length);
    for (size_t i = 0; i < length; i++)
    {
        printf("%x", data[i]);
    }
    printf("\n");
}

uint8_t * bytes_to_hex(uint8_t * bytes, size_t length)
{
    size_t hex_string_len = length * 2 + 1;
    uint8_t * output = malloc(hex_string_len);

    for (size_t i = 0; i < hex_string_len; i++)
    {
        int ret =  sprintf(output+(i*2), "%02x", bytes[i]);
        if (2 != ret)
        {
            fprintf(stderr, "[-] sprintf() failed, ret=%d\n", ret);
        }
    }

    output[hex_string_len - 1] = 0;

    return output;
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
    char one_byte[] = {0, 0, 0};

    for (int i = 0; i < length; i++)
    {
        one_byte[0] = hex[(i*2)];
        one_byte[1] = hex[(i*2)+1];
        bytes[i] = strtol(one_byte, NULL, 16);
        // check strtol for error...
    }

    return bytes;
}

int is_english(const char * ascii, size_t length)
{
    // Arbitrary heuristics that seem reasonable to me
    int certainty = 0;
    int vowels_lower = 0;
    int vowels_upper = 0;
    int consonants_lower = 0;
    int consonants_upper = 0;
    int letters = 0;
    int punctuation = 0;
    int digits = 0;
    int spaces = 0;
    int misc = 0;
    
    for (size_t i = 0; i < length; i++)
    {
        char current = ascii[i];
        if (isupper(current))
        {
            switch (current)
            {
                case 'A':
                case 'E':
                case 'I':
                case 'O':
                case 'U':
                    vowels_upper++;
                    break;
                default:
                    consonants_upper++;
            }
        }
        else if (islower(current))
        {
            switch (current)
            {
                case 'a':
                case 'e':
                case 'i':
                case 'o':
                case 'u':
                    vowels_lower++;
                    break;
                default:
                    consonants_lower++;
            }
        }
        else if (isdigit(current))
        {
            digits++;
        }
        else if (ispunct(current))
        {
            punctuation++;
        }
        else if (isspace(current))
        {
            spaces++;
        }
        else
        {
            misc++;
        }
    }

    letters = vowels_lower + vowels_upper + consonants_lower + consonants_upper;

    // Non-standard capitalization lowers the likelihood but isn't
    // weighted highly
    if (consonants_upper < consonants_lower)
    {
        certainty += 5;
    }
    if (vowels_upper < vowels_lower)
    {
        certainty += 5;
    }

    // Non-standard spacing also lowers likelihood, but is possible
    // (A more sophisticated heuristic would check distance between spaces)
    if ((spaces * 5) < length)
    {
        certainty += 20;
    }
    if (spaces > (length / 12))
    {
        certainty += 15;
    }

    // Should not expect excessive punctuation
    if ((punctuation * 3) < letters)
    {
        certainty += 8;
    }

    // Or too little punctuation
    if (punctuation > (length / 30))
    {
        certainty += 8;
    }    

    // Not too many digits
    if (digits < (length / 5))
    {
        certainty += 5;
    }


    // Not too many miscellaneous
    if (misc < (length / 20))
    {
        certainty += 5;
    }

    return certainty;
} 

uint8_t * hex_to_base64(uint8_t * hex)
{
    char base64_lookup[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstu"
                             "vwxyz0123456789+/";
   
    size_t length = strlen(hex); 
    uint8_t * hex_as_bytes = malloc(length/2);
    hex_as_bytes = hex_to_bytes(hex);

    // Overhead is add'l 33-37% the size of original binary data (Wikipedia)
    size_t output_size = length + (length * 4)/10;

    uint8_t * base64 = calloc(output_size, sizeof(uint8_t));

    uint8_t value = 0; // numeric value of hextet
    uint8_t bits_used = 0;
    uint8_t leftover_bits = 0;
    uint8_t output_index = 0;
    for (size_t i = 0; i < length / 2; i++)
    {
        uint8_t current_byte = hex_as_bytes[i]; // was hex
        if (0 == bits_used)
        {
            // Use the high order six bits, store 2 leftover bits
            value = current_byte >> 2;
            base64[output_index++] = base64_lookup[value];
            bits_used = 6;
            leftover_bits = (0b00000011 & current_byte) << 4;

            if ((length / 2) - 1 == i)
            {
                // end of input, remaining bits all 0
                base64[output_index++] = base64_lookup[leftover_bits];
                bits_used = 0;
                leftover_bits = 0;
            }

        }
        else if (6 == bits_used)
        {
            // Combine 2 leftover bits with new current_byte, store 4 leftovers
            value = leftover_bits + (current_byte >> 4);
            base64[output_index++] = base64_lookup[value];
            bits_used = 4;
            leftover_bits = (0b00001111 & current_byte) << 2;

            if ((length / 2) - 1 == i)
            {
                // end of input, remaining bits all 0
                base64[output_index++] = base64_lookup[leftover_bits];
                bits_used = 0;
                leftover_bits = 0;
            } 

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

    // padding (=) is needed if unencoded input was not a multiple of 3
    int input_len = length / 2;
    while (input_len % 3 != 0)
    {
        base64[output_index++] = '=';
        input_len++;
    }

    free(hex_as_bytes);

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


    uint8_t * output_as_hex = bytes_to_hex(output, length);
    //free(input_as_bytes);
    //free(key_as_bytes);
    //free(output);

    return output_as_hex;    
}

uint8_t crack_single_byte_xor(char * ciphertext, char * k)
{
    size_t length = strlen(ciphertext) / 2;
    uint8_t * bytes = hex_to_bytes(ciphertext);
    uint8_t * temp = malloc(length);    

    int best_score = -1;
    int best_key = -1;

    // Assume for now key is in printable range 0x20 ' ' to 0x7E '~'
    for (uint8_t key = ' '; key <= '~'; key++)
    {
        for (int i = 0; i < length; i++)
        {
            temp[i] = bytes[i] ^ key;
        }
        int current_score = is_english(temp, length);
        if (current_score > best_score)
        {
            best_score = current_score;
            best_key = key;
        }

    }

    //printf("Best key seen was %c\n", best_key);

    free(temp);
    free(bytes);

    *k = best_key;

    return best_score;
}

void detect_single_byte_xor(char * filepath)
{
    int fd = open(filepath, O_RDONLY);
    if (-1 == fd)
    {
        fprintf(stderr, "[-] Can't open %s: %s\n", filepath, strerror(errno));
        exit(EXIT_FAILURE);
    }

    int best_score = -1;
    int best_key = -1;
    char buffer[MAX_BUFFER];
    char key = 0;
    int i = 0;
    int line_no = 0;
    ssize_t count = -1;
    do
    {
        count = read(fd, buffer+i, 1);
        if (buffer[i] == '\n' || 0 == count)
        {
            buffer[i] = '\0';
            uint8_t * bytes = hex_to_bytes(buffer);
            int current_score = crack_single_byte_xor(buffer, &key);
            if (current_score > best_score)
            {
                best_score = current_score;
                printf("line_no: %d, best_score: %d, key: %c\n", line_no, best_score, key);
                printf("ciphertext: %s\n", buffer);
                printf("plaintext: %s\n", fixed_xor(buffer, key));
            }
            i = -1;
            free(bytes);
        }
        i++;
        line_no++;
    } while (count != 0);
    
    int rc = close(fd);
    if (-1 == rc)
    {
        fprintf(stderr, "[-] Error closing %s: %s\n", filepath, strerror(errno));
    }
}
