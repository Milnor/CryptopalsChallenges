#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/basics.h"

// Test function that takes 1 string as input
void test_func_1param(const char * name, const char * expected, const char * input1, char * (*func)(char *))
{
    char * output = func(input1);
    
    if (strncmp(output, expected, strlen(expected)))
    {
        fprintf(stderr, " [-] %s test failed:\n"
                        "  Expected: %s\n"
                        "  Actual: %s\n",
                        name, expected, output);
    }
    else
    {
        printf(" [+] %s test passed\n", name);
    }

    free(output);
}

// Test function that takes 2 strings as inputs
void test_func_2params(const char * name, const char * expected, const char * input1, const char * input2, char * (*func)(char *, char *))
{
    char * output = func(input1, input2);
    
    if (strncmp(output, expected, strlen(expected)))
    {
        fprintf(stderr, " [-] %s test failed:\n"
                        "  Expected: %s\n"
                        "  Actual: %s\n",
                        name, expected, output);
    }
    else
    {
        printf(" [+] %s test passed\n", name);
    }

    free(output);
}

int main()
{
    printf("New test function...\n");

    /* 1.1 Convert hex to base64 */

    // No padding example from https://cryptopals.com/sets/1/challenges/1
    test_func_1param("Hex string to base64 - no pad",
                     "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29u"  
                     "b3VzIG11c2hyb29t",
                     "49276d206b696c6c696e6720796f757220627261696e206c6"  
                     "96b65206120706f69736f6e6f7573206d757368726f6f6d",
                     hex_to_base64);

    
    // Examples with padding from https://en.wikipedia.org/wiki/Base64
    test_func_1param("Hex string to base64 - 1 pad",
                     "bGlnaHQgd29yay4=",
                     "6c6967687420776f726b2e",      // "light work."
                     hex_to_base64);
    

    test_func_1param("Hex string to base64 - 2 pad",
                     "bGlnaHQgd29yaw==",
                     "6c6967687420776f726b",        // "light work" 
                     hex_to_base64);


    /* 1.2 Fixed XOR */
    
    test_func_2params("Fixed XOR",
                      "746865206b696420646f6e277420706c6179",
                      "1c0111001f010100061a024b53535009181c",
                      "686974207468652062756c6c277320657965",
                      fixed_xor);

    /* 1.3 Single-byte XOR cipher */
    char * ciphertext = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    
    // TODO: now that it works, decide how best to test/demonstrate it...
    crack_single_byte_xor(ciphertext);
}

