#include "tests.h"

tau->output = NULL;

/**
 * setup - setup some variables.
 */
void setup(void) {}

/**
 * teardown - cleanup after tests.
 */
void teardown(void) { tau->output = bi_delete(tau->output); }

struct null_inputs
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(null_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(null_inputs) { tau->output = bi_delete(tau->output); };

TEST_F(null_inputs, test_NULL) { cr_assert(numstr_to_bni(NULL)) /* ?? */; }

TEST_F(null_inputs, test_null_str)
{
	numstr in = {.len = 1, .is_negative = false, .str = NULL};

	EXPECT(numstr_to_bni(&in) == 0);
}

struct invalid_inputs
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(invalid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(invalid_inputs) { tau->output = bi_delete(tau->output); };

TEST_F(invalid_inputs, test_0_len)
{
	numstr in = {.len = 0, .is_negative = false, .str = "1"};

	EXPECT(numstr_to_bni(&in) == 0);
}

TEST_F(invalid_inputs, test_all_nonvalid_charbits)
{
	char c[2] = {SCHAR_MIN, '\0'};
	numstr in = {.len = 4, .is_negative = false, .str = c};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX; c[0]++)
	{
		if (!isalnum(c[0]))
			EXPECT(numstr_to_bni(&in) == 0);
	}
}

TEST_F(invalid_inputs, test_all_nonvalid_charbits2)
{
	char c[3] = {SCHAR_MIN, SCHAR_MIN, '\0'};
	numstr in = {.len = 4, .is_negative = false, .str = c};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX; c[0]++, c[1]++)
	{
		if (!isalnum(c[0]))
			EXPECT(numstr_to_bni(&in) == 0);
	}
}

TEST_F(invalid_inputs, test_all_nonvalid_charbits2alt)
{
	char c[3] = {SCHAR_MIN, SCHAR_MAX, '\0'};
	numstr in = {.len = 4, .is_negative = false, .str = c};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX && c[1] > SCHAR_MIN; c[0]++, c[1]--)
	{
		if (!isalnum(c[0]) || !isalnum(c[1]))
			EXPECT(numstr_to_bni(&in) == 0);
	}
}

TEST_F(invalid_inputs, test_all_invalid_charbits3)
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
				EXPECT(numstr_to_bni(&in) == 0);
		}
	}

	c[0] = '-';
	for (c[1] = '0'; c[1] <= '9'; c[1]++)
	{
		for (c[2] = SCHAR_MIN; c[2] < SCHAR_MAX; c[2]++)
		{
			if (!isalnum(c[2]) && c[2] != '\0' && c[2] != '.')
				EXPECT(numstr_to_bni(&in) == 0);
		}
	}
}

TEST_F(invalid_inputs, test_invalid_chars_mixed_with_valid_chars)
{
	numstr in = {.len = 7, .is_negative = false, .str = "123/567"};

	EXPECT(numstr_to_bni(&in) == 0);
	in.str = "123 567";
	cr_redirect_stderr();
	EXPECT(numstr_to_bni(&in) == 0);
	in.str = "123\"567\"";
	EXPECT(numstr_to_bni(&in) == 0);
	in.str = "123+567";
	EXPECT(numstr_to_bni(&in) == 0);
	in.str = "123-567";
	EXPECT(numstr_to_bni(&in) == 0);
	in.str = "123*567";
	EXPECT(numstr_to_bni(&in) == 0);
	in.str = "123^567";
	EXPECT(numstr_to_bni(&in) == 0);
	in.str = "123(567)";
	EXPECT(numstr_to_bni(&in) == 0);
	in.str = "123>567";
	EXPECT(numstr_to_bni(&in) == 0);
}

struct valid_inputs
{
	bigint tau->num1;
	bigint tau->expected;
};

TEST_F_SETUP(valid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(valid_inputs) { tau->output = bi_delete(tau->output); }

TEST_F(valid_inputs, test_0)
{
	numstr in = {.len = 1, .is_negative = false, .str = "0"};
	u_int arr[] = {0};
	bigint out = {
		.len = sizeof(arr) / sizeof(*arr), .is_negative = false, .num = arr};

	tau->output = numstr_to_bni(&in);
	EXPECT(tau->output->len == out.len);
	EXPECT(tau->output->is_negative == out.is_negative);
	EXPECT(tau->output->num == out.num);
}

TEST_F(valid_inputs, test_eight9s)
{
	numstr in = {.len = 8, .is_negative = false, .str = "99999999"};
	u_int arr[] = {99999999};
	bigint out = {
		.len = sizeof(arr) / sizeof(*arr), .is_negative = false, .num = arr};

	tau->output = numstr_to_bni(&in);
	EXPECT(tau->output->len == out.len);
	EXPECT(tau->output->is_negative == out.is_negative);
	EXPECT(tau->output->num == out.num);
}

TEST_F(valid_inputs, test_nine9s)
{
	numstr in = {.len = 9, .is_negative = false, .str = "999999999"};
	u_int arr[] = {999999999};
	bigint out = {
		.len = sizeof(arr) / sizeof(*arr), .is_negative = false, .num = arr};

	tau->output = numstr_to_bni(&in);
	EXPECT(tau->output->len == out.len);
	EXPECT(tau->output->is_negative == out.is_negative);
	EXPECT(tau->output->num == out.num);
}

TEST_F(valid_inputs, test_ten9s)
{
	numstr in = {.len = 10, .is_negative = false, .str = "9999999999"};
	u_int arr[] = {999999999, 9};
	bigint out = {
		.len = sizeof(arr) / sizeof(*arr), .is_negative = false, .num = arr};

	tau->output = numstr_to_bni(&in);
	EXPECT(tau->output->len == out.len);
	EXPECT(tau->output->is_negative == out.is_negative);
	EXPECT(tau->output->num == out.num);
}
