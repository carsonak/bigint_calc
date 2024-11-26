#include "tests.h"

static struct numstr *output;
static size_t processed;

/**
 * setup - setup some variables.
 */
void setup(void)
{
	output = NULL;
	processed = 0;
}

/**
 * teardown - cleanup after tests.
 */
void teardown(void)
{
	output = free_numstr(output);
}

TestSuite(null_inputs);

Test(null_inputs, test_NULL, .description = "NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, str_to_numstr(NULL, 10, &processed)));
	cr_assert(zero(sz, processed));
}

Test(null_inputs, test_null, .description = "\0001", .timeout = 1.0)
{
	cr_assert(zero(ptr, str_to_numstr("\0001", 10, &processed)));
	cr_assert(zero(sz, processed));
}

TestSuite(invalid_inputs);

Test(invalid_inputs, test_all_nonvalid_chars,
	 .description = "invalid chars", .timeout = 1.0)
{
	char c[2] = {SCHAR_MIN, '\0'};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX; c[0]++)
	{
		if (!isalnum(c[0]))
			cr_assert(zero(ptr, str_to_numstr(c, 36, NULL)));
	}
}

Test(invalid_inputs, test_all_nonvalid_chars2,
	 .description = "invalid 2chars", .timeout = 1.0)
{
	char c[3] = {SCHAR_MIN, SCHAR_MAX, '\0'};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX && c[1] > SCHAR_MIN; c[0]++, c[1]--)
	{
		if (isalnum(c[0]) || (c[0] == '-' && c[0] == '+' && isalnum(c[1])))
			continue;

		cr_assert(zero(ptr, str_to_numstr(c, 36, NULL)));
	}
}

Test(invalid_inputs, test_leading_separators_signs,
	 .description = "Assortment of '.-_,+'", .timeout = 1.0)
{
	cr_redirect_stderr();
	/* Leading Separator. */
	cr_assert(zero(ptr, str_to_numstr("---___", 10, &processed)));
	cr_assert(eq(sz, processed, 3));
	cr_assert(zero(ptr, str_to_numstr("+++___", 10, &processed)));
	cr_assert(eq(sz, processed, 3));
	cr_assert(zero(ptr, str_to_numstr("+++---__123", 10, &processed)));
	cr_assert(eq(sz, processed, 6));
	cr_assert(zero(ptr, str_to_numstr("-+-+-+-_0", 10, &processed)));
	cr_assert(eq(sz, processed, 7));
	/* No valid characters. */
	cr_assert(zero(ptr, str_to_numstr("---", 10, &processed)));
	cr_assert(eq(sz, processed, 3));
	cr_assert(zero(ptr, str_to_numstr("-+-+-+-", 10, &processed)));
	cr_assert(eq(sz, processed, 7));
	cr_assert(zero(ptr, str_to_numstr("+++", 10, NULL)));
}

Test(invalid_inputs, test_leading_underscore,
	 .description = "_20", .timeout = 1.0)
{
	cr_assert(zero(ptr, str_to_numstr("_20", 10, &processed)));
	cr_assert(eq(sz, processed, 0));
}

Test(invalid_inputs, test_trailing_underscore,
	 .description = "20_", .timeout = 1.0)
{
	cr_assert(zero(ptr, str_to_numstr("20_", 10, &processed)));
	cr_assert(eq(sz, processed, 2));
}

Test(invalid_inputs, test_neg_leading_underscore,
	 .description = "-_20", .timeout = 1.0)
{
	cr_redirect_stderr();
	cr_assert(zero(ptr, str_to_numstr("-_20", 10, &processed)));
	cr_assert(eq(sz, processed, 1));
}

Test(invalid_inputs, test_pos_leading_underscore,
	 .description = "+_20", .timeout = 1.0)
{
	cr_redirect_stderr();
	cr_assert(zero(ptr, str_to_numstr("+_20", 10, &processed)));
	cr_assert(eq(sz, processed, 1));
}

TestSuite(invalid_chars_in_str, .init = setup, .fini = teardown);

Test(invalid_chars_in_str, test_valid_chars_with_dash_in_middle,
	 .description = "123-567", .timeout = 1.0)
{
	output = str_to_numstr("123-567", 10, &processed);

	cr_assert(eq(sz, output->len, 3));
	cr_assert(eq(sz, processed, 3));
	cr_assert(eq(chr, output->is_negative, false));
	cr_assert(eq(str, output->str, "123"));
}

Test(invalid_chars_in_str, test_valid_chars_with_plus_in_middle,
	 .description = "123+567", .timeout = 1.0)
{
	output = str_to_numstr("123+567", 10, &processed);

	cr_assert(eq(sz, output->len, 3));
	cr_assert(eq(sz, processed, 3));
	cr_assert(eq(chr, output->is_negative, false));
	cr_assert(eq(str, output->str, "123"));
}

Test(invalid_chars_in_str, test_valid_chars_with_space_in_middle,
	 .description = "123 567", .timeout = 1.0)
{
	output = str_to_numstr("123 567", 10, &processed);

	cr_assert(eq(sz, output->len, 3));
	cr_assert(eq(sz, processed, 3));
	cr_assert(eq(chr, output->is_negative, false));
	cr_assert(eq(str, output->str, "123"));
}

Test(invalid_chars_in_str, test_valid_chars_with_bracket_in_middle,
	 .description = "123(567)", .timeout = 1.0)
{
	output = str_to_numstr("123(567)", 10, &processed);

	cr_assert(eq(sz, output->len, 3));
	cr_assert(eq(sz, processed, 3));
	cr_assert(eq(chr, output->is_negative, false));
	cr_assert(eq(str, output->str, "123"));
}

TestSuite(valid_inputs, .init = setup, .fini = teardown);

Test(valid_inputs, test_decimals1, .description = "0-9", .timeout = 1.0)
{
	char c[2] = {'0', '\0'};

	for (; c[0] <= '9'; c[0]++)
	{
		output = str_to_numstr(c, 10, &processed);

		cr_assert(eq(sz, output->len, 1));
		cr_assert(eq(sz, processed, 1));
		cr_assert(eq(chr, output->is_negative, false));
		cr_assert(eq(str, output->str, c));
		output = free_numstr(output);
	}
}

Test(valid_inputs, test_uppercase_letters1,
	 .description = "A-Z", .timeout = 1.0)
{
	char c[2] = {'A', '\0'};

	for (; c[0] <= 'Z'; c[0]++)
	{
		output = str_to_numstr(c, 36, &processed);

		cr_assert(eq(sz, output->len, 1));
		cr_assert(eq(sz, processed, 1));
		cr_assert(eq(chr, output->is_negative, false));
		cr_assert(eq(str, output->str, c));
		output = free_numstr(output);
	}
}

Test(valid_inputs, test_lowercase_letters1,
	 .description = "a-z", .timeout = 1.0)
{
	char c[2] = {'a', '\0'}, out[2] = {'A', '\0'};

	for (; c[0] <= 'z' && out[0] <= 'Z'; c[0]++, out[0]++)
	{
		output = str_to_numstr(c, 36, &processed);

		cr_assert(eq(sz, output->len, 1));
		cr_assert(eq(sz, processed, 1));
		cr_assert(eq(chr, output->is_negative, false));
		cr_assert(eq(str, output->str, out));
		output = free_numstr(output);
	}
}

Test(valid_inputs, test_decimals2,
	 .description = "10-99", .timeout = 1.0)
{
	char c[3] = {'1', '0', '\0'};

	for (; c[0] <= '9'; c[0]++)
	{
		for (c[1] = '0'; c[1] <= '9'; c[1]++)
		{
			output = str_to_numstr(c, 36, &processed);

			cr_assert(eq(sz, output->len, 2));
			cr_assert(eq(sz, processed, 2));
			cr_assert(eq(chr, output->is_negative, false));
			cr_assert(eq(str, output->str, c));
			output = free_numstr(output);
		}
	}
}

Test(valid_inputs, test_uppercase_letters2,
	 .description = "AA-ZZ", .timeout = 1.0)
{
	char c[3] = {'A', 'A', '\0'};

	for (; c[0] <= 'Z'; c[0]++)
	{
		for (c[1] = 'A'; c[1] <= 'Z'; c[1]++)
		{
			output = str_to_numstr(c, 36, &processed);

			cr_assert(eq(sz, output->len, 2));
			cr_assert(eq(sz, processed, 2));
			cr_assert(eq(chr, output->is_negative, false));
			cr_assert(eq(str, output->str, c));
			output = free_numstr(output);
		}
	}
}

Test(valid_inputs, test_lowercase_letters2,
	 .description = "aa-zz", .timeout = 1.0)
{
	char c[3] = {'a', 'a', '\0'}, out[3] = {'A', 'A', '\0'};

	for (; c[0] <= 'z' && out[0] <= 'Z'; c[0]++, out[0]++)
	{
		for (c[1] = 'a', out[1] = 'A';
			 c[1] <= 'z' && out[1] <= 'Z';
			 c[1]++, out[1]++)
		{
			output = str_to_numstr(c, 36, &processed);

			cr_assert(eq(sz, output->len, 2));
			cr_assert(eq(sz, processed, 2));
			cr_assert(eq(chr, output->is_negative, false));
			cr_assert(eq(str, output->str, out));
			output = free_numstr(output);
		}
	}
}

Test(valid_inputs, test_0000000000000,
	 .description = "0000000000000", .timeout = 1.0)
{
	const char in[] = "0000000000000";

	output = str_to_numstr(in, 10, &processed);

	cr_assert(eq(sz, output->len, 1));
	cr_assert(eq(sz, processed, sizeof(in) - 1));
	cr_assert(eq(chr, output->is_negative, false));
	cr_assert(eq(str, output->str, "0"));
}

Test(valid_inputs, test_neg0000000000000,
	 .description = "-0000000000000", .timeout = 1.0)
{
	const char in[] = "-0000000000000";

	output = str_to_numstr(in, 10, &processed);

	cr_assert(eq(sz, output->len, 1));
	cr_assert(eq(sz, processed, sizeof(in) - 1));
	cr_assert(eq(chr, output->is_negative, true));
	cr_assert(eq(str, output->str, "0"));
}

Test(valid_inputs, test_00000000000001,
	 .description = "00000000000001", .timeout = 1.0)
{
	const char in[] = "00000000000001";

	output = str_to_numstr(in, 10, &processed);

	cr_assert(eq(sz, output->len, 1));
	cr_assert(eq(sz, processed, sizeof(in) - 1));
	cr_assert(eq(chr, output->is_negative, false));
	cr_assert(eq(str, output->str, "1"));
}

Test(valid_inputs, test_neg00000000000001,
	 .description = "-00000000000001", .timeout = 1.0)
{
	const char in[] = "-00000000000001";

	output = str_to_numstr(in, 10, &processed);

	cr_assert(eq(sz, output->len, 1));
	cr_assert(eq(sz, processed, sizeof(in) - 1));
	cr_assert(eq(chr, output->is_negative, true));
	cr_assert(eq(str, output->str, "1"));
}

Test(valid_inputs, test_mmneg10_000_000_000,
	 .description = "-10_000_000_000", .timeout = 1.0)
{
	const char in[] = "---10_000_000_000";

	output = str_to_numstr(in, 10, &processed);

	cr_assert(eq(sz, output->len, 11));
	cr_assert(eq(sz, processed, sizeof(in) - 1));
	cr_assert(eq(chr, output->is_negative, true));
	cr_assert(eq(str, output->str, "10000000000"));
}

Test(valid_inputs, test_neg1, .description = "--ZZZ_4Ar_YU8_012_qa9", .timeout = 1.0)
{
	const char in[] = "--ZZZ_4Ar_YU8_012_qa9";

	output = str_to_numstr(in, 36, &processed);

	cr_assert(eq(sz, output->len, 15));
	cr_assert(eq(sz, processed, sizeof(in) - 1));
	cr_assert(eq(chr, output->is_negative, false));
	cr_assert(eq(str, output->str, "ZZZ4ARYU8012QA9"));
}
