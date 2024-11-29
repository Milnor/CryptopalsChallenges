#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "testing.h"

#define SUCCESS     printf("\e[0;32m [+]\e[0;37m %s test passed.\n", name); \
                    return 1

void test_func_1param(const char * name, const char * expected, const char * input1, char * (*func)(const char *))
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

void test_func_2params(const char * name, const char * expected, const char * input1, const char * input2, char * (*func)(const char *, const char *))
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

/* This alternate set of functions can't automatically free memory allocated
 * by the functions being tested, but is more flexible. Just testing outputs
 * obviates the need for one test function for every possible parameter list.
 *
 * Also, returning 1 on success allows us to total the count of tests that pass.
 */

int assert_equal_integers(const char * name, int expected, int actual)
{
    if (expected == actual)
    {
        SUCCESS;
    }
    else
    {
        fprintf(stderr, "\e[0;31m [-]\e[0;37m %s test failed:\n"
                        "  Expected: %d\n"
                        "  Actual: %d\n",
                        name, expected, actual);
    }
}

int assert_equal_strings(const char * name, const char * expected, const char * actual)
{
    return 0;
}

int assert_equal_bytes(const char * name, uint8_t * expected, uint8_t * actual, size_t length)
{
    return 0;
} 
