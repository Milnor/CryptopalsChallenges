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

/*
 * @brief Test succeeds if and only if two integers are equal.
 * @param name Description of current test.
 * @param expected The correct result.
 * @param actual The result calculated by the function under test.
 * @return 1 on success; 0 on failure;
 */
int assert_equal_integers(const char * name, int expected, int actual);

/*
 * @brief Test succeeds if and only if two strings are identical.
 * @param name Description of current test.
 * @param expected The correct result.
 * @param actual The result calculated by the function under test.
 * @return 1 on success; 0 on failure;
 */
int assert_equal_strings(const char * name, const char * expected, const char * actual);

/*
 * @brief Test succeeds if and only if two byte arrays are identical.
 * @param name Description of current test.
 * @param expected The correct result.
 * @param actual The result calculated by the function under test.
 * @param length The length of the arrays in bytes.
 * @return 1 on success; 0 on failure;
 */
int assert_equal_bytes(const char * name, uint8_t * expected, uint8_t * actual, size_t length);

#endif

