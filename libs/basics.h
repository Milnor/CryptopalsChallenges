#ifndef BASICS_H
#define BASICS_H

#include <stdint.h>

/**
 * @brief Convert byte array to string of hex digits.
 * @param bytes Array of bytes to convert.
 * @param length Size of array.
 * @return Hex string representation of byte array.
 */
char * bytes_to_hex(uint8_t * bytes, size_t length); 

/**
 * @brief Convert a string of hex digits to a byte array.
 * @param hex String of hex digits to convert.
 * @return Byte array.
 */
uint8_t * hex_to_bytes(const char * hex);

/**
 * @brief Estimate likelihood that a byte array is English when read as ASCII.
 * @param ascii A possible plaintext.
 * @param length Size in bytes, since data is not assumed to be NULL-terminated.
 * @return Score of likelihood this is the valid plaintext, 0-100.
 */
int is_english(const unsigned char * ascii, ssize_t length);

/**
 * @brief Helper function for debug printing.
 * @param data Array of bytes to print.
 * @length Size of byte array.
 */
void print_bytes(uint8_t * data, size_t length);

/**
 * @brief Decode a single Base64 ASCII character to an integer.
 * @param in A Base64 character.
 * @return An integer, 0-63.
 */
uint8_t decode_b64_byte(uint8_t in);

/**
 * @brief Convert base64 to bytes.
 * @param b64 A base64 encoded string.
 * @return A byte array decoded from base64.
 */
uint8_t * base64_to_bytes(const char * b64);

/**
 * @brief 1.1 Convert hex to base64.
 * @param hex A string of hex digits.
 * @return A base64 encoded string.
 */
char * hex_to_base64(const char * hex);

/**
 * @brief 1.2 Fixed XOR, apply a single character key to every byte of input.
 * @param input A hex encoded string.
 * @param key A one-byte value that will be XOR'd against every input byte.
 * @return A hex encoded string with the XOR operation applied to each byte. 
 */
char * fixed_xor(const char * input, const char * key);

/**
 * @brief 1.3 Single-byte XOR cipher, find the one byte key that produces the best plaintext.
 * @param ciphertext Hex string encoded with single-byte XOR.
 * @param k Storage for single char key estimated to produce the best plaintext.
 * @param plaintext Storage for string estimated to be best decoded plaintext.
 * @return Score (certainty plaintext is valid) in the range 0-100.
 */
uint8_t crack_single_byte_xor(char * ciphertext, char * k, char * plaintext);

/**
 * @brief 1.4 Detect single-character XOR.
 * @param filepath File containing strings (separated by newline), one of which is XOR encoded with a one-byte key.
 */
void detect_single_byte_xor(char * filepath);

/**
 * @brief 1.5 Implement repeating-key XOR.
 * @param input Plaintext to encrypt.
 * @param key Encryption key of arbitrary length.
 * @return The input incrypted with the key.
 */
char * repeating_xor(const char * input, const char * key);

/**
 * @brief 1.6 Break repeating-key XOR.
 * @param File containing vigenere-encrypted text, encoded in base64.
 */
void crack_repeating_xor(char * filepath);

#endif

