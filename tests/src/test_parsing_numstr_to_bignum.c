#include "tests.h"

bignum *output = NULL;

/**
 * setup - setup some variables.
 */
void setup(void)
{
	set_base(10);
}

/**
 * teardown - cleanup after tests.
 */
void teardown(void)
{
	output = free_bignum(output);
}

TestSuite(null_inputs);

Test(null_inputs, test_NULL, .description = "NULL", .timeout = 3.0)
{
	cr_assert(zero(ptr, numstr_to_bignum(NULL)));
}

Test(null_inputs, test_null_str, .description = "null string", .timeout = 3.0)
{
	numstr in = {.len = 1, .is_negative = false, .str = NULL};

	cr_assert(zero(ptr, numstr_to_bignum(&in)));
}

TestSuite(invalid_inputs);

Test(invalid_inputs, test_0_len, .description = "len is 0", .timeout = 3.0)
{
	numstr in = {.len = 0, .is_negative = false, .str = "1"};

	cr_assert(zero(ptr, numstr_to_bignum(&in)));
}

Test(invalid_inputs, test_not_alnum,
	 .description = "First character is not alphanumeric", .timeout = 3.0)
{
	numstr in = {.len = 4, .is_negative = false, .str = "-24"};

	cr_assert(zero(ptr, numstr_to_bignum(&in)));
}

Test(invalid_inputs, test_all_nonvalid_charbits,
	 .description = "invalid chars", .timeout = 3.0)
{
	char c[2] = {SCHAR_MIN, '\0'};
	numstr in = {.len = 4, .is_negative = false, .str = c};

	set_base(36);
	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX; c[0]++)
	{
		if (!isalnum(c[0]))
			cr_assert(zero(ptr, numstr_to_bignum(&in)));
	}
}

Test(invalid_inputs, test_all_nonvalid_charbits2,
	 .description = "invalid 2chars", .timeout = 3.0)
{
	char c[3] = {SCHAR_MIN, SCHAR_MIN, '\0'};
	numstr in = {.len = 4, .is_negative = false, .str = c};

	set_base(36);
	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX; c[0]++, c[1]++)
	{
		if (!isalnum(c[0]))
			cr_assert(zero(ptr, numstr_to_bignum(&in)));
	}
}

Test(invalid_inputs, test_all_nonvalid_charbits2alt,
	 .description = "invalid 2chars alt", .timeout = 3.0)
{
	char c[3] = {SCHAR_MIN, SCHAR_MAX, '\0'};
	numstr in = {.len = 4, .is_negative = false, .str = c};

	set_base(36);
	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX && c[1] > SCHAR_MIN; c[0]++, c[1]--)
	{
		if (!isalnum(c[0]) || !isalnum(c[1]))
			cr_assert(zero(ptr, numstr_to_bignum(&in)));
	}
}

Test(invalid_inputs, test_all_invalid_charbits3,
	 .description = "invalid 3chars", .timeout = 3.0)
{
	char c[] = {'\0', '\0', '\0', '\0'};
	numstr in = {.len = 4, .is_negative = false, .str = c};

	set_base(36);
	cr_redirect_stderr();
	c[0] = '+';
	for (c[1] = '0'; c[1] <= '9'; c[1]++)
	{
		for (c[2] = SCHAR_MIN; c[2] < SCHAR_MAX; c[2]++)
		{
			if (!isalnum(c[2]) && c[2] != '\0' && c[2] != '.')
				cr_assert(zero(ptr, numstr_to_bignum(&in)));
		}
	}

	c[0] = '-';
	for (c[1] = '0'; c[1] <= '9'; c[1]++)
	{
		for (c[2] = SCHAR_MIN; c[2] < SCHAR_MAX; c[2]++)
		{
			if (!isalnum(c[2]) && c[2] != '\0' && c[2] != '.')
				cr_assert(zero(ptr, numstr_to_bignum(&in)));
		}
	}
}

Test(invalid_inputs, test_invalid_chars_mixed_with_valid_chars,
	 .description = "Assortment of valid and invalid chars", .timeout = 3.0)
{
	numstr in = {.len = 4, .is_negative = false, .str = "123-567"};

	cr_assert(zero(ptr, numstr_to_bignum(&in)));
	in.str = "123 567";
	cr_redirect_stderr();
	cr_assert(zero(ptr, numstr_to_bignum(&in)));
	in.str = "123\"567\"";
	cr_assert(zero(ptr, numstr_to_bignum(&in)));
	in.str = "123+567";
	cr_assert(zero(ptr, numstr_to_bignum(&in)));
	in.str = "123/567";
	cr_assert(zero(ptr, numstr_to_bignum(&in)));
	in.str = "123*567";
	cr_assert(zero(ptr, numstr_to_bignum(&in)));
	in.str = "123^567";
	cr_assert(zero(ptr, numstr_to_bignum(&in)));
	in.str = "123(567)";
	cr_assert(zero(ptr, numstr_to_bignum(&in)));
	in.str = "123>567";
	cr_assert(zero(ptr, numstr_to_bignum(&in)));
}
