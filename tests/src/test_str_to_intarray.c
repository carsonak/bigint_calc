#include "tests.h"

void setUp(void)
{
}

void tearDown(void)
{
}

/**
 * test_nullInputs - null inputs
 */
void test_nullInputs(void)
{
	m_uint expected[3] = {1, 0, 0};
	char c = '\0';

	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray(NULL), 2, "NULL => [1, 0]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray(""), 2, "'' => [1, 0]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("\0"), 2, "'\0' => [1, 0]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray((void *)0), 2, "(void *)0 => [1, 0]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray(&c), 2, "&'\0' => [1, 0]");
}

/**
 * test_invalidInputs - strings with unaccepted charaters
 */
void test_invalidInputs(void)
{
	char input[2] = {0};

	TEST_ASSERT_NULL_MESSAGE(str_to_intarray("123-456-789"), "123-456-789");
	TEST_ASSERT_NULL_MESSAGE(str_to_intarray("--40"), "--40");
	TEST_ASSERT_NULL_MESSAGE(str_to_intarray("839475+8889"), "839475+8889");
	TEST_ASSERT_NULL_MESSAGE(str_to_intarray("Two"), "Two");
	for (input[0] = 1; input[0] < 127; input[0]++)
	{
		if (!(input[0] >= '0' && input[0] <= '9') && input[0] != ' ' && input[0] != ',' && input[0] != '-')
			TEST_ASSERT_NULL_MESSAGE(str_to_intarray(input), input);
	}
}

/**
 * test_under10Digits - strings with less than 10 digits
 */
void test_under10Digits(void)
{
	m_uint expected[3] = {1, 123456789, 0};

	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123456789"), 2, "123456789 => [1, 123456789]");
}
