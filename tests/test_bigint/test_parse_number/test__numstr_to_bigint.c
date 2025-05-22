#include "tests.h"

TEST(invalid_inputs, test_NULL) { CHECK_PTR_EQ(_numstr_to_bi(NULL), NULL); }

TEST(invalid_inputs, test_null_str)
{
	numstr input = {.len = 1, .str = NULL};

	CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
}

TEST(invalid_inputs, test_0_len)
{
	numstr input = {.len = 0, .str = "1"};

	CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
}

TEST(invalid_inputs, test_all_nonvalid_charbits)
{
	char s[2] = {SCHAR_MIN, '\0'};
	numstr input = {.len = 4, .str = s};

	/* Error messages should be ignored. */
	for (; s[0] < SCHAR_MAX; s[0]++)
	{
		if (!isalnum(s[0]))
			CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
	}
}

TEST(invalid_inputs, test_all_nonvalid_charbits2)
{
	char s[3] = {SCHAR_MIN, SCHAR_MIN, '\0'};
	numstr input = {.len = 4, .str = s};

	/* Error messages should be ignored. */
	for (; s[0] < SCHAR_MAX; s[0]++, s[1]++)
	{
		if (!isalnum(s[0]))
			CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
	}
}

TEST(invalid_inputs, test_all_nonvalid_charbits2alt)
{
	char s[3] = {SCHAR_MIN, SCHAR_MAX, '\0'};
	numstr input = {.len = 4, .str = s};

	/* Error messages should be ignored. */
	for (; s[0] < SCHAR_MAX && s[1] > SCHAR_MIN; s[0]++, s[1]--)
	{
		if (!isalnum(s[0]) || !isalnum(s[1]))
			CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
	}
}

TEST(invalid_inputs, test_all_invalid_charbits3)
{
	char s[] = {'\0', '\0', '\0', '\0'};
	numstr input = {.len = 4, .str = s};

	/* Error messages should be ignored. */
	s[0] = '+';
	for (s[1] = '0'; s[1] <= '9'; s[1]++)
	{
		for (s[2] = SCHAR_MIN; s[2] < SCHAR_MAX; s[2]++)
		{
			if (!isalnum(s[2]) && s[2] != '\0' && s[2] != '.')
				CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
		}
	}

	s[0] = '-';
	for (s[1] = '0'; s[1] <= '9'; s[1]++)
	{
		for (s[2] = SCHAR_MIN; s[2] < SCHAR_MAX; s[2]++)
		{
			if (!isalnum(s[2]) && s[2] != '\0' && s[2] != '.')
				CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
		}
	}
}

TEST(invalid_inputs, test_invalid_chars_mixed_with_valid_chars)
{
	numstr input = {.len = 7, .str = "123/567"};

	CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
	input.str = "123 567";
	/* Error messages should be ignored. */
	CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
	input.str = "123\"567\"";
	CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
	input.str = "123+567";
	CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
	input.str = "123-567";
	CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
	input.str = "123*567";
	CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
	input.str = "123^567";
	CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
	input.str = "123(567)";
	CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
	input.str = "123>567";
	CHECK_PTR_EQ(_numstr_to_bi(&input), NULL);
}

struct valid_inputs
{
	bigint *output;
};

TEST_F_SETUP(valid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(valid_inputs) { tau->output = bi_delete(tau->output); }

TEST_F(valid_inputs, test_0)
{
	numstr input = {.len = 1, .str = "0"};
	u_int arr[] = {0};
	bigint expected = {.len = sizeof(arr) / sizeof(*arr), .num = arr};

	tau->output = _numstr_to_bi(&input);
	CHECK(tau->output->len == expected.len);
	CHECK(tau->output->is_negative == expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, expected.num, sizeof(*expected.num) * expected.len
	);
}

TEST_F(valid_inputs, test_eight9s)
{
	numstr input = {.len = 8, .str = "99999999"};
	u_int arr[] = {99999999};
	bigint expected = {.len = sizeof(arr) / sizeof(*arr), .num = arr};

	tau->output = _numstr_to_bi(&input);
	CHECK(tau->output->len == expected.len);
	CHECK(tau->output->is_negative == expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, expected.num, sizeof(*expected.num) * expected.len
	);
}

TEST_F(valid_inputs, test_nine9s)
{
	numstr input = {.len = 9, .str = "999999999"};
	u_int arr[] = {999999999};
	bigint expected = {.len = sizeof(arr) / sizeof(*arr), .num = arr};

	tau->output = _numstr_to_bi(&input);
	CHECK(tau->output->len == expected.len);
	CHECK(tau->output->is_negative == expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, expected.num, sizeof(*expected.num) * expected.len
	);
}

TEST_F(valid_inputs, test_ten9s)
{
	numstr input = {.len = 10, .str = "9999999999"};
	u_int arr[] = {999999999, 9};
	bigint expected = {.len = sizeof(arr) / sizeof(*arr), .num = arr};

	tau->output = _numstr_to_bi(&input);
	CHECK(tau->output->len == expected.len);
	CHECK(tau->output->is_negative == expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, expected.num, sizeof(*expected.num) * expected.len
	);
}

TEST_F(valid_inputs, test_1234567890)
{
	char in_s[] = "1234567890";
	numstr input = {.len = sizeof(in_s) - 1, .str = in_s};
	u_int arr[] = {234567890, 1};
	bigint expected = {.len = sizeof(arr) / sizeof(*arr), .num = arr};

	tau->output = _numstr_to_bi(&input);
	CHECK(tau->output->len == expected.len);
	CHECK(tau->output->is_negative == expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, expected.num, sizeof(*expected.num) * expected.len
	);
}

TEST_F(valid_inputs, test_9876543210)
{
	char in_s[] = "9876543210";
	numstr input = {.len = sizeof(in_s) - 1, .str = in_s};
	u_int arr[] = {876543210, 9};
	bigint expected = {.len = sizeof(arr) / sizeof(*arr), .num = arr};

	tau->output = _numstr_to_bi(&input);
	CHECK(tau->output->len == expected.len);
	CHECK(tau->output->is_negative == expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, expected.num, sizeof(*expected.num) * expected.len
	);
}

struct long_nums
{
	bigint *output;
};

TEST_F_SETUP(long_nums) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(long_nums) { tau->output = bi_delete(tau->output); }

TEST_F(long_nums, test_3digits)
{
	char in_s[] = "12345678909876543210";
	numstr input = {.len = sizeof(in_s) - 1, .str = in_s};
	u_int arr[] = {876543210, 345678909, 12};
	bigint expected = {.len = sizeof(arr) / sizeof(*arr), .num = arr};

	tau->output = _numstr_to_bi(&input);
	CHECK(tau->output->len == expected.len);
	CHECK(tau->output->is_negative == expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, expected.num, sizeof(*expected.num) * expected.len
	);
}

TEST_F(long_nums, test_5digits_neg)
{
	char in_s[] = "4857987000000000000000012345678909876543210";
	numstr input = {.len = sizeof(in_s) - 1, .is_negative = true, .str = in_s};
	u_int arr[] = {876543210, 345678909, 12, 0, 4857987};
	bigint expected = {
		.len = sizeof(arr) / sizeof(*arr), .is_negative = true, .num = arr
	};

	tau->output = _numstr_to_bi(&input);
	CHECK(tau->output->len == expected.len);
	CHECK(tau->output->is_negative == expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, expected.num, sizeof(*expected.num) * expected.len
	);
}
