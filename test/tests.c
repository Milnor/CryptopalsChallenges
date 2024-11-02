#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/basics.h"

int main()
{
    printf("Tests for Set 1: Basics\n");
    printf("[!] Testing 1.1 Hex to Base64...\n");

    // No padding example from https://cryptopals.com/sets/1/challenges/1
    
    int successes = 0;

    uint8_t pad0_input[] = "\x49\x27\x6d\x20\x6b\x69\x6c\x6c\x69\x6e\x67\x20"
                           "\x79\x6f\x75\x72\x20\x62\x72\x61\x69\x6e\x20\x6c"
                           "\x69\x6b\x65\x20\x61\x20\x70\x6f\x69\x73\x6f\x6e"
                           "\x6f\x75\x73\x20\x6d\x75\x73\x68\x72\x6f\x6f\x6d";
    uint8_t pad0_output[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29u"
                            "b3VzIG11c2hyb29t";

    char * output = hex_to_base64(pad0_input, 48);
    
    if (strncmp(output, pad0_output, strlen(pad0_output)))
    {
        // Failure
        fprintf(stderr, "[-] No padding test failed:\n"
                        "Expected: %s\n"
                        "Actual: %s\n",
                        pad0_output, output);
    }
    else
    {
        successes++;
        printf("[+] No padding test passed\n");
    }

    free(output);

    // Examples with padding from https://en.wikipedia.org/wiki/Base64
    uint8_t pad1_input[] = "light work."; 
    uint8_t pad1_output[] = "bGlnaHQgd29yay4=";
    output = hex_to_base64(pad1_input, 11);
    
    if (strncmp(output, pad1_output, strlen(pad1_output)))
    {
        fprintf(stderr, "[-] Pad 1 test failed:\n"
                        "Expected: %s\n"
                        "Actual: %s\n",
                        pad1_output, output);
    }
    else
    {
        successes++;
        printf("[+] Pad 1 test passed\n");
    }

    free(output);

    uint8_t pad2_input[] = "light work"; 
    uint8_t pad2_output[] = "bGlnaHQgd29yaw==";
    output = hex_to_base64(pad2_input, 10);

    if (strncmp(output, pad2_output, strlen(pad2_output)))
    {
        fprintf(stderr, "[-] Pad 2 test failed:\n"
                        "Expected: %s\n"
                        "Actual: %s\n",
                        pad2_output, output);
    }
    else
    {
        successes++;
        printf("[+] Pad 2 test passed\n");
    }

    free(output);

    printf("[!] %d of 3 tests passed for Hex to Base64\n", successes);
}

