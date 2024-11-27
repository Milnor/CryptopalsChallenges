#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "testing.h"

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

