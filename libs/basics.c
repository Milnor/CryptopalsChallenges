#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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

char * bytes_to_hex(uint8_t * bytes, size_t length)
{
    size_t hex_string_len = length * 2 + 1;
    char * output = malloc(hex_string_len);

    for (size_t i = 0; i < length; i++)
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

uint8_t * hex_to_bytes(const char * hex)
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
        errno = 0;
        bytes[i] = strtol(one_byte, NULL, 16);
        if (errno)
        {
            fprintf(stderr, "[-] strtol() error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    return bytes;
}

int is_english(const unsigned char * ascii, ssize_t length)
{
    // Arbitrary heuristics that seem reasonable to me
    int certainty = 0;
    int nonprintable = 0;
    int vowels_lower = 0;
    int vowels_upper = 0;
    int consonants_lower = 0;
    int consonants_upper = 0;
    int letters = 0;
    int punctuation = 0;
    int digits = 0;
    int spaces = 0;
    int misc = 0;
    
    for (ssize_t i = 0; i < length; i++)
    {
        char current = ascii[i];
        if (!isprint(current))
        {
            nonprintable++;
        }
        else if (isupper(current))
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

    if (nonprintable > (length / 10))
    {
        certainty -= 35;
    }

    if (certainty < 0)
    {
        certainty = 0;
    }

    return certainty;
} 

uint8_t decode_b64_byte(uint8_t in)
{
    /*
     * Upper
     * A    0x41     65     ==>     000000   0
     * Z    0x5A     90     ==>     011001  25
     * Lower
     * a    0x61     97     ==>     011010  26
     * z    0x7A    122     ==>     110011  51
     * Digit
     * 0    0x30     48     ==>     110100  52
     * 9    0x39     57     ==>     111101  61
     * Misc
     * +    0x2B     43     ==>     111110  62
     * /    0x2F     47     ==>     111111  63
     */

    if (isupper(in))
    {
        return in - 65;
    }
    else if (islower(in))
    {
        // compiler can optimize this math
        return in - 97 + 26;
    }
    else if (isdigit(in))
    {
        return in - 48 + 52;
    }
    else if ('+' == in)
    {
        return 62;
    }
    else if ('/' == in)
    {
        return 63;
    }
    else if ('=' == in)
    {
        return 0; // padding
    }
    else
    {
        fprintf(stderr, "[-] decode_b64_byte(%c) failed\n", in);
        exit(EXIT_FAILURE);
    }
}

uint8_t * base64_to_bytes(const char * b64)
{
    // TODO: function or macro to calculate precise length
    // every four characters of base64 will decode to three bytes,
    // less if there is padding.

    int size = 0;
    int len = strlen(b64);
    if (len % 4 != 0)
    {
        printf("[-] Invalid length for base64: %d\n", len);
    }

    size = (len / 4) * 3;
    if ('=' == b64[len - 1])
    {
        size--;
    }
    if ('=' == b64[len - 2])
    {
        size--;
    }

    uint8_t * bytes = malloc(size);

    // Convert each 4-byte chunk of Base64 into
    // 3 bytes of decoded data
    int j = 0;
    for (int i = 0; i < size - 4; i++)
    {
        
        uint8_t one = decode_b64_byte(b64[i]);
        uint8_t two = decode_b64_byte(b64[i + 1]);
        uint8_t three = decode_b64_byte(b64[i + 2]);
        uint8_t four = decode_b64_byte(b64[i + 3]);

        // 6 and 2
        bytes[j] = (one << 2) + (two >> 4);
        if (j + 1 >= size)
        {
            break;
        }
        // 4 and 4
        bytes[j + 1] = (two << 4) + (three >> 4);
        if (j + 2 >= size)
        {   
            break;
        }
        // 2 and 6
        bytes[j + 2] = (three << 4) + (four >> 6);

        j += 3;
    }

    return bytes; 
}

char * hex_to_base64(const char * hex)
{
    char base64_lookup[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstu"
                             "vwxyz0123456789+/";
   
    size_t length = strlen(hex); 
    uint8_t * hex_as_bytes = hex_to_bytes(hex);

    // Overhead is add'l 33-37% the size of original binary data (Wikipedia)
    size_t output_size = length + (length * 4)/10;

    char * base64 = calloc(output_size, sizeof(uint8_t));

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

    // TODO: move this to debug print or verbose print
    printf("\t%.48s (%.48s)\n", hex, hex_as_bytes);
    printf("\t ==> %.48s\n", base64);

    free(hex_as_bytes);

    return base64;
}


char * fixed_xor(const char * input, const char * key)
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


    char * output_as_hex = bytes_to_hex(output, length);

    // TODO: Move to debug print or verbose print
    printf("\t%.48s (%.48s)\n", input, input_as_bytes);
    printf("\t%.48s (%.48s)\n", key, key_as_bytes);
    printf("\t---------XOR---------------\n");
    printf("\t%.48s (%.48s)\n", output_as_hex, output);

    free(input_as_bytes); 
    free(key_as_bytes);   
    free(output);

    return output_as_hex;    
}

uint8_t crack_single_byte_xor(char * ciphertext, char * k, char * plaintext)
{
    ssize_t length = strlen(ciphertext) / 2;
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
        //printf("   Score: %d  key: %c, plaintext:%s\n", current_score, key, temp);
        if (current_score > best_score)
        {
            memcpy(plaintext, temp, length);
            best_score = current_score;
            best_key = key;
            
        }

    }

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
    char current_msg[MAX_BUFFER];
    char secret[MAX_BUFFER];
    char key = 0;
    int i = 0;
    int line_no = 0;
    ssize_t count = -1;
    do
    {
        count = read(fd, buffer+i, 1);
        if (0x0a == buffer[i] || 0 == count)
        {
            buffer[i] = '\0';
            //printf("[!] Analyzing i=%d, line %d: %s\n", i, line_no, buffer);
            uint8_t * bytes = hex_to_bytes(buffer);
            int current_score = crack_single_byte_xor(buffer, &key, current_msg);
            if (current_score > best_score)
            {
                best_score = current_score;
                best_key = key;
                memset(secret, '\0', sizeof(secret));
                memcpy(secret, current_msg, i);
                //printf("line_no: %d, best_score: %d, key: %c\n", line_no, best_score, best_key);
                //printf("ciphertext: %s\n", secret);
            }
            i = -1;
            free(bytes);
            line_no++;
            //if (line_no > 0)
            //{
            //    exit(EXIT_SUCCESS);
            //}
        }
        i++;
    } while (count > 0);

    printf("Key: %c, Hidden message: %s\n", best_key, secret);
    
    int rc = close(fd);
    if (-1 == rc)
    {
        fprintf(stderr, "[-] Error closing %s: %s\n", filepath, strerror(errno));
    }
}

char * repeating_xor(const char * input, const char * key)
{
    size_t length = strlen(input);
    size_t key_length = strlen(key);

    uint8_t * output = malloc(length + 1);

    for (size_t i = 0; i < length; i++) 
    {
        output[i] = input[i] ^ key[i % key_length];
    }

    output[length] = '\0';

    char * output_as_hex = bytes_to_hex(output, length);

    // TODO: Move to debug print or verbose print
    printf("\t%.48s\n", input);
    printf("\t%.48s\n", key);
    printf("\t---------XOR---------------\n");
    printf("\t%.48s (%.48s)\n", output_as_hex, output);

    return output_as_hex;
}

void crack_repeating_xor(char * filepath)
{
    int fd = open(filepath, O_RDONLY);
    if (-1 == fd)
    {
        fprintf(stderr, "[-] Can't open %s: %s\n", filepath, strerror(errno));
        exit(EXIT_FAILURE);
    }

    off_t size = lseek(fd, 0, SEEK_END);
    if (-1 == size)
    {
        fprintf(stderr, "[-] lseek() failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("[!] size = %ld\n", size);
    /* I'm opting for memory inefficiency over a separate read()
       syscall on each individual byte. */
    char * temp = malloc(size + 1);         // May contain newlines
    char * raw_base64 = malloc(size + 1);   

    // rewind
    int ret = lseek(fd, 0, SEEK_SET);
    if (-1 == ret)
    {
        fprintf(stderr, "[-] lseek() failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    ret = read(fd, temp, size);
    if (-1 == ret)
    {
        fprintf(stderr, "[-] read() failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    ssize_t j = 0;
    for (ssize_t i = 0; i < ret; i++)
    {
        // filter out newlines
        if (isprint(temp[i]))
        {
            raw_base64[j++] = temp[i];
        } 
    }
    raw_base64[j] = '\0';
    printf("len of temp = %ld, len of raw_base64 = %ld\n", strlen(temp), strlen(raw_base64));
    free(temp);

    int rc = close(fd);
    if (-1 == rc)
    {
        fprintf(stderr, "[-] Error closing %s: %s\n", filepath, strerror(errno));
    }
    //printf("raw_base64:\n%s\n", raw_base64);


    // TODO: we'll need base64 to bytes, just to get started.
    uint8_t * bytes = base64_to_bytes(raw_base64);  
}

