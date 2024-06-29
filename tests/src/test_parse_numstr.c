#include "tests.h"

intstr *output = NULL;

/**
 * setup - setup some variables
 */
void setup(void) {}

/**
 * teardown - cleanup after tests
 */
void teardown(void)
{
	output = free_numstr_array(output);
}

TestSuite(null_inputs, .init = setup, .fini = teardown);

Test(null_inputs, test_NULL, .description = "NULL", .timeout = 3.0)
{
	cr_assert(zero(ptr, parse_numstr(NULL)));
}

Test(null_inputs, test_null, .description = "\0", .timeout = 3.0)
{
	cr_assert(zero(ptr, parse_numstr("\0")));
}

TestSuite(invalid_inputs, .init = setup, .fini = teardown);

Test(invalid_inputs, test_all_nonvalid_charbits,
	 .description = "invalid chars", .timeout = 3.0)
{
	char c[2] = {SCHAR_MIN, '\0'};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX; c[0]++)
	{
		if (!(c[0] >= '0' && c[0] <= '9'))
			cr_assert(zero(ptr, parse_numstr(c)));
	}
}

Test(invalid_inputs, test_all_nonvalid_charbits2,
	 .description = "invalid 1char", .timeout = 3.0)
{
	char c[3] = {SCHAR_MIN, SCHAR_MIN, '\0'};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX; c[0]++, c[1]++)
	{
		if (!(c[0] >= '0' && c[0] <= '9'))
			cr_assert(zero(ptr, parse_numstr(c)));
	}
}

Test(invalid_inputs, test_all_nonvalid_charbits2alt,
	 .description = "invalid 2chars", .timeout = 3.0)
{
	char c[3] = {SCHAR_MIN, SCHAR_MAX, '\0'};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX && c[1] > SCHAR_MIN; c[0]++, c[1]--)
	{
		if (!(c[0] >= '0' && c[0] <= '9') && !(c[1] >= '0' && c[1] <= '9'))
			cr_assert(zero(ptr, parse_numstr(c)));
	}
}

Test(invalid_inputs, test_all_invalid_charbits3,
	 .description = "invalid 3chars", .timeout = 3.0)
{
	char c[] = {'\0', '\0', '\0', '\0'};

	cr_redirect_stderr();
	c[0] = '+';
	for (c[1] = '0'; c[1] <= '9'; c[1]++)
	{
		for (c[2] = SCHAR_MIN; c[2] < SCHAR_MAX; c[2]++)
		{
			if (!(c[2] >= '0' && c[2] <= '9') && c[2] != '\0' && c[2] != ',' &&
				c[2] != '_' && c[2] != '.')
				cr_assert(zero(ptr, parse_numstr(c)));
		}
	}

	c[0] = '-';
	for (c[1] = '0'; c[1] <= '9'; c[1]++)
	{
		for (c[2] = SCHAR_MIN; c[2] < SCHAR_MAX; c[2]++)
		{
			if (!(c[2] >= '0' && c[2] <= '9') && c[2] != '\0' && c[2] != ',' &&
				c[2] != '_' && c[2] != '.')
				cr_assert(zero(ptr, parse_numstr(c)));
		}
	}
}

Test(invalid_inputs, test_leading_separators_signs,
	 .description = "Assortment of '.-_,+'", .timeout = 3.0)
{
	cr_redirect_stderr();
	cr_assert(zero(ptr, parse_numstr("---___")));
	cr_assert(zero(ptr, parse_numstr("---,,,")));
	cr_assert(zero(ptr, parse_numstr("+++___,,,")));
	cr_assert(zero(ptr, parse_numstr("+++_,_,_,")));
	cr_assert(zero(ptr, parse_numstr("-+-+-+-.")));
	cr_assert(zero(ptr, parse_numstr("+-+-+-+.")));
	cr_assert(zero(ptr, parse_numstr("---___.")));
	cr_assert(zero(ptr, parse_numstr("---,,,.")));
	cr_assert(zero(ptr, parse_numstr("+++___,,,.")));
	cr_assert(zero(ptr, parse_numstr("+++_,_,_,.")));
}

Test(invalid_inputs, test_invalid_chars_mixed_with_valid_chars,
	 .description = "Assortment of valid and invalid chars", .timeout = 3.0)
{
	cr_redirect_stderr();
	cr_assert(zero(ptr, parse_numstr("123-567")));
	cr_assert(zero(ptr, parse_numstr("123 567")));
	cr_assert(zero(ptr, parse_numstr("123\"567\"")));
	cr_assert(zero(ptr, parse_numstr("123+567")));
	cr_assert(zero(ptr, parse_numstr("123/567")));
	cr_assert(zero(ptr, parse_numstr("123*567")));
	cr_assert(zero(ptr, parse_numstr("123^567")));
	cr_assert(zero(ptr, parse_numstr("123(567)")));
	cr_assert(zero(ptr, parse_numstr("123>567")));
}

TestSuite(valid_inputs, .init = setup, .fini = teardown);

Test(valid_inputs, test_all_valid_charbits,
	 .description = "valid chars", .timeout = 3.0)
{
	char c[2] = {'0', '\0'};

	for (; c[0] <= '9'; c[0]++)
	{
		output = parse_numstr(c);
		cr_assert(eq(sz, output->len, 1));
		cr_assert(eq(chr, output->is_negative, '\0'));
		cr_assert(eq(str, output->str, c));
		output = free_numstr_array(output);
	}
}

Test(valid_inputs, test_all_valid_charbits2,
	 .description = "valid 2chars", .timeout = 3.0)
{
	char c[3] = {'1', '1', '\0'};

	for (; c[0] <= '9'; c[0]++, c[1]++)
	{
		output = parse_numstr(c);
		cr_assert(eq(sz, output->len, 2));
		cr_assert(eq(chr, output->is_negative, '\0'));
		cr_assert(eq(str, output->str, c));
		output = free_numstr_array(output);
	}
}

Test(valid_inputs, test_all_valid_charbits2alt,
	 .description = "valid 2chars alt", .timeout = 3.0)
{
	char c[3] = {'1', '9', '\0'};

	while (c[0] <= '9' && c[1] >= '0')
	{
		output = parse_numstr(c);
		cr_assert(eq(sz, output->len, 2));
		cr_assert(eq(chr, output->is_negative, '\0'));
		cr_assert(eq(str, output->str, c));
		output = free_numstr_array(output);

		if (c[0] < '9')
			c[0]++;

		if (c[1] >= '0')
			c[1]--;
	}
}

Test(valid_inputs, test_0000000000000,
	 .description = "0000000000000 == 0", .timeout = 3.0)
{
	output = parse_numstr("0000000000000");

	cr_assert(eq(sz, output->len, 1));
	cr_assert(eq(chr, output->is_negative, '\0'));
	cr_assert(eq(str, output->str, "0"));
}

Test(valid_inputs, test_minus0000000000000,
	 .description = "-0000000000000 == 0", .timeout = 3.0)
{
	output = parse_numstr("-0000000000000");

	cr_assert(eq(sz, output->len, 1));
	cr_assert(eq(chr, output->is_negative, (char)1));
	cr_assert(eq(str, output->str, "0"));
}

Test(valid_inputs, test_00000000000001,
	 .description = "00000000000001 == 1", .timeout = 3.0)
{
	output = parse_numstr("00000000000001");

	cr_assert(eq(sz, output->len, 1));
	cr_assert(eq(chr, output->is_negative, '\0'));
	cr_assert(eq(str, output->str, "1"));
}

Test(valid_inputs, test_minus00000000000001,
	 .description = "-00000000000001 == 1", .timeout = 3.0)
{
	output = parse_numstr("-00000000000001");

	cr_assert(eq(sz, output->len, 1));
	cr_assert(eq(chr, output->is_negative, (char)1));
	cr_assert(eq(str, output->str, "1"));
}
