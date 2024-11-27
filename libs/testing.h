#ifndef TESTS_H
#define TESTS_H


/** 
 * @brief Test function that takes 1 string as input.
 * @param name Description of current test.
 * @param expected Correct output for test case.
 * @param input1 Input passed to function under test.
 * @param func Function being tested.
 */
void test_func_1param(const char * name, const char * expected, const char * input1, char * (*func)(const char *));

/**
 * @brief Test function that takes 2 strings as inputs.
 * @param name Description of current test.
 * @param expected Correct output for test case.
 * @param input1 First input passed to function under test.
 * @param input2 Second input passed to function under test.
 * @param func Function being tested.
 */
void test_func_2params(const char * name, const char * expected, const char * input1, const char * input2, char * (*func)(const char *, const char *));

#endif

