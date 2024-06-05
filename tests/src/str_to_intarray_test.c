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
	uint32_t expected[3] = {1, 0, 0};
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
	uint32_t expected[3] = {1, 123456789, 0};

	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123456789"), 2, "=> [1, 123456789]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123 456 789"), 2, "'123 456 789' => [1, 123456789]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123,456,789"), 2, "123,456,789 => [1, 123456789]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123, 456, 789"), 2, "'123, 456, 789' => [1, 123456789]");

	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("   123 456789"), 2, "'   123 456789' => [1, 123456789]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("1 2 34 56 789   "), 2, "'1 2 34 56 789   ' => [1, 123456789]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray(",,123, ,456 7,89"), 2, "',,123, ,456 7,89' => [1, 123456789]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123,456,789, ,,"), 2, "'123,456,789, ,,' => [1, 123456789]");

	expected[1] = 0;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("00"), 2, "00 => [1, 0]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-0"), 2, "'-0' => [1, 0]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("   0"), 2, "'   0' => [1, 0]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("0, 0"), 2, "'0, 0' => [1, 0]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("0,0"), 2, "'0,0' => [1, 0]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("0000,,,"), 2, "'0000,,,' => [1, 0]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("   000,  000,   "), 2, "'   000,  000,   ' => [1, 0]");

	expected[1] = 1 + (NEGBIT_u4b);
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-1"), 2, "'-1' => [1, 1 + (1 << 30)]");
}

/**
 * test_leading0 - input has leading zeros
 */
void test_leading0(void)
{
	uint32_t expected[3] = {1, 12345, 0};

	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("000, 123, 45"), 2, "'000, 123, 45' => [1, 12345]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("00000123, 45"), 2, "'00000123, 45' => [1, 12345]");
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("000 000 000 000 123, 45"), 2, "'000 000 000 000 123, 45' => [1, 12345]");
	expected[1] = 5 + (NEGBIT_u4b);
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-00005"), 2, "'-00005' => [1, 5 + (1 << 30)]");
}

/**
 * test_input1Billion - input has alot of trailing zeros
 */
void test_input1Billion(void)
{
	uint32_t expected[5] = {2, 0, 1, 0};

	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("1 000000000"), 3, "'1 000000000' => [1, 0, 1]");
}

/**
 * test_0sandwich - input has a embedded zeros
 */
void test_0sandwich(void)
{
	uint32_t expected[7] = {3, 78234587, 0, 909897004, 0, 0, 0};

	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("909897004 000000000 078234587"), 4, "'909897004 000000000 078234587' => [3, 78234587, 0, 909897004]");

	expected[3] += (NEGBIT_u4b);
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-,909897004, 000000000, 078234587,"), 4, "'-,909897004, 000000000, 078234587,' => [3, 78234587, 0, 909897004 + (1 << 30)]");

	expected[0] = 4;
	expected[1] = 4000000;
	expected[2] = 300000000;
	expected[3] = 200000000;
	expected[4] = 100000000;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("00000,100000000,200000000,300000000,004000000"), 5, "'00000,100000000,200000000,300000000,004000000' => [4, 4000000, 300000000, 200000000, 100000000]");
}

/**
 * test_increaseTo27Digits - gradual increase from 0 digits to 27 digits
 */
void test_increaseTo27Digits(void)
{
	uint32_t expected[5] = {1, 0, 0, 0};

	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("0"), 2, "'0' => [1, 0]");
	expected[1] = 1;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("1"), 2, "'1' => [1, 1]");
	expected[1] = 12;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("12"), 2, "'12' => [1, 12]");
	expected[1] = 123;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123"), 2, "'123' => [1, 123]");
	expected[1] = 1234;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("1234"), 2, "'1234' => [1, 1234]");
	expected[1] = 12345;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("12345"), 2, "'12345' => [1, 12345]");
	expected[1] = 123456;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123456"), 2, "'123456' => [1, 123456]");
	expected[1] = 1234567;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("1234567"), 2, "'1234567' => [1, 1234567]");
	expected[1] = 12345678;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("12345678"), 2, "'12345678' => [1, 12345678]");
	expected[1] = 123456789;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123456789"), 2, "'123456789' => [1, 123456789]");

	expected[0] += 1;
	expected[2] = 1;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("1 123456789"), 3, "'1 123456789' => [1, 123456789, 1]");
	expected[2] = 12;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("12 123456789"), 3, "'12 123456789' => [1, 123456789, 12]");
	expected[2] = 123;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123 123456789"), 3, "'123 123456789' => [1, 123456789, 123]");
	expected[2] = 1234;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("1234 123456789"), 3, "'1234 123456789' => [1, 123456789, 1234]");
	expected[2] = 12345;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("12345 123456789"), 3, "'12345 123456789' => [1, 123456789, 12345]");
	expected[2] = 123456;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123456 123456789"), 3, "'123456 123456789' => [1, 123456789, 123456]");
	expected[2] = 1234567;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("1234567 123456789"), 3, "'1234567 123456789' => [1, 123456789, 1234567]");
	expected[2] = 12345678;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("12345678 123456789"), 3, "'12345678 123456789' => [1, 123456789, 12345678]");
	expected[2] = 123456789;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123456789 123456789"), 3, "'123456789 123456789' => [1, 123456789, 123456789]");

	expected[0] += 1;
	expected[3] = 1;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("1 123456789 123456789"), 4, "'1 123456789 123456789' => [1, 123456789, 123456789, 1]");
	expected[3] = 12;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("12 123456789 123456789"), 4, "'12 123456789 123456789' => [1, 123456789, 123456789, 12]");
	expected[3] = 123;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123 123456789 123456789"), 4, "'123 123456789 123456789' => [1, 123456789, 123456789, 123]");
	expected[3] = 1234;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("1234 123456789 123456789"), 4, "'1234 123456789 123456789' => [1, 123456789, 123456789, 1234]");
	expected[3] = 12345;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("12345 123456789 123456789"), 4, "'12345 123456789 123456789' => [1, 123456789, 123456789, 12345]");
	expected[3] = 123456;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123456 123456789 123456789"), 4, "'123456 123456789 123456789' => [1, 123456789, 123456789, 123456]");
	expected[3] = 1234567;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("1234567 123456789 123456789"), 4, "'1234567 123456789 123456789' => [1, 123456789, 123456789, 1234567]");
	expected[3] = 12345678;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("12345678 123456789 123456789"), 4, "'12345678 123456789 123456789' => [1, 123456789, 123456789, 12345678]");
	expected[3] = 123456789;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("123456789 123456789 123456789"), 4, "'123456789 123456789 123456789' => [1, 123456789, 123456789, 123456789]");
}

/**
 * test_increaseTo27DigitsNegatives - gradual increase from 0 digits to 27 digits (negative values)
 */
void test_increaseTo27DigitsNegatives(void)
{
	uint32_t expected[5] = {1, 0, 0, 0};

	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-0"), 2, "'0' => [1, 0]");
	expected[1] = (NEGBIT_u4b) + 1;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-1"), 2, "'1' => [1, 1 + (1 << 30)]");
	expected[1] = (NEGBIT_u4b) + 12;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-12"), 2, "'12' => [1, 12 + (1 << 30)]");
	expected[1] = (NEGBIT_u4b) + 123;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-123"), 2, "'123' => [1, 123 + (1 << 30)]");
	expected[1] = (NEGBIT_u4b) + 1234;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-1234"), 2, "'1234' => [1, 1234 + (1 << 30)]");
	expected[1] = (NEGBIT_u4b) + 12345;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-12345"), 2, "'12345' => [1, 12345 + (1 << 30)]");
	expected[1] = (NEGBIT_u4b) + 123456;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-123456"), 2, "'123456' => [1, 123456 + (1 << 30)]");
	expected[1] = (NEGBIT_u4b) + 1234567;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-1234567"), 2, "'1234567' => [1, 1234567 + (1 << 30)]");
	expected[1] = (NEGBIT_u4b) + 12345678;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-12345678"), 2, "'12345678' => [1, 12345678 + (1 << 30)]");
	expected[1] = (NEGBIT_u4b) + 123456789;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-123456789"), 2, "'123456789' => [1, 123456789 + (1 << 30)]");

	expected[0] += 1;
	expected[1] -= (NEGBIT_u4b);
	expected[2] = (NEGBIT_u4b) + 1;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-1 123456789"), 3, "'1 123456789' => [1, 123456789, 1 + (1 << 30)]");
	expected[2] = (NEGBIT_u4b) + 12;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-12 123456789"), 3, "'12 123456789' => [1, 123456789, 12 + (1 << 30)]");
	expected[2] = (NEGBIT_u4b) + 123;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-123 123456789"), 3, "'123 123456789' => [1, 123456789, 123 + (1 << 30)]");
	expected[2] = (NEGBIT_u4b) + 1234;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-1234 123456789"), 3, "'1234 123456789' => [1, 123456789, 1234 + (1 << 30)]");
	expected[2] = (NEGBIT_u4b) + 12345;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-12345 123456789"), 3, "'12345 123456789' => [1, 123456789, 12345 + (1 << 30)]");
	expected[2] = (NEGBIT_u4b) + 123456;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-123456 123456789"), 3, "'123456 123456789' => [1, 123456789, 123456 + (1 << 30)]");
	expected[2] = (NEGBIT_u4b) + 1234567;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-1234567 123456789"), 3, "'1234567 123456789' => [1, 123456789, 1234567 + (1 << 30)]");
	expected[2] = (NEGBIT_u4b) + 12345678;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-12345678 123456789"), 3, "'12345678 123456789' => [1, 123456789, 12345678 + (1 << 30)]");
	expected[2] = (NEGBIT_u4b) + 123456789;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-123456789 123456789"), 3, "'123456789 123456789' => [1, 123456789, 123456789 + (1 << 30)]");

	expected[0] += 1;
	expected[2] -= (NEGBIT_u4b);
	expected[3] = (NEGBIT_u4b) + 1;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-1 123456789 123456789"), 4, "'1 123456789 123456789' => [1, 123456789, 123456789, 1 + (1 << 30)]");
	expected[3] = (NEGBIT_u4b) + 12;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-12 123456789 123456789"), 4, "'12 123456789 123456789' => [1, 123456789, 123456789, 12 + (1 << 30)]");
	expected[3] = (NEGBIT_u4b) + 123;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-123 123456789 123456789"), 4, "'123 123456789 123456789' => [1, 123456789, 123456789, 123 + (1 << 30)]");
	expected[3] = (NEGBIT_u4b) + 1234;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-1234 123456789 123456789"), 4, "'1234 123456789 123456789' => [1, 123456789, 123456789, 1234 + (1 << 30)]");
	expected[3] = (NEGBIT_u4b) + 12345;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-12345 123456789 123456789"), 4, "'12345 123456789 123456789' => [1, 123456789, 123456789, 12345 + (1 << 30)]");
	expected[3] = (NEGBIT_u4b) + 123456;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-123456 123456789 123456789"), 4, "'123456 123456789 123456789' => [1, 123456789, 123456789, 123456 + (1 << 30)]");
	expected[3] = (NEGBIT_u4b) + 1234567;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-1234567 123456789 123456789"), 4, "'1234567 123456789 123456789' => [1, 123456789, 123456789, 1234567 + (1 << 30)]");
	expected[3] = (NEGBIT_u4b) + 12345678;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-12345678 123456789 123456789"), 4, "'12345678 123456789 123456789' => [1, 123456789, 123456789, 12345678 + (1 << 30)]");
	expected[3] = (NEGBIT_u4b) + 123456789;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(expected, str_to_intarray("-123456789 123456789 123456789"), 4, "'123456789 123456789 123456789' => [1, 123456789, 123456789, 123456789 + (1 << 30)]");
}
