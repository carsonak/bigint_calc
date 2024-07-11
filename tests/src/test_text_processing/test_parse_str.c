#include "tests.h"

numstr *output = NULL;

/**
 * setup - setup some variables.
 */
void setup(void) {}

/**
 * teardown - cleanup after tests.
 */
void teardown(void)
{
	output = free_numstr(output);
}

TestSuite(null_inputs);

Test(null_inputs, test_NULL, .description = "NULL", .timeout = 3.0)
{
	cr_assert(zero(ptr, parse_number(NULL, 10)));
}

Test(null_inputs, test_null, .description = "\0001", .timeout = 3.0)
{
	cr_assert(zero(ptr, parse_number("\0001", 10)));
}

TestSuite(invalid_inputs);

Test(invalid_inputs, test_all_nonvalid_charbits,
	 .description = "invalid chars", .timeout = 3.0)
{
	char c[2] = {SCHAR_MIN, '\0'};
	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX; c[0]++)
	{
		if (!isalnum(c[0]))
			cr_assert(zero(ptr, parse_number(c, 36)));
	}
}

Test(invalid_inputs, test_all_nonvalid_charbits2,
	 .description = "invalid 2chars", .timeout = 3.0)
{
	char c[3] = {SCHAR_MIN, SCHAR_MIN, '\0'};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX; c[0]++, c[1]++)
	{
		if (!isalnum(c[0]))
			cr_assert(zero(ptr, parse_number(c, 36)));
	}
}

Test(invalid_inputs, test_all_nonvalid_charbits2alt,
	 .description = "invalid 2chars alt", .timeout = 3.0)
{
	char c[3] = {SCHAR_MIN, SCHAR_MAX, '\0'};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX && c[1] > SCHAR_MIN; c[0]++, c[1]--)
	{
		if (!isalnum(c[0]) || !isalnum(c[1]))
			cr_assert(zero(ptr, parse_number(c, 36)));
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
			if (!isalnum(c[2]) && c[2] != '\0' && c[2] != '.')
				cr_assert(zero(ptr, parse_number(c, 36)));
		}
	}

	c[0] = '-';
	for (c[1] = '0'; c[1] <= '9'; c[1]++)
	{
		for (c[2] = SCHAR_MIN; c[2] < SCHAR_MAX; c[2]++)
		{
			if (!isalnum(c[2]) && c[2] != '\0' && c[2] != '.')
				cr_assert(zero(ptr, parse_number(c, 36)));
		}
	}
}

Test(invalid_inputs, test_leading_separators_signs,
	 .description = "Assortment of '.-_,+'", .timeout = 3.0)
{
	cr_redirect_stderr();
	cr_assert(zero(ptr, parse_number("---___", 10)));
	cr_assert(zero(ptr, parse_number("---,,,", 10)));
	cr_assert(zero(ptr, parse_number("+++___,,,", 10)));
	cr_assert(zero(ptr, parse_number("+++_,_,_,", 10)));
	cr_assert(zero(ptr, parse_number("-+-+-+-.", 10)));
	cr_assert(zero(ptr, parse_number("+-+-+-+.", 10)));
	cr_assert(zero(ptr, parse_number("---___.", 10)));
	cr_assert(zero(ptr, parse_number("---,,,.", 10)));
	cr_assert(zero(ptr, parse_number("+++___,,,.", 10)));
	cr_assert(zero(ptr, parse_number("+++_,_,_,.", 10)));
}

Test(invalid_inputs, test_invalid_chars_mixed_with_valid_chars,
	 .description = "Assortment of valid and invalid chars", .timeout = 3.0)
{
	cr_redirect_stderr();
	cr_assert(zero(ptr, parse_number("123-567", 10)));
	cr_assert(zero(ptr, parse_number("123 567", 10)));
	cr_assert(zero(ptr, parse_number("123\"567\"", 10)));
	cr_assert(zero(ptr, parse_number("123+567", 10)));
	cr_assert(zero(ptr, parse_number("123/567", 10)));
	cr_assert(zero(ptr, parse_number("123*567", 10)));
	cr_assert(zero(ptr, parse_number("123^567", 10)));
	cr_assert(zero(ptr, parse_number("123(567)", 10)));
	cr_assert(zero(ptr, parse_number("123>567", 10)));
}

Test(invalid_inputs, test_34base2, .description = "+34", .timeout = 3.0)
{
	cr_assert(zero(ptr, parse_number("+34", 2)));
}

Test(invalid_inputs, test_leading_underscore,
	 .description = "_20", .timeout = 3.0)
{
	cr_assert(zero(ptr, parse_number("_20", 10)));
}

Test(invalid_inputs, test_trailing_underscore,
	 .description = "20_", .timeout = 3.0)
{
	cr_assert(zero(ptr, parse_number("20_", 10)));
}

Test(invalid_inputs, test_leading_comma,
	 .description = ",20", .timeout = 3.0)
{
	cr_redirect_stderr();
	cr_assert(zero(ptr, parse_number(",20", 10)));
}

Test(invalid_inputs, test_trailing_comma,
	 .description = "20,", .timeout = 3.0)
{
	cr_redirect_stderr();
	cr_assert(zero(ptr, parse_number("20,", 10)));
}

Test(invalid_inputs, test_neg_leading_underscore,
	 .description = "-_20", .timeout = 3.0)
{
	cr_redirect_stderr();
	cr_assert(zero(ptr, parse_number("-_20", 10)));
}

Test(invalid_inputs, test_neg_leading_comma,
	 .description = "-,20", .timeout = 3.0)
{
	cr_redirect_stderr();
	cr_assert(zero(ptr, parse_number("-,20", 10)));
}

Test(invalid_inputs, test_pos_leading_underscore,
	 .description = "+_20", .timeout = 3.0)
{
	cr_redirect_stderr();
	cr_assert(zero(ptr, parse_number("+_20", 10)));
}

Test(invalid_inputs, test_pos_leading_comma,
	 .description = "+,20", .timeout = 3.0)
{
	cr_redirect_stderr();
	cr_assert(zero(ptr, parse_number("+,20", 10)));
}

TestSuite(valid_inputs, .init = setup, .fini = teardown);

Test(valid_inputs, test_decimals1, .description = "0-9", .timeout = 3.0)
{
	char c[2] = {'0', '\0'};

	for (; c[0] <= '9'; c[0]++)
	{
		output = parse_number(c, 10);
		cr_assert(eq(sz, output->len, 1));
		cr_assert(eq(chr, output->is_negative, false));
		cr_assert(eq(str, output->str, c));
		output = free_numstr(output);
	}
}

Test(valid_inputs, test_uppercase_letters1,
	 .description = "A-Z", .timeout = 3.0)
{
	char c[2] = {'A', '\0'};

	for (; c[0] <= 'Z'; c[0]++)
	{
		output = parse_number(c, 36);
		cr_assert(eq(sz, output->len, 1));
		cr_assert(eq(chr, output->is_negative, false));
		cr_assert(eq(str, output->str, c));
		output = free_numstr(output);
	}
}

Test(valid_inputs, test_lowercase_letters1,
	 .description = "a-z", .timeout = 3.0)
{
	char c[2] = {'a', '\0'}, out[2] = {'A', '\0'};

	for (; c[0] <= 'z' && out[0] <= 'Z'; c[0]++, out[0]++)
	{
		output = parse_number(c, 36);
		cr_assert(eq(sz, output->len, 1));
		cr_assert(eq(chr, output->is_negative, false));
		cr_assert(eq(str, output->str, out));
		output = free_numstr(output);
	}
}

Test(valid_inputs, test_decimals2,
	 .description = "10-99", .timeout = 3.0)
{
	char c[3] = {'1', '0', '\0'};

	for (; c[0] <= '9'; c[0]++)
	{
		for (c[1] = '0'; c[1] <= '9'; c[1]++)
		{
			output = parse_number(c, 36);
			cr_assert(eq(sz, output->len, 2));
			cr_assert(eq(chr, output->is_negative, false));
			cr_assert(eq(str, output->str, c));
			output = free_numstr(output);
		}
	}
}

Test(valid_inputs, test_uppercase_letters2,
	 .description = "AA-ZZ", .timeout = 3.0)
{
	char c[3] = {'A', 'A', '\0'};

	for (; c[0] <= 'Z'; c[0]++)
	{
		for (c[1] = 'A'; c[1] <= 'Z'; c[1]++)
		{
			output = parse_number(c, 36);
			cr_assert(eq(sz, output->len, 2));
			cr_assert(eq(chr, output->is_negative, false));
			cr_assert(eq(str, output->str, c));
			output = free_numstr(output);
		}
	}
}

Test(valid_inputs, test_lowercase_letters2,
	 .description = "aa-zz", .timeout = 3.0)
{
	char c[3] = {'a', 'a', '\0'}, out[3] = {'A', 'A', '\0'};

	for (; c[0] <= 'z' && out[0] <= 'Z'; c[0]++, out[0]++)
	{
		for (c[1] = 'a', out[1] = 'A';
			 c[1] <= 'z' && out[1] <= 'Z';
			 c[1]++, out[1]++)
		{
			output = parse_number(c, 36);
			cr_assert(eq(sz, output->len, 2));
			cr_assert(eq(chr, output->is_negative, false));
			cr_assert(eq(str, output->str, out));
			output = free_numstr(output);
		}
	}
}

Test(valid_inputs, test_0000000000000,
	 .description = "0000000000000", .timeout = 3.0)
{
	output = parse_number("0000000000000", 10);

	cr_assert(eq(sz, output->len, 1));
	cr_assert(eq(chr, output->is_negative, false));
	cr_assert(eq(str, output->str, "0"));
}

Test(valid_inputs, test_neg0000000000000,
	 .description = "-0000000000000", .timeout = 3.0)
{
	output = parse_number("-0000000000000", 10);

	cr_assert(eq(sz, output->len, 1));
	cr_assert(eq(chr, output->is_negative, true));
	cr_assert(eq(str, output->str, "0"));
}

Test(valid_inputs, test_00000000000001,
	 .description = "00000000000001", .timeout = 3.0)
{
	output = parse_number("00000000000001", 10);

	cr_assert(eq(sz, output->len, 1));
	cr_assert(eq(chr, output->is_negative, false));
	cr_assert(eq(str, output->str, "1"));
}

Test(valid_inputs, test_neg00000000000001,
	 .description = "-00000000000001", .timeout = 3.0)
{
	output = parse_number("-00000000000001", 10);

	cr_assert(eq(sz, output->len, 1));
	cr_assert(eq(chr, output->is_negative, true));
	cr_assert(eq(str, output->str, "1"));
}

Test(valid_inputs, test_mmneg10_000_000_000,
	 .description = "-10_000_000_000", .timeout = 3.0)
{
	output = parse_number("---10_000_000_000", 10);

	cr_assert(eq(sz, output->len, 11));
	cr_assert(eq(chr, output->is_negative, true));
	cr_assert(eq(str, output->str, "10000000000"));
}

Test(valid_inputs, test_neg1, .description = "--ZZZ,4Ar,YU8,012,qa9", .timeout = 3.0)
{
	output = parse_number("--ZZZ,4Ar,YU8,012,qa9", 36);

	cr_assert(eq(sz, output->len, 15));
	cr_assert(eq(chr, output->is_negative, false));
	cr_assert(eq(str, output->str, "ZZZ4ARYU8012QA9"));
}

Test(valid_inputs, test_2000, .description = "2,0__0,,,0", .timeout = 3.0)
{
	output = parse_number("2,0__0,,,0", 10);

	cr_assert(eq(sz, output->len, 4));
	cr_assert(eq(chr, output->is_negative, false));
	cr_assert(eq(str, output->str, "2000"));
}
