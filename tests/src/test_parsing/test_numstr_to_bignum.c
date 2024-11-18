#include "tests.h"

bignum *output = NULL;

/**
 * setup - setup some variables.
 */
void setup(void) {}

/**
 * teardown - cleanup after tests.
 */
void teardown(void)
{
	output = bn_free(output);
}

TestSuite(null_inputs);

Test(null_inputs, test_NULL, .description = "NULL", .timeout = 2.0)
{
	cr_assert(zero(ptr, numstr_to_bignum(NULL)));
}

Test(null_inputs, test_null_str, .description = "null string", .timeout = 2.0)
{
	numstr in = {.len = 1, .is_negative = false, .str = NULL};

	cr_assert(zero(ptr, numstr_to_bignum(&in)));
}

TestSuite(invalid_inputs);

Test(invalid_inputs, test_0_len, .description = "len is 0", .timeout = 2.0)
{
	numstr in = {.len = 0, .is_negative = false, .str = "1"};

	cr_assert(zero(ptr, numstr_to_bignum(&in)));
}

Test(invalid_inputs, test_all_nonvalid_charbits,
	 .description = "invalid chars", .timeout = 2.0)
{
	char c[2] = {SCHAR_MIN, '\0'};
	numstr in = {.len = 4, .is_negative = false, .str = c};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX; c[0]++)
	{
		if (!isalnum(c[0]))
			cr_assert(zero(ptr, numstr_to_bignum(&in)));
	}
}

Test(invalid_inputs, test_all_nonvalid_charbits2,
	 .description = "invalid 2chars", .timeout = 2.0)
{
	char c[3] = {SCHAR_MIN, SCHAR_MIN, '\0'};
	numstr in = {.len = 4, .is_negative = false, .str = c};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX; c[0]++, c[1]++)
	{
		if (!isalnum(c[0]))
			cr_assert(zero(ptr, numstr_to_bignum(&in)));
	}
}

Test(invalid_inputs, test_all_nonvalid_charbits2alt,
	 .description = "invalid 2chars alt", .timeout = 2.0)
{
	char c[3] = {SCHAR_MIN, SCHAR_MAX, '\0'};
	numstr in = {.len = 4, .is_negative = false, .str = c};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX && c[1] > SCHAR_MIN; c[0]++, c[1]--)
	{
		if (!isalnum(c[0]) || !isalnum(c[1]))
			cr_assert(zero(ptr, numstr_to_bignum(&in)));
	}
}

Test(invalid_inputs, test_all_invalid_charbits3,
	 .description = "invalid 3chars", .timeout = 2.0)
{
	char c[] = {'\0', '\0', '\0', '\0'};
	numstr in = {.len = 4, .is_negative = false, .str = c};

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
	 .description = "Assortment of valid and invalid chars", .timeout = 2.0)
{
	numstr in = {.len = 7, .is_negative = false, .str = "123/567"};

	cr_assert(zero(ptr, numstr_to_bignum(&in)));
	in.str = "123 567";
	cr_redirect_stderr();
	cr_assert(zero(ptr, numstr_to_bignum(&in)));
	in.str = "123\"567\"";
	cr_assert(zero(ptr, numstr_to_bignum(&in)));
	in.str = "123+567";
	cr_assert(zero(ptr, numstr_to_bignum(&in)));
	in.str = "123-567";
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

TestSuite(valid_inputs, .init = setup, .fini = teardown);

Test(valid_inputs, test_0, .description = "0", .timeout = 2.0)
{
	numstr in = {.len = 1, .is_negative = false, .str = "0"};
	u_int arr[] = {0};
	bignum out = {
		.len = sizeof(arr) / sizeof(*arr), .is_negative = false, .num = arr};

	output = numstr_to_bignum(&in);
	cr_assert(eq(sz, output->len, out.len));
	cr_assert(eq(chr, output->is_negative, out.is_negative));
	cr_assert(eq(u32[out.len], output->num, out.num));
}

Test(valid_inputs, test_eight9s, .description = "99999999", .timeout = 2.0)
{
	numstr in = {.len = 8, .is_negative = false, .str = "99999999"};
	u_int arr[] = {99999999};
	bignum out = {
		.len = sizeof(arr) / sizeof(*arr), .is_negative = false, .num = arr};

	output = numstr_to_bignum(&in);
	cr_assert(eq(sz, output->len, out.len));
	cr_assert(eq(chr, output->is_negative, out.is_negative));
	cr_assert(eq(u32[out.len], output->num, out.num));
}

Test(valid_inputs, test_nine9s, .description = "999999999", .timeout = 2.0)
{
	numstr in = {.len = 9, .is_negative = false, .str = "999999999"};
	u_int arr[] = {999999999};
	bignum out = {
		.len = sizeof(arr) / sizeof(*arr), .is_negative = false, .num = arr};

	output = numstr_to_bignum(&in);
	cr_assert(eq(sz, output->len, out.len));
	cr_assert(eq(chr, output->is_negative, out.is_negative));
	cr_assert(eq(u32[out.len], output->num, out.num));
}

Test(valid_inputs, test_ten9s, .description = "9,999999999", .timeout = 2.0)
{
	numstr in = {.len = 10, .is_negative = false, .str = "9999999999"};
	u_int arr[] = {999999999, 9};
	bignum out = {
		.len = sizeof(arr) / sizeof(*arr), .is_negative = false, .num = arr};

	output = numstr_to_bignum(&in);
	cr_assert(eq(sz, output->len, out.len));
	cr_assert(eq(chr, output->is_negative, out.is_negative));
	cr_assert(eq(u32[out.len], output->num, out.num));
}
