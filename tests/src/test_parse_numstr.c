#include "tests.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void str_attr_assert(str_attr *expected, str_attr *actual, int ln_num, char *msg);

/**
 * tr_attr_assert - runs checks for str_attr a.k.a struct numstring_attributes
 * @expected: pointer to expected struct
 * @actual: pointer to actual struct
 * @ln_num: line number
 * @msg: message
 */
void str_attr_assert(str_attr *expected, str_attr *actual, int ln_num, char *msg)
{
	char t_msg[100] = {0};

	sprintf(t_msg, "L%d: %s", ln_num, msg);
	TEST_ASSERT_EQUAL_STRING_MESSAGE(expected->str, actual->str, t_msg);
	TEST_ASSERT_EQUAL_size_t_MESSAGE(expected->len, actual->len, t_msg);
	TEST_ASSERT_EQUAL_size_t_MESSAGE(expected->digits, actual->digits, t_msg);
	TEST_ASSERT_EQUAL_UINT8_MESSAGE(expected->is_negative, actual->is_negative, t_msg);
}

/**
 * test_nullInputs - a
 */
void test_nullInputs(void)
{
	TEST_ASSERT_NULL_MESSAGE(parse_numstr(NULL), "NULL");
	TEST_ASSERT_NULL_MESSAGE(parse_numstr(""), "''");
	TEST_ASSERT_NULL_MESSAGE(parse_numstr("\0"), "'\0'");
}

/**
 * test_invalidInputs - strings with unaccepted charaters
 */
void test_invalidInputs(void)
{
	char c[2] = {0};

	TEST_ASSERT_NULL_MESSAGE(parse_numstr("123-456-789"), "123-456-789");
	TEST_ASSERT_NULL_MESSAGE(parse_numstr("--40"), "--40");
	TEST_ASSERT_NULL_MESSAGE(parse_numstr("839475+8889"), "839475+8889");
	TEST_ASSERT_NULL_MESSAGE(parse_numstr("Two"), "Two");
	for (c[0] = 0; c[0] < 127; c[0]++)
	{
		if (!(c[0] >= '0' && c[0] <= '9') && c[0] != ' ' && c[0] != ',' && c[0] != '-')
			TEST_ASSERT_NULL_MESSAGE(parse_numstr(c), c);
	}
}

/**
 * test_onlySpacesCommasAndZeros - strings with only "0", "," and " "
 */
void test_onlySpacesCommasAndZeros(void)
{
	str_attr expected = {(s_uchar *)"0", 1, 1, 0};

	str_attr_assert(&expected, parse_numstr("0"), __LINE__, "0");
	str_attr_assert(&expected, parse_numstr("0000"), __LINE__, "0000");
	str_attr_assert(&expected, parse_numstr("00,000"), __LINE__, "00,000");
	str_attr_assert(&expected, parse_numstr("0 0 0 0"), __LINE__, "0 0 0 0");
	str_attr_assert(&expected, parse_numstr("000, 000, 000,000"), __LINE__, "000, 000, 000,000");
	expected.str = (s_uchar *)" ";
	expected.digits = 0;
	str_attr_assert(&expected, parse_numstr(" "), __LINE__, "' '");
	str_attr_assert(&expected, parse_numstr("    "), __LINE__, "'    '");
	str_attr_assert(&expected, parse_numstr("000    000   00 "), __LINE__, "'000    000   00 '");
	str_attr_assert(&expected, parse_numstr(", "), __LINE__, "', '");
	str_attr_assert(&expected, parse_numstr("    ,    ,   "), __LINE__, "'    ,    ,   '");
	str_attr_assert(&expected, parse_numstr("000, "), __LINE__, "'000, '");
	expected.str = (s_uchar *)",";
	str_attr_assert(&expected, parse_numstr(","), __LINE__, ",");
	str_attr_assert(&expected, parse_numstr(",,,,,,,,,,,,,,,,"), __LINE__, ",,,,,,,,,,,,,,,,");
	str_attr_assert(&expected, parse_numstr("000,,000,0,,,,000,"), __LINE__, "000,,000,0,,,,000,");
	str_attr_assert(&expected, parse_numstr(" ,"), __LINE__, "' ,");
	str_attr_assert(&expected, parse_numstr(", , , ,   , ,  ,,, ,"), __LINE__, ", , , ,   , ,  ,,, ,");
	str_attr_assert(&expected, parse_numstr("0, 0, 0, 0,"), __LINE__, "0, 0, 0, 0,");
}

/**
 * test_onlyNegatives - strings starting with "-".
 */
void test_onlyNegatives(void)
{
	str_attr expected = {(s_uchar *)"0", 1, 1, 1};

	str_attr_assert(&expected, parse_numstr("-0"), __LINE__, "-0");
	str_attr_assert(&expected, parse_numstr("-00000"), __LINE__, "-00000");
	str_attr_assert(&expected, parse_numstr("-000, 000"), __LINE__, "-000, 000");
	str_attr_assert(&expected, parse_numstr("-     0"), __LINE__, "-     0");
	expected.str = (s_uchar *)"1234567";
	expected.len = 7;
	expected.digits = 7;
	str_attr_assert(&expected, parse_numstr("-1234567"), __LINE__, "-1234567");
	expected.str = (s_uchar *)"1,234,567";
	expected.len = 9;
	str_attr_assert(&expected, parse_numstr("-1,234,567"), __LINE__, "-,1,234,567");
	expected.str = (s_uchar *)"1 234 567";
	str_attr_assert(&expected, parse_numstr("- 1 234 567"), __LINE__, "- 1 234 567");
	expected.str = (s_uchar *)"1234567";
	expected.len = 7;
	str_attr_assert(&expected, parse_numstr("-0001234567"), __LINE__, "-0001234567");
	expected.str = (s_uchar *)"1, 234, 567";
	expected.len = 11;
	str_attr_assert(&expected, parse_numstr("-,  1, 234, 567"), __LINE__, "-,  1, 234, 567");
}

/**
 * test_validInputs - a
 */
void test_validInputs(void)
{
	str_attr expected = {(s_uchar *)"0", 1, 1, 0};

	str_attr_assert(&expected, parse_numstr("0"), __LINE__, "0");
	str_attr_assert(&expected, parse_numstr("00000"), __LINE__, "00000");
	str_attr_assert(&expected, parse_numstr("000, 000"), __LINE__, "000, 000");
	str_attr_assert(&expected, parse_numstr("     0"), __LINE__, "     0");
	expected.str = (s_uchar *)"1234567";
	expected.len = 7;
	expected.digits = 7;
	str_attr_assert(&expected, parse_numstr("1234567"), __LINE__, "1234567");
	expected.str = (s_uchar *)"1,234,567";
	expected.len = 9;
	str_attr_assert(&expected, parse_numstr("1,234,567"), __LINE__, ",1,234,567");
	expected.str = (s_uchar *)"1 234 567";
	str_attr_assert(&expected, parse_numstr(" 1 234 567"), __LINE__, " 1 234 567");
	expected.str = (s_uchar *)"1234567";
	expected.len = 7;
	str_attr_assert(&expected, parse_numstr("0001234567"), __LINE__, "0001234567");
	expected.str = (s_uchar *)"1, 234, 56700";
	expected.len = 13;
	expected.digits = 9;
	str_attr_assert(&expected, parse_numstr(",  1, 234, 56700"), __LINE__, "',  1, 234, 56700'");
	str_attr_assert(&expected, parse_numstr(",  1, 234, 56700,"), __LINE__, "',  1, 234, 56700'");
	str_attr_assert(&expected, parse_numstr("1, 234, 56700 "), __LINE__, "1, 234, 56700");
	str_attr_assert(&expected, parse_numstr("1, 234, 56700                                  "), __LINE__, "'1, 234, 56700                                  '");
	str_attr_assert(&expected, parse_numstr("1, 234, 56700,   ,   ,  ,, ,"), __LINE__, "'1, 234, 56700,   ,   ,  ,, ,'");
}
