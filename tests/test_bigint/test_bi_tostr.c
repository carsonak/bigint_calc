#include "_bi_internals.h"
#include "test_bigint.h"

/* ################################################################### */
/* ########################## invalid_inputs ######################### */
/* ################################################################### */

struct invalid_inputs
{
	bigint num1;
};

TEST_F_SETUP(invalid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(invalid_inputs) { (void)tau; }

TEST(invalid_inputs, test_null) { CHECK_PTR_EQ(bi_tostr(NULL), NULL); }

TEST_F(invalid_inputs, test_NaN)
{
	CHECK_PTR_EQ(bi_tostr(&(tau->num1)), NULL);
}

TEST_F(invalid_inputs, test_len_0)
{
	u_int arr[1] = {0};
	tau->num1.num = arr;
	CHECK_PTR_EQ(bi_tostr(&(tau->num1)), NULL);
}

TEST_F(invalid_inputs, test_len_neg1)
{
	tau->num1.len = -1;
	CHECK_PTR_EQ(bi_tostr(&(tau->num1)), NULL);

	u_int arr[1] = {0};
	tau->num1.num = arr;
	CHECK_PTR_EQ(bi_tostr(&(tau->num1)), NULL);
}

/* ################################################################### */
/* ########################### valid_inputs ########################## */
/* ################################################################### */

struct valid_inputs
{
	bigint num1;
	char *restrict output;
};

TEST_F_SETUP(valid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(valid_inputs) { xfree(tau->output); }

TEST_F(valid_inputs, test_0)
{
	const char expected[] = "0";
	tau->num1 = (bigint){.len = 1, .num = (u_int[1]){0}};

	tau->output = bi_tostr(&(tau->num1));

	CHECK_STREQ(tau->output, expected);
}

TEST_F(valid_inputs, test_long0s)
{
	const char expected[] = "0";
	tau->num1 = (bigint){.len = 5, .num = (u_int[5]){0}};

	tau->output = bi_tostr(&(tau->num1));

	CHECK_STREQ(tau->output, expected);
}

TEST_F(valid_inputs, test_longnum)
{
	/* clang-format off */
	const char expected[] = "1000000002000000003000000004000000005000000006000000007000000008000000009000000000";
	tau->num1 = (bigint){.len = 10, .num = (u_int[10]){0, 9, 8, 7, 6, 5, 4, 3, 2, 1}};
	/* clang-format on */

	tau->output = bi_tostr(&(tau->num1));

	CHECK_STREQ(tau->output, expected);
}

TEST_F(valid_inputs, test_longnum_with_leading_zero)
{
	/* clang-format off */
	const char expected[] = "1000000002000000003000000004000000005000000006000000007000000008000000009000000000";
	tau->num1 = (bigint){.len = 11, .num = (u_int[11]){0, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}};
	/* clang-format on */

	tau->output = bi_tostr(&(tau->num1));

	CHECK_STREQ(tau->output, expected);
}

TEST_F(valid_inputs, test_longnum_with_leading_zeros)
{
	/* clang-format off */
	const char expected[] = "1000000002000000003000000004000000005000000006000000007000000008000000009000000000";
	tau->num1 = (bigint){.len = 13, .num = (u_int[13]){0, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0}};
	/* clang-format on */

	tau->output = bi_tostr(&(tau->num1));

	CHECK_STREQ(tau->output, expected);
}

/* ################################################################### */
/* ########################### negative_numbers ########################## */
/* ################################################################### */

struct negative_numbers
{
	bigint num1;
	char *restrict output;
};

TEST_F_SETUP(negative_numbers)
{
	memset(tau, 0, sizeof(*tau));
	tau->num1.is_negative = true;
}

TEST_F_TEARDOWN(negative_numbers) { xfree(tau->output); }

TEST_F(negative_numbers, test_neg0)
{
	const char expected[] = "0";
	tau->num1.len = 1;
	tau->num1.num = (u_int[1]){0};

	tau->output = bi_tostr(&(tau->num1));

	CHECK_STREQ(tau->output, expected);
}

TEST_F(negative_numbers, test_neg10)
{
	const char expected[] = "-10";
	tau->num1.len = 1;
	tau->num1.num = (u_int[1]){10};

	tau->output = bi_tostr(&(tau->num1));

	CHECK_STREQ(tau->output, expected);
}

TEST_F(negative_numbers, test_longnum)
{
	/* clang-format off */
	const char expected[] = "-1000000002000000003000000004000000005000000006000000007000000008000000009000000000";
	/* clang-format on */
	tau->num1.len = 10;
	tau->num1.num = (u_int[10]){0, 9, 8, 7, 6, 5, 4, 3, 2, 1};

	tau->output = bi_tostr(&(tau->num1));

	CHECK_STREQ(tau->output, expected);
}

TEST_F(negative_numbers, test_longnum_with_leading_zero)
{
	/* clang-format off */
	const char expected[] = "-1000000002000000003000000004000000005000000006000000007000000008000000009000000000";
	/* clang-format on */
	tau->num1.len = 11;
	tau->num1.num = (u_int[11]){0, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

	tau->output = bi_tostr(&(tau->num1));

	CHECK_STREQ(tau->output, expected);
}

TEST_F(negative_numbers, test_longnum_with_leading_zeros)
{
	/* clang-format off */
	const char expected[] = "-1000000002000000003000000004000000005000000006000000007000000008000000009000000000";
	/* clang-format on */
	tau->num1.len = 13;
	tau->num1.num = (u_int[13]){0, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0};

	tau->output = bi_tostr(&(tau->num1));

	CHECK_STREQ(tau->output, expected);
}
