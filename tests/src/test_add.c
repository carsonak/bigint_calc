#include "tests.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_IgnoredTest(void)
{
	TEST_IGNORE_MESSAGE("This Test Was Ignored On Purpose");
}

void test_twoNumbers(void)
{
	mid_uint result[2] = {1, 44};
	mid_uint num1[2] = {1, 22};

	TEST_ASSERT_EQUAL_INT32_ARRAY(result, infiX_addition(num1, num1), 2);
}
