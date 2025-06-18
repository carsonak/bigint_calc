#include "test_math.h"

/* ################################################################### */
/* ########################## invalid_inputs ######################### */
/* ################################################################### */

struct invalid_inputs
{
	bigint num1, expected, *output;
};

TEST_F_SETUP(invalid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(invalid_inputs) { tau->output = bi_delete(tau->output); }

TEST(invalid_inputs, test_null_shift_r)
{
	CHECK_PTR_EQ(bi_shift_r(NULL, 1), NULL);
	CHECK_PTR_EQ(bi_shift_r(NULL, 0), NULL);
	CHECK_PTR_EQ(bi_shift_r(NULL, -1), NULL);
}

TEST_F(invalid_inputs, test_NaN_shift_r_neg1)
{
	CHECK_PTR_EQ(bi_shift_r(&(tau->num1), -1), NULL);
}

TEST_F(invalid_inputs, test_NaN_shift_r_0)
{
	tau->output = bi_shift_r(&(tau->num1), 0);
	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_NaN_shift_r_1)
{
	tau->output = bi_shift_r(&(tau->num1), 1);
	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_shift_r_neg1)
{
	u_int in1[1] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK_PTR_EQ(bi_shift_r(&(tau->num1), -1), NULL);
}

/* ################################################################### */
/* ########################## simple_shifts ########################## */
/* ################################################################### */

struct simple_shifts
{
	bigint *restrict num1, *restrict output;
	char *restrict outstr;
};

TEST_F_SETUP(simple_shifts) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(simple_shifts)
{
	bi_delete(tau->num1);
	bi_delete(tau->output);
	xfree(tau->outstr);
}

TEST_F(simple_shifts, test_0_shift_r_0)
{
	const char expected[] = "0";

	tau->num1 = bi_new("0", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, 0);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_3_shift_r_0)
{
	const char expected[] = "3";

	tau->num1 = bi_new("3", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, 0);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_0_shift_r_3)
{
	const char expected[] = "0";

	tau->num1 = bi_new("0", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, 3);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_0_shift_r_longmaxi)
{
	const char expected[] = "0";

	tau->num1 = bi_new("0", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, LLONG_MAX);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_0_shift_r_half_longmaxi)
{
	const char expected[] = "0";

	tau->num1 = bi_new("0", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, LLONG_MAX / 2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_3_shift_r_1)
{
	const char expected[] = "0";

	tau->num1 = bi_new("3", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, 1);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_3_shift_r_5)
{
	const char expected[] = "0";

	tau->num1 = bi_new("3", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, 5);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_3_shift_r_longmaxi)
{
	const char expected[] = "0";

	tau->num1 = bi_new("3", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, LLONG_MAX);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_3_shift_r_half_longmaxi)
{
	const char expected[] = "0";

	tau->num1 = bi_new("3", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, LLONG_MAX / 2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_large_shift_r_1)
{
	/* clang-format off */
	const char expected[] = "9000000008000000007000000006000000005000000004000000003000000002";

	tau->num1 = bi_new("9_000000008_000000007_000000006_000000005_000000004_000000003_000000002_000000001", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, 1);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_large_shift_r_5)
{
	/* clang-format off */
	const char expected[] = "9000000008000000007000000006";

	tau->num1 = bi_new("9_000000008_000000007_000000006_000000005_000000004_000000003_000000002_000000001", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, 5);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_large_shift_r_longmaxi)
{
	/* clang-format off */
	const char expected[] = "0";

	tau->num1 = bi_new("9_000000008_000000007_000000006_000000005_000000004_000000003_000000002_000000001", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, LLONG_MAX);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_large_shift_r_half_longmaxi)
{
	/* clang-format off */
	const char expected[] = "0";

	tau->num1 = bi_new("9_000000008_000000007_000000006_000000005_000000004_000000003_000000002_000000001", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, LLONG_MAX / 2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

/* ################################################################### */
/* ####################### negative_num_shifts ####################### */
/* ################################################################### */

struct negative_num_shifts
{
	bigint *restrict num1, *restrict output;
	char *restrict outstr;
};

TEST_F_SETUP(negative_num_shifts) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(negative_num_shifts)
{
	bi_delete(tau->num1);
	bi_delete(tau->output);
	xfree(tau->outstr);
}

TEST_F(negative_num_shifts, test_neglarge_shift_r_5)
{
	/* clang-format off */
	const char expected[] = "-9000000008000000007000000006";

	tau->num1 = bi_new("-9_000000008_000000007_000000006_000000005_000000004_000000003_000000002_000000001", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, 5);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(negative_num_shifts, test_neg86234_shift_r_1)
{
	const char expected[] = "0";

	tau->num1 = bi_new("-86234", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_r(tau->num1, 1);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

/* ################################################################### */
/* ########################### bi_shift_l ############################ */
/* ################################################################### */

/********************* invalid_inputs *********************/

TEST(invalid_inputs, test_null_shift_l)
{
	CHECK_PTR_EQ(bi_shift_l(NULL, 1), NULL);
	CHECK_PTR_EQ(bi_shift_l(NULL, 0), NULL);
	CHECK_PTR_EQ(bi_shift_l(NULL, -1), NULL);
}

TEST_F(invalid_inputs, test_NaN_shift_l_neg1)
{
	CHECK_PTR_EQ(bi_shift_l(&(tau->num1), -1), NULL);
}

TEST_F(invalid_inputs, test_NaN_shift_l_0)
{
	tau->output = bi_shift_l(&(tau->num1), 0);
	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_NaN_shift_l_1)
{
	tau->output = bi_shift_l(&(tau->num1), 1);
	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_shift_l_neg1)
{
	u_int in1[1] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK_PTR_EQ(bi_shift_l(&(tau->num1), -1), NULL);
}

/********************* simple_shifts *********************/

TEST_F(simple_shifts, test_0_shift_l_0)
{
	const char expected[] = "0";

	tau->num1 = bi_new("0", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_l(tau->num1, 0);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_3_shift_l_0)
{
	const char expected[] = "3";

	tau->num1 = bi_new("3", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_l(tau->num1, 0);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_0_shift_l_3)
{
	const char expected[] = "0";

	tau->num1 = bi_new("0", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_l(tau->num1, 3);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_0_shift_l_longmaxi)
{
	const char expected[] = "0";

	tau->num1 = bi_new("0", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_l(tau->num1, LLONG_MAX);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_0_shift_l_half_longmaxi)
{
	const char expected[] = "0";

	tau->num1 = bi_new("0", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_l(tau->num1, LLONG_MAX / 2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_3_shift_l_1)
{
	const char expected[] = "3000000000";

	tau->num1 = bi_new("3", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_l(tau->num1, 1);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_3_shift_l_5)
{
	const char expected[] = "3000000000000000000000000000000000000000000000";

	tau->num1 = bi_new("3", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_l(tau->num1, 5);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_3_shift_l_longmaxi)
{
	tau->num1 = bi_new("3", 10, NULL);
	CHECK_PTR_EQ(bi_shift_l(tau->num1, LLONG_MAX), NULL);
}

TEST_F(simple_shifts, test_3_shift_l_half_longmaxi)
{
	tau->num1 = bi_new("3", 10, NULL);
	CHECK_PTR_EQ(bi_shift_l(tau->num1, LLONG_MAX / 2), NULL);
}

TEST_F(simple_shifts, test_large_shift_l_1)
{
	/* clang-format off */
	const char expected[] = "9000000008000000007000000006000000005000000004000000003000000002000000001000000000";

	tau->num1 = bi_new("9_000000008_000000007_000000006_000000005_000000004_000000003_000000002_000000001", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_l(tau->num1, 1);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_large_shift_l_5)
{
	/* clang-format off */
	const char expected[] = "9000000008000000007000000006000000005000000004000000003000000002000000001000000000000000000000000000000000000000000000";

	tau->num1 = bi_new("9_000000008_000000007_000000006_000000005_000000004_000000003_000000002_000000001", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_l(tau->num1, 5);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_shifts, test_large_shift_l_longmaxi)
{
	/* clang-format off */
	tau->num1 = bi_new("9_000000008_000000007_000000006_000000005_000000004_000000003_000000002_000000001", 10, NULL);
	/* clang-format on */
	CHECK_PTR_EQ(bi_shift_l(tau->num1, LLONG_MAX), NULL);
}

TEST_F(simple_shifts, test_large_shift_l_half_longmaxi)
{
	/* clang-format off */
	tau->num1 = bi_new("9_000000008_000000007_000000006_000000005_000000004_000000003_000000002_000000001", 10, NULL);
	/* clang-format on */
	CHECK_PTR_EQ(bi_shift_l(tau->num1, LLONG_MAX / 2), NULL);
}

/************************ negative_num_shifts ************************/

TEST_F(negative_num_shifts, test_neglarge_shift_l_5)
{
	/* clang-format off */
	const char expected[] = "-9000000008000000007000000006000000005000000004000000003000000002000000001000000000000000000000000000000000000000000000";

	tau->num1 = bi_new("-9_000000008_000000007_000000006_000000005_000000004_000000003_000000002_000000001", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_l(tau->num1, 5);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(negative_num_shifts, test_neg86234_shift_l_1)
{
	/* clang-format off */
	const char expected[] = "-86234000000000";
	/* clang-format on */

	tau->num1 = bi_new("-86234", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_shift_l(tau->num1, 1);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);
	CHECK_STREQ(tau->outstr, expected);
}
