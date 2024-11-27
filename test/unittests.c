#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/basics.h"
#include "../libs/testing.h"

/**
 * @brief Test correctness of individual functions.
 */
int main()
{


    /* 1.1 Convert hex to base64 */

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
    char secret[256];
    char key = '\0';
    uint8_t score = crack_single_byte_xor(ciphertext, &key, secret);
    printf("[+] Key: %c, Score: %d, Message: %s\n", key, score, secret);

    /* 1.4 Detect single-character XOR */
    detect_single_byte_xor("data/detect_single_xor.txt");

    /* 1.5 Implement repeating-key XOR */
    
    test_func_2params("Repeating-key XOR",
                      "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d6334"
                      "3c2a26226324272765272a282b2f20430a652e2c652a3124333a65"
                      "3e2b2027630c692b20283165286326302e27282f",
                      "Burning 'em, if you ain't quick and nimble\n"
                      "I go crazy when I hear a cymbal",
                      "ICE",
                      repeating_xor);

    /* 1.6 Break repeating-key XOR */

    crack_repeating_xor("data/crack_vigenere.txt");

    // TODO: move to separate file for unit tests
    // TODO: separate demo of solving challenges from unit testing
/*
    test_func_1param("Hex to base64",
                     "Man",
                     "TWFu",
                     base64_to_bytes);
*/

    base64_to_bytes("TWFu");

// had test of hex_to_base64 here
/*
    test_func_1param("Base64 to hex",
                     "31323334",
                     "MTIzNA==",
                     base64_to_bytes);

    test_func_1param("Base64 to hex",
                     "31323334",
                     "MTIzNA==",
                     base64_to_bytes);
*/

    int ham = hamming_calc("this is a test", "wokka wokka!!!");
    printf("[!] hamming=%d; expected 37.\n", ham);
}

