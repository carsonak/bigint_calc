#include "tests.h"

void setUp(void)
{
}

void tearDown(void)
{
}

/**
 * test_nullInputs - test Null inputs and empty string inputs.
 */
void test_nullInputs(void)
{
	TEST_ASSERT_EQUAL_size_t_MESSAGE(0, padding_chars_len(NULL, NULL), "[NULL, NULL]");
	TEST_ASSERT_EQUAL_size_t_MESSAGE(0, padding_chars_len("", ""), "['', '']");
	TEST_ASSERT_EQUAL_size_t_MESSAGE(0, padding_chars_len("000", NULL), "['000', NULL]");
	TEST_ASSERT_EQUAL_size_t_MESSAGE(0, padding_chars_len(NULL, "000"), "[NULL, '000']");
	TEST_ASSERT_EQUAL_size_t_MESSAGE(0, padding_chars_len("000", ""), "['000', '']");
	TEST_ASSERT_EQUAL_size_t_MESSAGE(0, padding_chars_len("", "000"), "['', '000']");
	TEST_ASSERT_EQUAL_size_t_MESSAGE(0, padding_chars_len("000", "\0"), "['000', '\0']");
	TEST_ASSERT_EQUAL_size_t_MESSAGE(0, padding_chars_len("000", (char *)0), "['000', (char *)0]");
}

/**
 * test_withASCII - test normal ascii character inputs.
 */
void test_withASCII(void)
{
	char c[2] = {0}, text[20] = {0};

	TEST_ASSERT_EQUAL_size_t_MESSAGE(4, padding_chars_len("    Ahem!   000", " "), "['    Ahem!   000', ' ']");
	TEST_ASSERT_EQUAL_size_t_MESSAGE(0, padding_chars_len("123,456, 789", ","), "['123,456, 789', ',']");
	for (c[0] = 0; c[0] < 127; c[0]++)
	{
		sprintf(text, "['%c', '%c']", *c, *c);
		TEST_ASSERT_EQUAL_size_t_MESSAGE(0, padding_chars_len(c, c), text);
	}
}

/**
 * test_withMultiplePaddingChar - multiple padding chars
 */
void test_withMultiplePaddingChars(void)
{
	TEST_ASSERT_EQUAL_size_t_MESSAGE(9, padding_chars_len("000 000 0123 456", "0 "), "['000 000 0123 456', '00 ']");
	TEST_ASSERT_EQUAL_size_t_MESSAGE(12, padding_chars_len("0,00, 000, 0123, 456,789", "0 ,"), "['0,00, 000, 0123, 456,789', '0 ,']");
	TEST_ASSERT_EQUAL_size_t_MESSAGE(11, padding_chars_len("0,00, 000, 0", ", 0"), "['0,00, 000, 0', ', 0']");
}
