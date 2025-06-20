#include "test_math.h"

/* ################################################################### */
/* ###################### invalid_inputs ############################# */
/* ################################################################### */

struct invalid_inputs
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(invalid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(invalid_inputs) { tau->output = bi_delete(tau->output); }

TEST(invalid_inputs, test_null_minus_null)
{
	CHECK_PTR_EQ(bi_subtract(NULL, NULL), NULL);
}

TEST_F(invalid_inputs, test_1_minus_null)
{
	digit_ty in1[] = {1};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};

	CHECK_PTR_EQ(bi_subtract(&(tau->num1), NULL), NULL);
}

TEST_F(invalid_inputs, test_null_minus_1)
{
	digit_ty in2[] = {1};

	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK_PTR_EQ(bi_subtract(NULL, &(tau->num2)), NULL);
}

TEST_F(invalid_inputs, test_0_minus_null)
{
	digit_ty in1[1] = {0};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};

	CHECK_PTR_EQ(bi_subtract(&(tau->num1), NULL), NULL);
}

TEST_F(invalid_inputs, test_null_minus_0)
{
	digit_ty in2[1] = {0};

	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK_PTR_EQ(bi_subtract(NULL, &(tau->num2)), NULL);
}

TEST_F(invalid_inputs, test_neg1_minus_null)
{
	digit_ty in1[] = {1};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};

	CHECK_PTR_EQ(bi_subtract(&(tau->num1), NULL), NULL);
}

TEST_F(invalid_inputs, test_null_minus_neg1)
{
	digit_ty in2[] = {1};

	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->num2.is_negative = true;

	CHECK_PTR_EQ(bi_subtract(NULL, &(tau->num2)), NULL);
}

/* NaN */

TEST_F(invalid_inputs, test_NaN_minus_NaN)
{
	tau->output = bi_subtract(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == 0);
	CHECK(tau->output->is_negative == 0);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_neg_NaN_minus_NaN)
{
	tau->output = bi_subtract(&(tau->num1), &(tau->num2));

	tau->num1.is_negative = true;
	CHECK(tau->output->len == 0);
	CHECK(tau->output->is_negative == 0);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_NaN_minus_neg_NaN)
{
	tau->output = bi_subtract(&(tau->num1), &(tau->num2));

	tau->num2.is_negative = true;
	CHECK(tau->output->len == 0);
	CHECK(tau->output->is_negative == 0);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_neg_NaN_minus_neg_NaN)
{
	tau->output = bi_subtract(&(tau->num1), &(tau->num2));

	tau->num1.is_negative = true;
	tau->num2.is_negative = true;
	CHECK(tau->output->len == 0);
	CHECK(tau->output->is_negative == 0);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_4490998_minus_NaN)
{
	digit_ty in1[] = {4490998};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};
	tau->output = bi_subtract(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_neg_4490998_minus_NaN)
{
	digit_ty in1[] = {4490998};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};
	tau->output = bi_subtract(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_4490998_minus_neg_NaN)
{
	digit_ty in1[] = {4490998};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};
	tau->num2.is_negative = true;
	tau->output = bi_subtract(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_neg_4490998_minus_neg_NaN)
{
	digit_ty in1[] = {4490998};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};
	tau->num2.is_negative = true;
	tau->output = bi_subtract(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_NaN_minus_largenum)
{
	digit_ty in2[] = {0, 7, 6, 5, 4, 3, 2, 1};

	tau->num2 = (bigint){.len = sizeof(in2) / sizeof(*in2),
						 .is_negative = false,
						 .num = in2};
	tau->output = bi_subtract(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_neg_NaN_minus_largenum)
{
	digit_ty in2[] = {0, 7, 6, 5, 4, 3, 2, 1};

	tau->num1.is_negative = true;
	tau->num2 = (bigint){.len = sizeof(in2) / sizeof(*in2),
						 .is_negative = false,
						 .num = in2};
	tau->output = bi_subtract(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_NaN_minus_neg_largenum)
{
	digit_ty in2[] = {0, 7, 6, 5, 4, 3, 2, 1};

	tau->num2 = (bigint){.len = sizeof(in2) / sizeof(*in2),
						 .is_negative = true,
						 .num = in2};
	tau->output = bi_subtract(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_neg_NaN_minus_neg_largenum)
{
	digit_ty in2[] = {0, 7, 6, 5, 4, 3, 2, 1};

	tau->num1.is_negative = true;
	tau->num2 = (bigint){.len = sizeof(in2) / sizeof(*in2),
						 .is_negative = true,
						 .num = in2};
	tau->output = bi_subtract(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

/* ################################################################### */
/* ##################### simple_subtractions ######################### */
/* ################################################################### */

struct simple_subtractions
{
	bigint *restrict num1, *restrict num2, *restrict output;
	char *restrict outstr;
};

TEST_F_SETUP(simple_subtractions) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(simple_subtractions)
{
	bi_delete(tau->num1);
	bi_delete(tau->num2);
	bi_delete(tau->output);
	xfree(tau->outstr);
}

TEST_F(simple_subtractions, test_0_minus_0)
{
	const char expected[] = "0";

	tau->num1 = bi_new("0", 10, NULL);
	tau->num2 = bi_new("0", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_subtractions, test_1_minus_0)
{
	const char expected[] = "1";

	tau->num1 = bi_new("1", 10, NULL);
	tau->num2 = bi_new("0", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_subtractions, test_0_minus_1)
{
	const char expected[] = "-1";

	tau->num1 = bi_new("0", 10, NULL);
	tau->num2 = bi_new("1", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_subtractions, test_1_minus_1)
{
	const char expected[] = "0";

	tau->num1 = bi_new("1", 10, NULL);
	tau->num2 = bi_new("1", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_subtractions, test_1_000000000_000000000_minus_50000)
{
	const char expected[] = "999999999999950000";

	tau->num1 = bi_new("1_000000000_000000000", 10, NULL);
	tau->num2 = bi_new("50000", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_subtractions, test_50000_minus_1_000000000_000000000)
{
	const char expected[] = "-999999999999950000";

	tau->num1 = bi_new("50000", 10, NULL);
	tau->num2 = bi_new("1_000000000_000000000", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

/* ################################################################### */
/* ################### same_number_subtractions ###################### */
/* ################################################################### */

struct same_number_subtractions
{
	bigint *restrict num1, *restrict num2, *restrict output;
	char *restrict outstr;
};

TEST_F_SETUP(same_number_subtractions) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(same_number_subtractions)
{
	bi_delete(tau->num1);
	bi_delete(tau->num2);
	bi_delete(tau->output);
	xfree(tau->outstr);
}

TEST_F(same_number_subtractions, test_1_minus_1)
{
	const char expected[] = "0";

	tau->num1 = bi_new("1", 10, NULL);
	tau->num2 = bi_new("1", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(same_number_subtractions, test_1000000001_minus_1000000001)
{
	const char expected[] = "0";

	tau->num1 = bi_new("1000000001", 10, NULL);
	tau->num2 = bi_new("1000000001", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(same_number_subtractions, test_longnum_minus_longnum)
{
	const char expected[] = "0";

	tau->num1 = bi_new("1_000000001_000000001", 10, NULL);
	tau->num2 = bi_new("1_000000001_000000001", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(same_number_subtractions, test_largenum_minus_largenum)
{
	/* clang-format off */
	const char expected[] = "0";

	tau->num1 = bi_new("1_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001", 10, NULL);
	tau->num2 = bi_new("1_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(same_number_subtractions, test_almostsame_minus_almostsame)
{
	/* clang-format off */
	const char expected[] = "-1";

	tau->num1 = bi_new("1_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000000", 10, NULL);
	tau->num2 = bi_new("1_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(same_number_subtractions, test_almostsame_minus_almostsame_reverse)
{
	/* clang-format off */
	const char expected[] = "1";

	tau->num1 = bi_new("1_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001", 10, NULL);
	tau->num2 = bi_new("1_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000001_000000000", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

/* ################################################################### */
/* ###################### long_subtractions ########################## */
/* ################################################################### */

struct long_subtractions
{
	bigint *restrict num1, *restrict num2, *restrict output;
	char *restrict outstr;
};

TEST_F_SETUP(long_subtractions) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(long_subtractions)
{
	bi_delete(tau->num1);
	bi_delete(tau->num2);
	bi_delete(tau->output);
	xfree(tau->outstr);
}

TEST_F(long_subtractions, test_long9s_minus_1)
{
	/* clang-format off */
	const char expected[] = "999999999999999999999999999999999999999999999999999999999999998";

	tau->num1 = bi_new("999999999_999999999_999999999_999999999_999999999_999999999_999999999", 10, NULL);
	tau->num2 = bi_new("1", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(long_subtractions, test_1_minus_long9s)
{
	/* clang-format off */
	const char expected[] = "-999999999999999999999999999999999999999999999999999999999999998";

	tau->num1 = bi_new("1", 10, NULL);
	tau->num2 = bi_new("999999999_999999999_999999999_999999999_999999999_999999999_999999999", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(long_subtractions, test_long_sparse_num1_minus_long_sparse_num2)
{
	/* clang-format off */
	const char expected[] = "-41975999999999999999999999999999999999999111111112000000000000000000000000000000000000";

	tau->num1 = bi_new("56789_000000000_000000000_000000000_000000000_999999999_000000000_000000000_000000000_000000000", 10, NULL);
	tau->num2 = bi_new("98765_000000000_000000000_000000000_000000000_111111111_000000000_000000000_000000000_000000000", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

/********** REVERSE OF PREVIOUS *************/

TEST_F(long_subtractions, test_long_sparse_num2_minus_long_sparse_num1)
{
	/* clang-format off */
	const char expected[] = "41975999999999999999999999999999999999999111111112000000000000000000000000000000000000";

	tau->num1 = bi_new("98765_000000000_000000000_000000000_000000000_111111111_000000000_000000000_000000000_000000000", 10, NULL);
	tau->num2 = bi_new("56789_000000000_000000000_000000000_000000000_999999999_000000000_000000000_000000000_000000000", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(long_subtractions, test_4000000000678_minus_999999000)
{
	const char expected[] = "3999000001678";

	tau->num1 = bi_new("4000000000678", 10, NULL);
	tau->num2 = bi_new("999999000", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

/********** REVERSE OF PREVIOUS *************/

TEST_F(long_subtractions, test_999999000_minus_4000000000678)
{
	const char expected[] = "-3999000001678";

	tau->num1 = bi_new("999999000", 10, NULL);
	tau->num2 = bi_new("4000000000678", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

/* ################################################################### */
/* ##################### negative_subtractions ####################### */
/* ################################################################### */

struct negative_subtractions
{
	bigint *restrict num1, *restrict num2, *restrict output;
	char *restrict outstr;
};

TEST_F_SETUP(negative_subtractions) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(negative_subtractions)
{
	bi_delete(tau->num1);
	bi_delete(tau->num2);
	bi_delete(tau->output);
	xfree(tau->outstr);
}

TEST_F(negative_subtractions, test_neg1_minus_neg1)
{
	const char expected[] = "0";

	tau->num1 = bi_new("-1", 10, NULL);
	tau->num2 = bi_new("-1", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(negative_subtractions, test_1_minus_neg1)
{
	const char expected[] = "2";

	tau->num1 = bi_new("1", 10, NULL);
	tau->num2 = bi_new("-1", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(negative_subtractions, test_neg1_minus_1)
{
	const char expected[] = "-2";

	tau->num1 = bi_new("-1", 10, NULL);
	tau->num2 = bi_new("1", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

/* ################################################################### */
/* ###################### large_subtractions ######################### */
/* ################################################################### */

struct large_subtractions
{
	bigint *restrict num1, *restrict num2, *restrict output;
	char *restrict outstr;
};

TEST_F_SETUP(large_subtractions) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(large_subtractions)
{
	bi_delete(tau->num1);
	bi_delete(tau->num2);
	bi_delete(tau->output);
	xfree(tau->outstr);
}

TEST_F(large_subtractions, test_o1kb_minus_o1kc)
{
	/* clang-format off */
	const char expected[] = "18229261729562381999943195364150791141310842462660658472794052519590847565789349402718324004839857142928212620403202777713783604366202070759555626401852588078440691829064124951508218929855914917618450280848912007284499268739280728777673597141834727026189637501497182469116507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363725908514186546204357679842338718477444792073993423658482382428119816381501067481045166037730605620161967625613384414360383390441495263443219011465754445417842402092461651572335077870774981712577246796292638635637328991215483143816789988504044536402352738195137863656439121201039712282212072035747723202909336516542736369239534563619774418024541935268238240138010931106827128463433638706953587774858293618793747247853007337951511006306768696205902321912271659743738793851605740584319133365398057039499873779344804399418219036747743139037010602011830710941205985393702809196251537930575609145272741150361826225195425360496005836175816862574623784743992763686463418814045472979164057243894411378771231856466071795154085141271250844292645332607238119089307615090884745510925671879767885371000989331269526073212960479634709967894151160492018547692700582814284203702545004976494683959080140806281976692585019553782077204065086081221091696255332203782378235347294508254054853697025561506646565336686230510824911242699435683247925844205185052270721101555358378473298658512696420345599233953897801987527117516411360705303883450764631633372414047778082293889638662474154625684909253542894961768035925684575204907105615235243907554462309646434030739219644316363683837339421388133259223669435097200422814066071306602267381184984226812831185040602626717334620742176832432746723116666878494099225409409311050926218336148544438539667380387340520478289978174307602083635379706606909805094215664772278508941971642717971841891076715571559316901581472296756004741238219563900918074137045551522980957598119587441917225922826283596432510673475193183632879923869324027098297178672373453587588723240952542587540653855026365446553497596820167610376557700383595492057631084319813258375191610601970642940333397166593751788503039976940716673609266427519406688400311659057588715005508487576009759536802509536757334741833602329248066251219840709617150385796290367870393672814575138929537718229393152390372623810152981624504251783671794702503932393788881220797466762069198154524961236967509186640224450084190654970052454291507387970756908792516077113122486632528012574566002352425641172051374709119894964157128913124109250294313866110282503520730678541742478765867656061726545477525915193703649663835744447251897415065613249834420975592746185234528393156359085008975419826283842326451262484827345368141551864207150817577574741086807158134537956424596144989515225876079803696403415180669612275114885024970195217387099105054909491434854977266967507277206696158476678920996453566656932687090186494595387760332403402870170649984268991159832500318572909067401250933409067927675844010231803892134437444996108711770608525115879";

	tau->num1 = bi_new((const char[]){'1', '8', '2', '2', '9', '2', '6', '1', '7', '2', '9', '5', '6', '2', '3', '8', '1', '9', '9', '9', '9', '4', '3', '1', '9', '5', '3', '6', '4', '1', '5', '0', '7', '9', '1', '1', '4', '1', '3', '1', '0', '8', '4', '2', '4', '6', '2', '6', '6', '0', '6', '5', '8', '4', '7', '2', '7', '9', '4', '0', '5', '2', '5', '1', '9', '5', '9', '0', '8', '4', '7', '5', '6', '5', '7', '8', '9', '3', '4', '9', '4', '0', '2', '7', '1', '8', '3', '2', '4', '0', '0', '4', '8', '3', '9', '8', '5', '7', '1', '4', '2', '9', '2', '8', '2', '1', '2', '6', '2', '0', '4', '0', '3', '2', '0', '2', '7', '7', '7', '7', '1', '3', '7', '8', '3', '6', '0', '4', '3', '6', '6', '2', '0', '2', '0', '7', '0', '7', '5', '9', '5', '5', '5', '6', '2', '6', '4', '0', '1', '8', '5', '2', '5', '8', '8', '0', '7', '8', '4', '4', '0', '6', '9', '1', '8', '2', '9', '0', '6', '4', '1', '2', '4', '9', '5', '1', '5', '0', '8', '2', '1', '8', '9', '2', '9', '8', '5', '5', '9', '1', '4', '9', '1', '7', '6', '1', '8', '4', '5', '0', '2', '8', '0', '8', '4', '8', '9', '1', '2', '0', '0', '7', '2', '8', '4', '4', '9', '9', '2', '6', '8', '7', '3', '9', '2', '8', '0', '7', '2', '8', '7', '7', '7', '6', '7', '3', '5', '9', '7', '1', '4', '1', '8', '3', '4', '7', '2', '7', '0', '2', '6', '1', '8', '9', '6', '3', '7', '5', '0', '1', '4', '9', '7', '1', '8', '2', '4', '6', '9', '1', '1', '6', '5', '0', '7', '7', '6', '1', '3', '3', '7', '9', '8', '5', '9', '0', '9', '5', '7', '0', '0', '0', '9', '7', '3', '3', '0', '4', '5', '9', '7', '4', '8', '8', '0', '8', '4', '2', '8', '4', '0', '1', '7', '9', '7', '4', '2', '9', '1', '0', '0', '6', '4', '2', '4', '5', '8', '6', '9', '1', '8', '1', '7', '1', '9', '5', '1', '1', '8', '7', '4', '6', '1', '2', '1', '5', '1', '5', '1', '7', '2', '6', '5', '4', '6', '3', '2', '2', '8', '2', '2', '1', '6', '8', '6', '9', '9', '8', '7', '5', '4', '9', '1', '8', '2', '4', '2', '2', '4', '3', '3', '6', '3', '7', '2', '5', '9', '0', '8', '5', '1', '4', '1', '8', '6', '5', '4', '6', '2', '0', '4', '3', '5', '7', '6', '7', '9', '8', '4', '2', '3', '3', '8', '7', '1', '8', '4', '7', '7', '4', '4', '4', '7', '9', '2', '0', '7', '3', '9', '9', '3', '4', '2', '3', '6', '5', '8', '4', '8', '2', '3', '8', '2', '4', '2', '8', '1', '1', '9', '8', '1', '6', '3', '8', '1', '5', '0', '1', '0', '6', '7', '4', '8', '1', '0', '4', '5', '1', '6', '6', '0', '3', '7', '7', '3', '0', '6', '0', '5', '6', '2', '0', '1', '6', '1', '9', '6', '7', '6', '2', '5', '6', '1', '3', '3', '8', '4', '4', '1', '4', '3', '6', '0', '3', '8', '3', '3', '9', '0', '4', '4', '1', '4', '9', '5', '2', '6', '3', '4', '4', '3', '2', '1', '9', '0', '1', '1', '4', '6', '5', '7', '5', '4', '4', '4', '5', '4', '1', '7', '8', '4', '2', '4', '0', '2', '0', '9', '2', '4', '6', '1', '6', '5', '1', '5', '7', '2', '3', '3', '5', '0', '7', '7', '8', '7', '0', '7', '7', '4', '9', '8', '1', '7', '1', '2', '5', '7', '7', '2', '4', '6', '7', '9', '6', '2', '9', '2', '6', '3', '8', '6', '3', '5', '6', '3', '7', '3', '2', '8', '9', '9', '1', '2', '1', '5', '4', '8', '3', '1', '4', '3', '8', '1', '6', '7', '8', '9', '9', '8', '8', '5', '0', '4', '0', '4', '4', '5', '3', '6', '4', '0', '2', '3', '5', '2', '7', '3', '8', '1', '9', '5', '1', '3', '7', '8', '6', '3', '6', '5', '6', '4', '3', '9', '1', '2', '1', '2', '0', '1', '0', '3', '9', '7', '1', '2', '2', '8', '2', '2', '1', '2', '0', '7', '2', '0', '3', '5', '7', '4', '7', '7', '2', '3', '2', '0', '2', '9', '0', '9', '3', '3', '6', '5', '1', '6', '5', '4', '2', '7', '3', '6', '3', '6', '9', '2', '3', '9', '5', '3', '4', '5', '6', '3', '6', '1', '9', '7', '7', '4', '4', '1', '8', '0', '2', '4', '5', '4', '1', '9', '3', '5', '2', '6', '8', '2', '3', '8', '2', '4', '0', '1', '3', '8', '0', '1', '0', '9', '3', '1', '1', '0', '6', '8', '2', '7', '1', '2', '8', '4', '6', '3', '4', '3', '3', '6', '3', '8', '7', '0', '6', '9', '5', '3', '5', '8', '7', '7', '7', '4', '8', '5', '8', '2', '9', '3', '6', '1', '8', '7', '9', '3', '7', '4', '7', '2', '4', '7', '8', '5', '3', '0', '0', '7', '3', '3', '7', '9', '5', '1', '5', '1', '1', '0', '0', '6', '3', '0', '6', '7', '6', '8', '6', '9', '6', '2', '0', '5', '9', '0', '2', '3', '2', '1', '9', '1', '2', '2', '7', '1', '6', '5', '9', '7', '4', '3', '7', '3', '8', '7', '9', '3', '8', '5', '1', '6', '0', '5', '7', '4', '0', '5', '8', '4', '3', '1', '9', '1', '3', '3', '3', '6', '6', '0', '9', '9', '8', '5', '8', '3', '3', '3', '2', '8', '4', '8', '8', '7', '9', '7', '2', '9', '2', '5', '2', '0', '9', '5', '1', '9', '6', '2', '2', '8', '0', '5', '7', '7', '7', '8', '0', '8', '9', '7', '9', '1', '0', '1', '8', '0', '3', '5', '9', '4', '5', '0', '9', '1', '0', '5', '8', '5', '2', '5', '9', '0', '1', '5', '6', '3', '7', '7', '4', '6', '8', '5', '1', '2', '8', '4', '2', '7', '0', '4', '0', '6', '6', '3', '8', '4', '6', '2', '9', '0', '4', '1', '6', '2', '0', '5', '1', '0', '8', '2', '8', '6', '7', '3', '6', '9', '0', '6', '3', '4', '2', '0', '8', '6', '4', '4', '5', '2', '4', '7', '1', '3', '4', '6', '7', '1', '2', '4', '0', '0', '3', '9', '5', '4', '7', '5', '8', '7', '6', '6', '5', '9', '1', '7', '8', '8', '0', '4', '0', '7', '1', '1', '9', '5', '6', '6', '2', '3', '2', '5', '2', '2', '2', '9', '6', '0', '7', '4', '9', '0', '0', '1', '5', '8', '6', '0', '7', '4', '5', '9', '2', '2', '7', '9', '9', '0', '6', '3', '1', '5', '4', '1', '0', '5', '9', '3', '3', '8', '7', '1', '3', '9', '6', '5', '1', '4', '3', '2', '6', '3', '3', '0', '2', '7', '7', '1', '2', '8', '7', '8', '6', '4', '4', '8', '2', '1', '3', '6', '8', '4', '1', '6', '3', '1', '3', '1', '6', '9', '9', '6', '5', '5', '3', '5', '2', '7', '4', '0', '2', '7', '7', '9', '6', '0', '7', '1', '2', '8', '2', '7', '0', '8', '0', '2', '8', '2', '0', '6', '4', '3', '2', '5', '7', '2', '2', '8', '7', '2', '1', '6', '9', '0', '0', '8', '2', '7', '7', '0', '7', '5', '9', '3', '7', '1', '3', '4', '0', '9', '2', '4', '9', '6', '1', '6', '7', '5', '8', '0', '3', '2', '1', '3', '8', '5', '8', '2', '0', '1', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '8', '9', '6', '8', '5', '2', '1', '2', '2', '1', '5', '9', '7', '4', '8', '0', '4', '0', '8', '2', '6', '3', '6', '6', '9', '8', '2', '3', '3', '5', '8', '3', '5', '3', '5', '6', '4', '2', '7', '8', '2', '5', '2', '4', '4', '6', '4', '8', '5', '9', '0', '7', '7', '7', '6', '2', '9', '6', '7', '7', '4', '2', '7', '5', '3', '7', '1', '6', '1', '6', '0', '6', '3', '9', '7', '6', '5', '2', '6', '6', '7', '8', '2', '4', '1', '4', '8', '5', '8', '4', '0', '7', '4', '2', '3', '8', '7', '5', '9', '1', '8', '3', '3', '1', '5', '8', '6', '2', '9', '5', '1', '7', '4', '7', '1', '4', '0', '4', '8', '9', '6', '3', '3', '1', '5', '4', '3', '4', '9', '9', '1', '6', '6', '0', '0', '9', '0', '4', '4', '1', '9', '1', '6', '7', '4', '6', '0', '8', '6', '6', '6', '1', '3', '1', '8', '6', '5', '2', '7', '1', '3', '1', '6', '0', '9', '8', '0', '9', '4', '0', '2', '6', '8', '8', '8', '8', '8', '9', '5', '9', '3', '3', '8', '8', '6', '6', '1', '4', '4', '5', '4', '6', '7', '2', '9', '2', '1', '6', '5', '9', '1', '1', '3', '5', '4', '1', '6', '3', '5', '1', '4', '7', '0', '3', '0', '7', '8', '1', '6', '6', '5', '5', '4', '6', '5', '6', '0', '9', '2', '4', '5', '4', '8', '2', '6', '9', '7', '9', '4', '6', '3', '8', '9', '1', '7', '4', '7', '2', '8', '5', '2', '0', '2', '9', '2', '9', '5', '6', '6', '4', '5', '4', '9', '2', '0', '9', '9', '1', '5', '3', '2', '5', '7', '5', '7', '3', '1', '1', '7', '8', '5', '4', '6', '9', '6', '9', '7', '1', '4', '4', '5', '6', '0', '5', '9', '1', '7', '4', '9', '9', '2', '2', '6', '1', '0', '5', '1', '8', '7', '8', '5', '5', '7', '1', '4', '2', '5', '5', '4', '1', '8', '7', '4', '7', '6', '3', '1', '2', '7', '2', '9', '7', '6', '7', '3', '7', '6', '1', '7', '0', '8', '3', '9', '1', '7', '5', '9', '3', '1', '9', '2', '3', '3', '4', '9', '7', '6', '4', '4', '6', '1', '1', '9', '3', '8', '9', '1', '2', '7', '5', '1', '5', '7', '1', '6', '3', '6', '1', '9', '3', '7', '4', '7', '2', '6', '6', '9', '5', '6', '3', '7', '6', '8', '4', '4', '6', '1', '8', '7', '4', '7', '0', '7', '7', '9', '5', '0', '1', '4', '9', '4', '6', '0', '0', '7', '8', '6', '5', '7', '8', '5', '1', '9', '2', '9', '6', '6', '9', '4', '0', '3', '7', '3', '0', '7', '2', '7', '8', '9', '1', '1', '3', '5', '4', '7', '5', '3', '4', '5', '4', '2', '5', '8', '3', '5', '9', '4', '8', '6', '1', '8', '9', '0', '7', '7', '0', '0', '2', '1', '6', '9', '8', '9', '9', '9', '2', '1', '7', '5', '7', '1', '2', '0', '0', '0', '5', '6', '9', '0', '2', '0', '8', '7', '0', '3', '8', '6', '6', '0', '1', '2', '8', '2', '9', '1', '4', '2', '3', '2', '5', '0', '9', '4', '2', '6', '2', '3', '8', '5', '4', '2', '0', '6', '8', '9', '7', '2', '3', '3', '1', '4', '1', '8', '7', '1', '1', '6', '3', '3', '0', '0', '3', '4', '8', '7', '1', '5', '7', '5', '1', '9', '5', '4', '9', '1', '6', '3', '7', '9', '8', '7', '4', '9', '6', '6', '7', '8', '1', '4', '8', '0', '4', '6', '5', '8', '2', '6', '6', '9', '6', '8', '7', '6', '5', '0', '6', '1', '0', '7', '3', '5', '3', '8', '3', '5', '3', '4', '6', '3', '0', '3', '2', '2', '4', '1', '5', '1', '7', '5', '8', '8', '2', '4', '3', '0', '6', '6', '2', '7', '8', '5', '1', '4', '1', '3', '1', '9', '8', '1', '8', '1', '6', '0', '6', '7', '3', '3', '1', '9', '4', '2', '8', '3', '9', '9', '2', '1', '1', '8', '0', '8', '9', '5', '5', '1', '6', '5', '9', '3', '4', '9', '1', '9', '9', '6', '2', '8', '1', '8', '0', '2', '2', '3', '5', '9', '0', '1', '1', '0', '4', '7', '4', '9', '1', '6', '3', '0', '3', '5', '5', '7', '9', '2', '6', '8', '3', '4', '0', '6', '4', '8', '3', '6', '9', '8', '5', '0', '6', '7', '5', '2', '3', '6', '3', '9', '9', '1', '7', '9', '1', '1', '5', '2', '2', '2', '7', '0', '1', '8', '0', '1', '2', '9', '3', '7', '8', '5', '0', '1', '4', '1', '9', '3', '5', '8', '0', '4', '0', '5', '1', '2', '0', '2', '0', '4', '5', '8', '6', '7', '4', '1', '0', '6', '1', '2', '3', '5', '9', '6', '2', '7', '6', '6', '5', '8', '3', '9', '0', '7', '0', '9', '4', '0', '2', '1', '8', '7', '9', '2', '1', '5', '1', '7', '1', '4', '8', '3', '1', '1', '9', '1', '3', '9', '8', '9', '4', '8', '7', '0', '1', '3', '3', '0', '9', '1', '1', '1', '1', '0', '4', '4', '9', '0', '1', '6', '8', '3', '4', '0', '0', '9', '4', '9', '4', '8', '3', '8', '4', '6', '8', '1', '8', '2', '9', '9', '5', '1', '8', '0', '4', '1', '7', '6', '3', '5', '0', '7', '9', '4', '8', '9', '2', '2', '5', '9', '0', '7', '7', '4', '9', '2', '5', '4', '6', '6', '0', '8', '8', '1', '7', '1', '8', '7', '9', '2', '5', '9', '4', '6', '5', '9', '2', '1', '0', '2', '6', '5', '9', '7', '0', '4', '6', '7', '0', '0', '4', '4', '9', '8', '1', '9', '8', '9', '9', '0', '9', '6', '8', '6', '2', '0', '3', '9', '4', '6', '0', '0', '1', '7', '7', '4', '3', '0', '9', '4', '4', '7', '3', '8', '1', '1', '0', '5', '6', '9', '9', '1', '2', '9', '4', '1', '2', '8', '5', '4', '2', '8', '9', '1', '8', '8', '0', '8', '5', '5', '3', '6', '2', '7', '0', '7', '4', '0', '7', '6', '7', '0', '7', '2', '2', '5', '9', '3', '7', '3', '7', '7', '7', '2', '6', '6', '6', '9', '7', '3', '4', '4', '0', '9', '7', '7', '3', '6', '1', '2', '4', '3', '3', '3', '6', '3', '9', '7', '3', '0', '8', '0', '5', '1', '7', '6', '3', '0', '9', '1', '5', '0', '6', '8', '3', '6', '3', '1', '0', '7', '9', '5', '3', '1', '2', '6', '0', '7', '2', '3', '9', '5', '2', '0', '3', '6', '5', '2', '9', '0', '0', '3', '2', '1', '0', '5', '8', '4', '8', '8', '3', '9', '5', '0', '7', '9', '8', '1', '4', '5', '2', '3', '0', '7', '2', '9', '9', '4', '1', '7', '1', '8', '5', '7', '1', '5', '7', '9', '6', '2', '9', '7', '4', '5', '4', '9', '9', '5', '0', '2', '3', '5', '0', '5', '3', '1', '6', '0', '4', '0', '9', '1', '9', '8', '5', '9', '1', '9', '3', '7', '1', '8', '0', '2', '3', '3', '0', '7', '4', '1', '4', '8', '8', '0', '4', '4', '6', '2', '1', '7', '9', '2', '2', '8', '0', '0', '8', '3', '1', '7', '6', '6', '0', '4', '0', '9', '3', '8', '6', '5', '6', '3', '4', '4', '5', '7', '1', '0', '3', '4', '7', '7', '8', '5', '5', '3', '4', '5', '7', '1', '2', '1', '0', '8', '0', '5', '3', '0', '7', '3', '6', '3', '9', '4', '5', '3', '5', '9', '2', '3', '9', '3', '2', '6', '5', '1', '8', '6', '6', '0', '3', '0', '5', '1', '5', '0', '4', '1', '0', '6', '0', '9', '6', '6', '4', '3', '7', '3', '1', '3', '3', '2', '3', '6', '7', '2', '8', '3', '1', '5', '3', '9', '3', '2', '3', '5', '0', '0', '0', '6', '7', '9', '3', '7', '1', '0', '7', '5', '4', '1', '9', '5', '5', '4', '3', '7', '3', '6', '2', '4', '3', '3', '2', '4', '8', '3', '6', '1', '2', '4', '2', '5', '2', '5', '9', '4', '5', '8', '6', '8', '8', '0', '2', '3', '5', '3', '9', '1', '6', '7', '6', '6', '1', '8', '1', '5', '3', '2', '3', '7', '5', '8', '5', '5', '5', '0', '4', '8', '8', '6', '9', '0', '1', '4', '3', '2', '2', '2', '1', '3', '4', '9', '7', '3', '3', '3', '5', '4', '0', '9', '7', '3', '9', '1', '1', '5', '6', '0', '2', '1', '0', '7', '5', '9', '4', '1', '9', '0', '7', '6', '5', '8', '8', '5', '8', '0', '1', '4', '3', '2', '2', '6', '6', '2', '9', '6', '9', '3', '5', '1', '6', '6', '7', '5', '6', '9', '0', '0', '7', '6', '6', '4', '4', '9', '8', '3', '3', '3', '6', '8', '2', '1', '5', '5', '8', '1', '5', '6', '5', '1', '4', '1', '0', '9', '6', '1', '7', '8', '7', '8', '5', '1', '8', '1', '0', '1', '0', '3', '7', '4', '4', '5', '1', '9', '7', '3', '8', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '2', '3', '7', '6', '8', '7', '3', '4', '1', '9', '4', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8', '3', '7', '8', '3', '4', '5', '8', '9', '2', '3', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '3', '9', '4', '5', '6', '8', '3', '7', '6', '7', '0', '0', '0', '0', '0', '3', '2', '4', '0', '0', '0', '5', '4', '3', '6', '3', '0', '6', '4', '0', '0', '0', '0', '0', '0', '4', '4', '9', '3', '8', '9', '3', '4', '7', '3', '2', '7', '8', '7', '0', '0', '0', '3', '5', '6', '4', '5', '0', '0', '5', '6', '6', '9', '0', '4', '7', '4', '7', '6', '4', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '5', '2', '6', '8', '7', '2', '9', '6', '4', '2', '2', '4', '5', '2', '2', '3', '1', '9', '1', '7', '5', '4', '6', '1', '8', '8', '8', '7', '3', '6', '0', '8', '4', '8', '5', '0', '6', '3', '8', '9', '8', '8', '3', '9', '4', '1', '8', '8', '3', '3', '2', '9', '4', '1', '0', '8', '3', '7', '3', '0', '3', '9', '8', '6', '7', '5', '3', '7', '6', '1', '3', '7', '5', '5', '2', '7', '0', '7', '7', '1', '0', '0', '1', '8', '3', '6', '2', '0', '8', '1', '1', '9', '1', '0', '8', '8', '9', '3', '4', '3', '0', '6', '4', '5', '8', '3', '6', '2', '3', '6', 0}, 10, NULL);
	tau->num2 = bi_new((const char[]){'7', '0', '1', '8', '0', '1', '2', '9', '3', '7', '8', '5', '0', '1', '4', '1', '9', '3', '5', '8', '0', '4', '0', '5', '1', '2', '0', '2', '0', '4', '5', '8', '6', '7', '4', '1', '0', '6', '1', '2', '3', '5', '9', '6', '2', '7', '6', '6', '5', '8', '3', '9', '0', '7', '0', '9', '4', '0', '2', '1', '8', '7', '9', '2', '1', '5', '1', '7', '1', '4', '8', '3', '1', '1', '9', '1', '3', '9', '8', '9', '4', '8', '7', '0', '1', '3', '3', '0', '9', '1', '1', '1', '1', '0', '4', '4', '9', '0', '1', '6', '8', '3', '4', '0', '0', '9', '4', '9', '4', '8', '3', '8', '4', '6', '8', '1', '8', '2', '9', '9', '5', '1', '8', '0', '4', '1', '7', '6', '3', '5', '0', '7', '9', '4', '8', '9', '2', '2', '5', '9', '0', '7', '7', '4', '9', '2', '5', '4', '6', '6', '0', '8', '8', '1', '7', '1', '8', '7', '9', '2', '5', '9', '4', '6', '5', '9', '2', '1', '0', '2', '6', '5', '9', '7', '0', '4', '6', '7', '0', '0', '4', '4', '9', '8', '1', '9', '8', '9', '9', '0', '9', '6', '8', '6', '2', '0', '3', '9', '4', '6', '0', '0', '1', '7', '7', '4', '3', '0', '9', '4', '4', '7', '3', '8', '1', '1', '0', '5', '6', '9', '9', '1', '2', '9', '4', '1', '2', '8', '5', '4', '2', '8', '9', '1', '8', '8', '0', '8', '5', '5', '3', '6', '2', '7', '0', '7', '4', '0', '7', '6', '7', '0', '7', '2', '2', '5', '9', '3', '7', '3', '7', '7', '7', '2', '6', '6', '6', '9', '7', '3', '4', '4', '0', '9', '7', '7', '3', '6', '1', '2', '4', '3', '3', '3', '6', '3', '9', '7', '3', '0', '8', '0', '5', '1', '7', '6', '3', '0', '9', '1', '5', '0', '6', '8', '3', '6', '3', '1', '0', '7', '9', '5', '3', '1', '2', '6', '0', '7', '2', '3', '9', '5', '2', '0', '3', '6', '5', '2', '9', '0', '0', '3', '2', '1', '0', '5', '8', '4', '8', '8', '3', '9', '5', '0', '7', '9', '8', '1', '4', '5', '2', '3', '0', '7', '2', '9', '9', '4', '1', '7', '1', '8', '5', '7', '1', '5', '7', '9', '6', '2', '9', '7', '4', '5', '4', '9', '9', '5', '0', '2', '3', '5', '0', '5', '3', '1', '6', '0', '4', '0', '9', '1', '9', '8', '5', '9', '1', '9', '3', '7', '1', '8', '0', '2', '3', '3', '0', '7', '4', '1', '4', '8', '8', '0', '4', '4', '6', '2', '1', '7', '9', '2', '2', '8', '0', '0', '8', '3', '1', '7', '6', '6', '0', '4', '0', '9', '3', '8', '6', '5', '6', '3', '4', '4', '5', '7', '1', '0', '3', '4', '7', '7', '8', '5', '5', '3', '4', '5', '7', '1', '2', '1', '0', '8', '0', '5', '3', '0', '7', '3', '6', '3', '9', '4', '5', '3', '5', '9', '2', '3', '9', '3', '2', '6', '5', '1', '8', '6', '6', '0', '3', '0', '5', '1', '5', '0', '4', '1', '0', '6', '0', '9', '6', '6', '4', '3', '7', '3', '1', '3', '3', '2', '3', '6', '7', '2', '8', '3', '1', '5', '3', '9', '3', '2', '3', '5', '0', '0', '0', '6', '7', '9', '3', '7', '1', '0', '7', '5', '4', '1', '9', '5', '5', '4', '3', '7', '3', '6', '2', '4', '3', '3', '2', '4', '8', '3', '6', '1', '2', '4', '2', '5', '2', '5', '9', '4', '5', '8', '6', '8', '8', '0', '2', '3', '5', '3', '9', '1', '6', '7', '6', '6', '1', '8', '1', '5', '3', '2', '3', '7', '5', '8', '5', '5', '5', '0', '4', '8', '8', '6', '9', '0', '1', '4', '3', '2', '2', '2', '1', '3', '4', '9', '7', '3', '3', '1', '8', '6', '0', '2', '3', '9', '1', '2', '7', '0', '7', '6', '8', '4', '6', '5', '1', '7', '1', '9', '8', '3', '6', '9', '3', '5', '4', '0', '2', '6', '0', '7', '6', '8', '7', '5', '2', '6', '9', '5', '1', '5', '9', '3', '0', '5', '9', '2', '8', '3', '9', '1', '5', '0', '2', '0', '1', '0', '2', '8', '3', '5', '3', '8', '3', '7', '0', '3', '1', '0', '2', '5', '9', '7', '1', '3', '7', '3', '8', '5', '2', '2', '1', '6', '4', '7', '4', '3', '3', '2', '7', '9', '4', '9', '2', '0', '6', '4', '3', '3', '9', '9', '9', '0', '6', '8', '2', '2', '5', '5', '3', '1', '8', '5', '5', '0', '7', '2', '5', '5', '4', '6', '0', '6', '7', '8', '2', '1', '3', '8', '8', '0', '0', '8', '4', '1', '1', '6', '2', '8', '6', '6', '0', '3', '7', '3', '9', '3', '3', '2', '4', '6', '5', '7', '8', '1', '7', '1', '8', '0', '4', '2', '0', '1', '7', '1', '7', '2', '2', '2', '4', '4', '9', '9', '5', '4', '0', '3', '0', '3', '1', '5', '2', '9', '3', '5', '4', '7', '8', '7', '1', '4', '0', '1', '3', '6', '2', '9', '6', '1', '5', '0', '1', '0', '6', '5', '0', '0', '2', '4', '8', '6', '5', '5', '2', '6', '8', '8', '6', '6', '3', '4', '1', '5', '7', '4', '5', '9', '7', '5', '8', '9', '2', '5', '7', '9', '3', '5', '9', '4', '1', '6', '5', '6', '5', '1', '0', '2', '0', '7', '8', '9', '2', '2', '0', '0', '6', '7', '3', '1', '1', '4', '1', '6', '9', '2', '6', '0', '7', '6', '9', '4', '9', '7', '7', '7', '7', '6', '7', '6', '0', '4', '9', '0', '6', '1', '0', '7', '0', '6', '1', '9', '3', '7', '8', '7', '3', '5', '4', '0', '6', '0', '1', '5', '9', '4', '2', '7', '4', '7', '3', '1', '6', '1', '7', '6', '1', '9', '3', '7', '7', '5', '3', '7', '4', '1', '9', '0', '7', '1', '3', '0', '7', '1', '1', '5', '4', '9', '0', '0', '6', '5', '8', '5', '0', '3', '2', '6', '9', '4', '6', '5', '5', '1', '6', '4', '9', '6', '8', '2', '8', '5', '6', '8', '6', '5', '4', '3', '7', '7', '1', '8', '3', '1', '9', '0', '5', '8', '6', '9', '5', '3', '7', '6', '4', '0', '6', '9', '8', '0', '4', '4', '9', '3', '2', '6', '3', '8', '8', '9', '3', '4', '9', '2', '4', '5', '7', '9', '1', '4', '7', '5', '0', '8', '5', '5', '8', '5', '8', '9', '8', '0', '8', '4', '9', '1', '9', '0', '4', '8', '8', '3', '8', '5', '3', '1', '5', '0', '7', '6', '9', '2', '2', '4', '5', '3', '7', '5', '5', '5', '2', '7', '4', '8', '1', '1', '3', '7', '6', '7', '1', '9', '0', '9', '6', '1', '4', '4', '1', '1', '9', '3', '9', '0', '0', '5', '2', '1', '9', '9', '0', '2', '7', '7', '1', '5', '6', '9', '1', '3', '0', '2', '6', '5', '7', '0', '7', '5', '2', '9', '5', '0', '9', '0', '8', '6', '9', '7', '3', '9', '7', '3', '0', '2', '5', '0', '3', '1', '5', '5', '9', '1', '8', '0', '3', '5', '8', '9', '1', '1', '2', '2', '8', '3', '5', '7', '6', '9', '3', '9', '8', '5', '8', '3', '9', '5', '5', '2', '9', '6', '3', '2', '6', '3', '4', '3', '0', '5', '9', '7', '6', '1', '4', '4', '5', '7', '1', '4', '4', '1', '6', '9', '6', '5', '9', '8', '1', '7', '0', '4', '0', '1', '2', '5', '1', '8', '5', '2', '1', '5', '9', '1', '3', '8', '5', '3', '3', '4', '5', '5', '9', '8', '2', '1', '7', '2', '3', '4', '3', '7', '1', '2', '3', '1', '3', '3', '8', '3', '2', '4', '7', '7', '3', '2', '1', '0', '7', '2', '6', '8', '5', '3', '5', '2', '4', '7', '7', '6', '3', '7', '8', '4', '1', '0', '5', '1', '8', '6', '5', '4', '9', '2', '4', '6', '1', '9', '9', '8', '8', '8', '0', '7', '0', '3', '3', '1', '0', '8', '8', '4', '6', '2', '8', '5', '5', '7', '4', '3', '5', '2', '0', '8', '8', '0', '6', '7', '1', '2', '9', '9', '3', '0', '2', '8', '9', '5', '5', '4', '6', '8', '2', '2', '6', '9', '5', '4', '9', '2', '9', '6', '8', '5', '7', '7', '3', '8', '0', '7', '0', '6', '7', '9', '5', '8', '4', '2', '8', '0', '2', '2', '0', '0', '8', '2', '9', '4', '1', '1', '1', '9', '8', '4', '2', '2', '2', '9', '7', '3', '2', '6', '0', '2', '0', '8', '2', '3', '3', '6', '9', '3', '1', '5', '2', '5', '8', '9', '2', '1', '1', '6', '2', '9', '9', '0', '1', '6', '8', '6', '9', '7', '3', '9', '3', '3', '4', '8', '7', '3', '6', '2', '3', '6', '0', '8', '1', '2', '9', '6', '6', '0', '4', '1', '8', '5', '1', '4', '5', '6', '9', '0', '6', '3', '9', '9', '5', '2', '8', '2', '9', '7', '8', '1', '7', '6', '7', '9', '0', '1', '4', '9', '7', '6', '0', '5', '2', '1', '3', '9', '5', '5', '4', '8', '5', '3', '2', '8', '1', '4', '1', '9', '6', '5', '3', '4', '6', '7', '6', '9', '7', '4', '2', '5', '9', '7', '4', '7', '9', '3', '0', '6', '8', '5', '8', '6', '4', '5', '8', '4', '9', '2', '6', '8', '3', '2', '8', '9', '8', '5', '6', '8', '7', '4', '2', '3', '8', '8', '1', '8', '5', '3', '6', '3', '2', '6', '0', '4', '7', '0', '6', '1', '7', '5', '5', '6', '4', '4', '6', '1', '7', '1', '9', '3', '9', '6', '1', '1', '7', '3', '1', '8', '2', '9', '8', '6', '7', '9', '8', '2', '0', '7', '8', '5', '4', '9', '1', '8', '7', '5', '6', '7', '4', '9', '4', '6', '7', '0', '0', '4', '1', '3', '6', '8', '0', '9', '3', '2', '1', '0', '3', '2', '5', '1', '9', '5', '9', '0', '8', '4', '7', '5', '6', '5', '7', '8', '9', '3', '4', '9', '4', '0', '2', '7', '1', '8', '3', '2', '4', '0', '0', '4', '8', '3', '9', '8', '5', '7', '1', '4', '2', '9', '2', '8', '2', '1', '2', '6', '2', '0', '4', '0', '3', '2', '0', '2', '7', '7', '7', '7', '1', '3', '7', '8', '3', '6', '0', '4', '3', '6', '6', '2', '0', '2', '0', '7', '0', '7', '5', '9', '5', '5', '5', '6', '2', '6', '4', '0', '1', '8', '5', '2', '5', '8', '8', '0', '7', '8', '4', '4', '0', '6', '9', '1', '8', '2', '9', '0', '6', '4', '1', '2', '4', '9', '5', '1', '5', '0', '8', '2', '1', '8', '9', '2', '9', '8', '5', '5', '9', '1', '4', '9', '1', '7', '6', '1', '8', '4', '5', '0', '2', '8', '0', '8', '4', '8', '9', '1', '2', '0', '0', '7', '2', '8', '4', '4', '9', '9', '2', '6', '8', '7', '3', '9', '2', '8', '0', '7', '2', '8', '7', '7', '7', '6', '7', '3', '5', '9', '7', '1', '4', '1', '8', '3', '4', '7', '2', '7', '0', '2', '6', '1', '8', '9', '6', '3', '7', '5', '0', '1', '4', '9', '7', '1', '8', '2', '4', '6', '9', '1', '1', '6', '5', '0', '7', '7', '6', '1', '3', '3', '7', '9', '8', '5', '9', '0', '9', '5', '7', '0', '0', '0', '9', '7', '3', '3', '0', '4', '5', '9', '7', '4', '8', '8', '0', '8', '4', '2', '8', '4', '0', '1', '7', '9', '7', '4', '2', '9', '1', '0', '0', '6', '4', '2', '4', '5', '8', '6', '9', '1', '8', '1', '7', '1', '9', '5', '1', '1', '8', '7', '4', '6', '1', '2', '1', '5', '1', '5', '1', '7', '2', '6', '5', '4', '6', '3', '2', '2', '8', '2', '2', '1', '6', '8', '6', '9', '9', '8', '7', '5', '4', '9', '1', '8', '2', '4', '2', '2', '4', '3', '3', '6', '3', '7', '2', '5', '9', '0', '8', '5', '1', '4', '1', '8', '6', '5', '4', '6', '2', '0', '4', '3', '5', '7', '6', '7', '9', '8', '4', '2', '3', '3', '8', '7', '1', '8', '4', '7', '7', '4', '4', '4', '7', '9', '2', '0', '7', '3', '9', '9', '3', '4', '2', '3', '6', '5', '8', '4', '8', '2', '3', '8', '2', '4', '2', '8', '1', '1', '9', '8', '1', '6', '3', '8', '1', '5', '0', '1', '0', '6', '7', '4', '8', '1', '0', '4', '5', '1', '6', '6', '0', '3', '7', '7', '3', '0', '6', '0', '5', '6', '2', '0', '1', '6', '1', '9', '6', '7', '6', '2', '5', '6', '1', '3', '3', '8', '4', '4', '1', '4', '3', '6', '0', '3', '8', '3', '3', '9', '0', '4', '4', '1', '4', '9', '5', '2', '6', '3', '4', '4', '3', '2', '1', '9', '0', '1', '1', '4', '6', '5', '7', '5', '4', '4', '4', '5', '4', '1', '7', '8', '4', '2', '4', '0', '2', '0', '9', '2', '4', '6', '1', '6', '5', '1', '5', '7', '2', '3', '3', '5', '0', '7', '7', '8', '7', '0', '7', '7', '4', '9', '8', '1', '7', '1', '2', '5', '7', '7', '2', '4', '6', '7', '9', '6', '2', '9', '2', '6', '3', '8', '6', '3', '5', '6', '3', '7', '3', '2', '8', '9', '9', '1', '2', '1', '5', '4', '8', '3', '1', '4', '3', '8', '1', '6', '7', '8', '9', '9', '8', '8', '5', '0', '4', '0', '4', '4', '5', '3', '6', '4', '0', '2', '3', '5', '2', '7', '3', '8', '1', '9', '5', '1', '3', '7', '8', '6', '3', '6', '5', '6', '4', '3', '9', '1', '2', '1', '2', '0', '1', '0', '3', '9', '7', '1', '2', '2', '8', '2', '2', '1', '2', '0', '7', '2', '0', '3', '5', '7', 0}, 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(large_subtractions, test_o1kb_minus_o1ka)
{
	/* clang-format off */
	const char expected[] = {'-', '6', '2', '7', '3', '5', '6', '4', '7', '4', '8', '1', '4', '3', '5', '0', '6', '2', '0', '1', '7', '1', '1', '3', '4', '8', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '9', '2', '7', '4', '3', '4', '3', '8', '8', '1', '5', '4', '6', '8', '3', '3', '0', '3', '2', '4', '3', '8', '5', '2', '1', '3', '1', '7', '9', '4', '9', '7', '8', '5', '5', '8', '1', '5', '5', '8', '5', '1', '0', '5', '8', '8', '6', '0', '7', '6', '6', '5', '6', '0', '0', '5', '4', '8', '5', '6', '5', '9', '5', '3', '7', '1', '4', '6', '6', '4', '0', '2', '2', '3', '5', '9', '2', '0', '4', '5', '2', '5', '7', '7', '0', '1', '4', '2', '4', '6', '0', '0', '1', '0', '0', '6', '0', '3', '3', '5', '9', '2', '2', '8', '0', '2', '9', '0', '0', '9', '4', '5', '6', '9', '8', '2', '8', '3', '0', '0', '7', '1', '3', '4', '8', '2', '7', '5', '3', '4', '2', '4', '3', '3', '1', '0', '6', '2', '5', '2', '1', '1', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '5', '3', '6', '9', '4', '4', '1', '0', '0', '3', '9', '1', '3', '6', '3', '7', '4', '1', '9', '1', '6', '4', '9', '3', '2', '6', '0', '1', '8', '7', '8', '1', '2', '3', '6', '2', '0', '7', '6', '6', '4', '4', '9', '0', '1', '7', '1', '9', '9', '2', '1', '2', '9', '4', '6', '5', '4', '9', '5', '4', '2', '0', '5', '2', '7', '1', '3', '3', '5', '7', '2', '6', '1', '8', '4', '6', '5', '6', '6', '1', '2', '9', '7', '7', '8', '0', '8', '5', '5', '7', '0', '0', '0', '0', '4', '3', '6', '8', '2', '8', '5', '4', '9', '0', '0', '8', '8', '0', '7', '7', '7', '3', '7', '5', '5', '7', '9', '8', '1', '1', '1', '8', '1', '8', '7', '2', '5', '1', '7', '5', '3', '9', '4', '6', '0', '1', '2', '4', '8', '3', '4', '7', '4', '8', '8', '2', '8', '6', '0', '8', '9', '3', '8', '7', '2', '1', '7', '8', '9', '8', '4', '7', '3', '8', '4', '0', '0', '5', '9', '2', '5', '6', '8', '6', '9', '5', '1', '4', '2', '6', '6', '7', '7', '1', '8', '4', '7', '4', '6', '3', '3', '4', '4', '8', '3', '1', '4', '0', '1', '7', '5', '8', '4', '9', '1', '8', '9', '8', '8', '4', '8', '9', '4', '4', '4', '6', '9', '6', '8', '7', '4', '1', '1', '0', '5', '0', '4', '1', '6', '1', '7', '5', '0', '8', '7', '3', '5', '0', '9', '3', '5', '8', '5', '2', '8', '8', '3', '1', '2', '3', '2', '9', '8', '0', '3', '6', '4', '8', '0', '4', '3', '7', '8', '9', '9', '5', '2', '9', '1', '3', '7', '1', '0', '6', '6', '6', '0', '4', '0', '0', '8', '0', '6', '0', '0', '7', '7', '6', '0', '1', '5', '7', '5', '8', '6', '9', '1', '5', '2', '4', '3', '8', '0', '5', '5', '2', '8', '8', '6', '4', '2', '3', '1', '6', '7', '5', '2', '9', '5', '9', '6', '2', '3', '6', '9', '6', '9', '9', '0', '4', '3', '8', '9', '0', '7', '3', '6', '4', '0', '8', '8', '4', '1', '1', '8', '2', '8', '8', '6', '1', '7', '8', '6', '8', '8', '0', '6', '5', '6', '7', '1', '7', '9', '7', '6', '8', '4', '5', '8', '9', '7', '1', '4', '6', '1', '2', '2', '6', '9', '7', '3', '1', '5', '2', '6', '5', '7', '4', '6', '0', '3', '5', '9', '5', '6', '5', '7', '9', '7', '9', '2', '8', '7', '1', '0', '5', '9', '6', '4', '1', '9', '5', '8', '4', '6', '3', '1', '1', '4', '2', '5', '6', '2', '4', '5', '9', '4', '9', '8', '2', '3', '4', '5', '6', '9', '0', '5', '0', '9', '5', '1', '6', '6', '1', '9', '8', '6', '9', '7', '5', '3', '8', '7', '1', '3', '2', '9', '6', '5', '8', '1', '7', '5', '8', '6', '7', '5', '7', '1', '0', '3', '2', '2', '6', '2', '9', '5', '2', '3', '6', '2', '4', '0', '0', '0', '1', '4', '3', '1', '2', '9', '1', '1', '1', '5', '7', '8', '0', '3', '1', '0', '2', '6', '6', '6', '2', '6', '9', '4', '9', '9', '5', '8', '0', '5', '1', '4', '6', '4', '1', '3', '5', '7', '1', '0', '4', '4', '5', '9', '1', '9', '9', '3', '8', '5', '3', '8', '9', '7', '6', '6', '2', '6', '6', '9', '7', '1', '9', '2', '7', '4', '9', '8', '6', '4', '4', '2', '8', '3', '1', '7', '8', '1', '2', '3', '2', '2', '5', '0', '2', '8', '2', '4', '8', '1', '5', '8', '7', '2', '9', '6', '9', '1', '5', '8', '8', '3', '0', '5', '7', '4', '5', '7', '0', '6', '1', '1', '2', '0', '9', '8', '0', '6', '5', '6', '6', '6', '3', '1', '4', '3', '3', '5', '5', '4', '2', '5', '1', '3', '7', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '3', '9', '6', '7', '8', '1', '0', '8', '3', '6', '6', '5', '9', '3', '6', '2', '0', '6', '5', '6', '2', '4', '8', '6', '0', '9', '8', '0', '2', '6', '8', '1', '3', '7', '8', '4', '8', '8', '0', '8', '4', '4', '5', '2', '5', '9', '1', '7', '4', '1', '2', '7', '3', '7', '1', '1', '8', '2', '4', '1', '9', '2', '5', '7', '2', '1', '1', '1', '5', '4', '7', '0', '9', '2', '1', '0', '4', '5', '1', '6', '7', '7', '6', '2', '9', '0', '9', '6', '5', '4', '7', '3', '5', '4', '7', '3', '6', '2', '3', '1', '7', '8', '0', '9', '3', '4', '7', '7', '4', '4', '7', '9', '3', '7', '3', '3', '1', '6', '3', '3', '1', '7', '3', '1', '0', '2', '0', '7', '5', '5', '6', '0', '4', '0', '5', '1', '3', '8', '2', '3', '2', '2', '9', '4', '7', '1', '2', '6', '3', '1', '0', '7', '2', '7', '8', '0', '8', '5', '5', '5', '1', '8', '2', '6', '8', '9', '8', '4', '2', '7', '7', '0', '7', '7', '6', '5', '5', '8', '0', '6', '3', '8', '4', '2', '2', '3', '5', '7', '6', '6', '6', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '2', '5', '9', '0', '8', '6', '7', '3', '8', '5', '0', '5', '7', '2', '4', '6', '8', '2', '9', '0', '3', '3', '7', '7', '2', '2', '6', '3', '0', '5', '3', '5', '0', '1', '7', '6', '0', '8', '8', '2', '5', '7', '5', '0', '2', '3', '3', '3', '0', '0', '4', '1', '5', '1', '2', '1', '1', '4', '3', '4', '9', '4', '4', '5', '7', '3', '6', '4', '1', '8', '2', '5', '3', '4', '8', '4', '4', '9', '7', '4', '9', '8', '7', '1', '5', '6', '9', '9', '0', '0', '3', '7', '8', '5', '4', '2', '8', '3', '7', '4', '4', '8', '4', '4', '6', '9', '7', '8', '8', '9', '3', '5', '2', '1', '6', '9', '0', '4', '6', '5', '9', '6', '7', '5', '2', '1', '1', '8', '5', '0', '9', '2', '7', '2', '3', '8', '7', '0', '1', '7', '1', '5', '7', '1', '5', '5', '4', '9', '0', '4', '7', '6', '3', '9', '8', '8', '0', '6', '7', '1', '5', '4', '6', '3', '3', '2', '7', '4', '8', '9', '5', '5', '0', '2', '7', '2', '8', '7', '6', '7', '3', '7', '8', '5', '3', '9', '5', '4', '6', '6', '8', '1', '5', '8', '1', '7', '1', '6', '1', '3', '5', '6', '2', '6', '9', '6', '8', '0', '9', '8', '5', '6', '8', '0', '9', '2', '6', '1', '6', '5', '2', '1', '7', '6', '5', '0', '6', '7', '1', '6', '8', '0', '4', '0', '3', '7', '9', '1', '9', '5', '2', '2', '6', '5', '1', '3', '6', '5', '5', '9', '6', '3', '1', '8', '1', '1', '7', '5', '0', '7', '2', '7', '0', '0', '0', '5', '2', '8', '7', '5', '1', '9', '3', '2', '8', '5', '5', '1', '9', '6', '6', '7', '1', '3', '7', '7', '9', '1', '9', '7', '0', '2', '0', '5', '9', '2', '1', '8', '2', '4', '4', '7', '4', '3', '2', '5', '3', '5', '3', '2', '6', '0', '6', '1', '4', '0', '4', '9', '6', '6', '2', '8', '2', '1', '0', '5', '0', '7', '6', '3', '5', '0', '5', '8', '2', '1', '3', '7', '8', '0', '4', '9', '6', '9', '4', '0', '3', '4', '5', '9', '4', '2', '4', '3', '9', '4', '6', '2', '1', '1', '5', '2', '5', '4', '1', '6', '9', '7', '3', '4', '1', '7', '9', '7', '3', '2', '1', '2', '0', '9', '8', '1', '1', '8', '3', '5', '4', '1', '6', '0', '3', '3', '1', '4', '5', '6', '0', '5', '5', '2', '5', '7', '3', '8', '3', '8', '9', '7', '6', '6', '9', '0', '3', '6', '1', '1', '9', '8', '8', '3', '6', '2', '2', '3', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '6', '7', '6', '6', '1', '3', '1', '0', '6', '4', '5', '4', '7', '8', '5', '5', '4', '4', '6', '1', '9', '5', '4', '9', '6', '3', '2', '6', '2', '4', '6', '8', '4', '0', '9', '3', '4', '5', '7', '1', '4', '1', '8', '7', '4', '7', '6', '7', '0', '0', '4', '9', '9', '8', '1', '9', '0', '3', '5', '4', '1', '4', '9', '7', '6', '9', '4', '7', '3', '3', '0', '9', '6', '4', '9', '1', '9', '4', '4', '9', '3', '1', '5', '1', '5', '5', '6', '0', '1', '4', '6', '3', '2', '2', '1', '4', '7', '9', '1', '7', '4', '4', '9', '1', '4', '8', '3', '8', '2', '7', '0', '0', '6', '1', '4', '4', '3', '0', '3', '4', '8', '1', '9', '9', '4', '6', '7', '2', '9', '2', '0', '2', '9', '5', '9', '8', '5', '3', '0', '7', '8', '9', '8', '4', '9', '9', '3', '5', '5', '1', '2', '4', '9', '0', '0', '5', '7', '8', '5', '0', '2', '2', '6', '8', '9', '2', '7', '5', '8', '4', '5', '0', '2', '7', '6', '3', '0', '1', '3', '7', '0', '5', '5', '7', '4', '6', '5', '1', '5', '0', '1', '8', '8', '7', '7', '9', '4', '3', '2', '1', '6', '3', '5', '7', '7', '2', '9', '9', '9', '6', '8', '6', '4', '1', '0', '5', '6', '8', '5', '0', '8', '9', '7', '7', '5', '0', '5', '7', '5', '0', '2', '9', '1', '4', '6', '8', '3', '0', '2', '8', '9', '7', '1', '7', '0', '1', '7', '4', '7', '4', '5', '9', '1', '8', '7', '5', '8', '3', '2', '4', '5', '0', '6', '0', '4', '7', '5', '8', '1', '7', '6', '8', '5', '8', '2', '4', '5', '4', '9', '0', '2', '3', '3', '9', '1', '8', '2', '8', '5', '9', '1', '1', '3', '6', '4', '2', '5', '1', '2', '1', '0', '4', '7', '3', '1', '8', '9', '2', '9', '7', '9', '4', '3', '6', '3', '4', '6', '9', '7', '1', '5', '2', '2', '3', '9', '4', '8', '9', '5', '8', '4', '2', '9', '1', '7', '4', '5', '3', '3', '9', '2', '3', '7', '4', '6', '3', '9', '9', '4', '4', '2', '6', '7', '9', '7', '3', '8', '0', '4', '5', '5', '4', '7', '1', '4', '5', '2', '6', '5', '1', '2', '2', '0', '3', '1', '5', '3', '8', '0', '8', '1', '0', '3', '4', '6', '6', '9', '7', '3', '4', '9', '6', '9', '3', '8', '1', '1', '4', '7', '3', '9', '1', '8', '0', '1', '6', '6', '1', '4', '9', '1', '8', '5', '4', '6', '9', '3', '9', '4', '1', '3', '8', '7', '9', '3', '3', '0', '8', '5', '0', '1', '7', '0', '5', '2', '5', '4', '6', '6', '2', '1', '0', '5', '7', '6', '4', '4', '1', '3', '9', '1', '5', '4', '2', '2', '4', '3', '2', '6', '8', '1', '5', '6', '4', '0', '4', '8', '0', '8', '1', '4', '5', '2', '8', '7', '2', '4', '8', '3', '1', '4', '4', '9', '2', '6', '3', '6', '9', '8', '8', '6', '6', '1', '4', '7', '6', '6', '6', '1', '0', '2', '7', '2', '5', '6', '7', '4', '2', '4', '8', '2', '0', '7', '6', '8', '6', '0', '1', '1', '7', '4', '2', '7', '0', '1', '2', '6', '6', '7', '5', '3', '4', '6', '0', '5', '6', '4', '3', '3', '6', '3', '9', '2', '8', '9', '3', '2', '8', '8', '4', '9', '3', '4', '3', '8', '1', '1', '4', '1', '2', '8', '1', '9', '4', '1', '6', '9', '2', '1', '6', '7', '3', '5', '4', '6', '3', '5', '6', '1', '0', '8', '7', '1', '5', '8', '9', '3', '3', '8', '5', '3', '2', '9', '4', '4', '8', '5', '1', '6', '8', '1', '5', '8', '3', '9', '6', '2', '7', '5', '4', '3', '3', '3', '8', '8', '5', '0', '9', '6', '9', '2', '9', '3', '7', '8', '4', '6', '1', '7', '6', '7', '4', '2', '1', '5', '9', '6', '3', '5', '0', '9', '5', '1', '6', '8', '0', '0', '0', '2', '1', '6', '0', '8', '1', '3', '5', '7', '6', '3', '0', '7', '3', '2', '4', '2', '7', '5', '7', '1', '5', '9', '7', '1', '3', '2', '8', '4', '5', '4', '1', '7', '5', '0', '5', '9', '0', '0', '5', '9', '3', '5', '8', '6', '9', '1', '8', '5', '1', '8', '1', '5', '5', '4', '8', '2', '4', '1', '8', '0', '9', '8', '3', '1', '2', '8', '2', '4', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '8', '8', '3', '6', '7', '4', '5', '5', '4', '9', '4', '8', '9', '0', '8', '6', '6', '7', '1', '2', '6', '1', '7', '8', '6', '2', '7', '2', '5', '6', '4', '5', '3', '2', '2', '8', '3', '9', '2', '6', '3', '2', '4', '1', '4', '7', '8', '3', '1', '5', '3', '6', '8', '9', '5', '3', '0', '9', '2', '1', '7', '4', '1', '1', '5', '0', '7', '1', '8', '1', '9', '1', '0', '6', '3', '8', '2', '5', '5', '0', '5', '4', '6', '8', '3', '8', '1', '2', '9', '0', '5', '3', '5', '7', '4', '4', '7', '5', '3', '5', '1', '9', '5', '4', '0', '4', '9', '2', '2', '7', '9', '1', '4', '8', '7', '6', '9', '7', '0', '2', '6', '3', '7', '1', '9', '9', '7', '5', '2', '9', '3', '7', '1', '2', '0', '4', '6', '6', '7', '8', '4', '0', '0', '1', '0', '3', '8', '5', '4', '7', '3', '8', '1', '7', '9', '1', '1', '4', '2', '5', '4', '4', '0', '2', '4', '0', '9', '0', '8', '3', '0', '4', '7', '8', '4', '4', '5', '0', '0', '8', '6', '8', '0', '9', '2', '6', '9', '9', '7', '3', '6', '4', '7', '2', '2', '5', '5', '9', '9', '5', '2', '5', '5', '9', '4', '2', '7', '7', '2', '0', '7', '0', '3', '6', '9', '0', '0', '9', '2', '4', '3', '0', '3', '2', '4', '5', '1', '0', '3', '3', '0', '2', '1', '6', '8', '1', '3', '4', '8', '9', '5', '6', '3', '5', '8', '5', '1', '3', '6', '7', '3', '2', '4', '9', '4', '5', '1', '0', '0', '2', '1', '6', '4', '3', '2', '2', '7', '3', '1', '3', '0', '3', '0', '1', '9', '6', '3', '7', '4', '5', '8', '7', '1', '4', '9', '7', '1', '5', '8', '7', '2', '4', '5', '3', '3', '8', '0', '4', '3', '4', '4', '0', '1', '8', '7', '5', '2', '1', '4', '9', '7', '3', '9', '7', '7', '2', '5', '4', '9', '0', '0', '2', '7', '8', '8', '3', '9', '2', '6', '2', '8', '5', '9', '2', '7', '3', '7', '7', '7', '4', '3', '3', '9', '3', '4', '5', '5', '3', '4', '5', '9', '7', '4', '9', '8', '6', '2', '5', '3', '2', '4', '0', '5', '6', '9', '2', '5', '6', '6', '0', '4', '7', '5', '0', '7', '1', '0', '1', '0', '7', '5', '0', '1', '6', '2', '4', '4', '3', '6', '8', '6', '0', '6', '6', '0', '1', '2', '6', '4', '9', '8', '0', '8', '8', '2', '5', '2', '6', '4', '7', '4', '9', '4', '4', '0', '1', '4', '1', '3', '9', '3', '6', '8', '0', '2', '6', '8', '4', '8', '9', '3', '0', '7', '1', '4', '6', '0', '3', '5', '4', '8', '1', '5', '6', '0', '3', '5', '6', '0', '3', '3', '8', '8', '0', '4', '3', '9', '0', '7', '9', '3', '5', '6', '5', '8', '4', '9', '2', '0', '8', '8', '5', '8', '6', '8', '9', '1', '5', '7', '5', '3', '7', '3', '3', '9', '3', '4', '1', '5', '2', '7', '2', '0', '5', '9', '4', '7', '4', '8', '0', '4', '0', '9', '1', '5', '2', '4', '3', '4', '2', '1', '0', '6', '5', '0', '5', '9', '7', '2', '8', '1', '6', '7', '5', '9', '9', '5', '1', '6', '0', '1', '4', '2', '8', '5', '7', '0', '7', '1', '7', '8', '7', '3', '7', '9', '5', '9', '6', '7', '9', '7', '2', '2', '2', '2', '8', '6', '2', '1', '6', '3', '9', '5', '6', '3', '3', '7', '9', '7', '9', '2', '9', '2', '4', '0', '4', '4', '4', '3', '7', '3', '5', '9', '8', '1', '4', '7', '4', '1', '1', '9', '2', '1', '5', '5', '9', '3', '0', '8', '1', '7', '0', '9', '3', '5', '8', '7', '5', '0', '4', '8', '4', '9', '1', '7', '8', '1', '0', '7', '0', '1', '4', '4', '0', '8', '5', '0', '8', '2', '3', '8', '1', '5', '4', '9', '7', '1', '9', '1', '5', '1', '0', '9', '4', '4', '1', '4', '6', '2', '9', '7', '4', '4', '2', '2', '2', '7', '3', '9', '6', '6', '5', '4', '4', '8', '1', '6', '9', '9', '7', '3', '4', '6', '8', '5', '3', '9', '3', '1', '1', '9', '8', '7', '9', '3', '5', '1', '6', '0', '1', '6', '0', '6', '6', '9', '7', '9', '6', '2', '2', '2', '0', '8', '4', '6', '3', '1', '1', '1', '6', '7', '6', '5', '0', '2', '2', '5', '9', '7', '8', '2', '3', '3', '1', '9', '0', '2', '3', '2', '5', '8', '8', '6', '7', '4', '7', '1', '8', '4', '4', '7', '0', '9', '4', '6', '0', '5', '1', '5', '7', '3', '8', '3', '3', '7', '2', '4', '9', '7', '7', '6', '4', '9', '6', '9', '8', '1', '6', '6', '3', '5', '0', '0', '2', '4', '5', '6', '5', '7', '2', '2', '9', '0', '9', '3', '4', '9', '1', '0', '8', '9', '1', '2', '2', '9', '3', '0', '9', '3', '9', '5', '9', '5', '4', '8', '9', '5', '7', '8', '5', '5', '3', '2', '8', '4', '4', '6', '8', '2', '2', '3', '7', '7', '8', '3', '8', '0', '8', '5', '0', '8', '0', '2', '7', '0', '9', '1', '8', '2', '4', '3', '9', '4', '2', '8', '2', '2', '1', '3', '7', '6', '1', '4', '6', '0', '8', '3', '3', '6', '1', '5', '0', '1', '5', '9', '4', '1', '5', '0', '2', '8', '6', '9', '9', '8', '6', '2', '7', '4', '5', '8', '9', '4', '7', '3', '7', '0', '3', '6', '8', '4', '1', '3', '1', '0', '6', '3', '9', '6', '2', '5', '3', '3', '2', '5', '5', '6', '7', '5', '6', '9', '8', '6', '3', '5', '4', '2', '5', '8', '4', '8', '5', '6', '9', '7', '9', '6', '5', '2', '2', '5', '4', '0', '4', '6', '3', '6', '1', '2', '1', '2', '2', '0', '7', '6', '7', '4', '4', '5', '2', '2', '9', '0', '4', '5', '7', '0', '8', '6', '0', '4', '3', '6', '1', '0', '5', '2', '6', '5', '5', '1', '7', '3', '3', '8', '1', '6', '6', '1', '3', '5', '1', '4', '5', '6', '2', '7', '8', '3', '9', '6', '9', '4', '4', '5', '5', '9', '6', '2', '5', '7', '0', '0', '5', '1', '9', '3', '3', '5', '8', '9', '9', '5', '0', '3', '3', '1', '2', '7', '6', '2', '6', '8', '4', '2', '3', '0', '0', '4', '7', '1', '8', '1', '9', '0', '7', '3', '5', '7', '5', '2', '1', '1', '9', '7', '3', '3', '3', '9', '6', '7', '7', '3', '1', '8', '2', '4', '8', '0', '1', '0', '9', '1', '6', '1', '3', '5', '5', '5', '5', '0', '0', '3', '2', '7', '4', '7', '4', '4', '8', '1', '1', '9', '1', '4', '4', '6', '2', '1', '2', '1', '6', '2', '0', '4', '5', '6', '9', '2', '1', '6', '6', '4', '9', '8', '8', '1', '2', '8', '1', '6', '4', '4', '5', '2', '4', '7', '6', '9', '9', '7', '2', '9', '0', '8', '6', '3', '6', '8', '3', '6', '5', '7', '4', '6', '3', '8', '3', '0', '9', '6', '0', '6', '6', '5', '6', '3', '6', '5', '8', '0', '4', '2', '5', '7', '8', '2', '1', '7', '5', '6', '5', '8', '2', '6', '4', '9', '3', '1', '9', '6', '1', '9', '6', '0', '0', '6', '2', '1', '8', '9', '2', '6', '8', '9', '1', '3', '2', '1', '1', '7', '0', '7', '1', '6', '4', '1', '8', '1', '4', '2', '8', '1', '4', '8', '9', '1', '4', '1', '7', '1', '6', '5', '3', '4', '6', '6', '8', '3', '1', '6', '4', '6', '6', '5', '9', '0', '0', '0', '9', '8', '8', '2', '8', '4', '8', '4', '9', '5', '0', '6', '2', '4', '5', '5', '1', '3', '3', '4', '8', '3', '3', '0', '0', '7', '0', '9', '5', '0', '1', '3', '2', '2', '7', '2', '9', '6', '4', '1', '1', '3', '7', '1', '8', '2', '9', '3', '4', '5', '5', '9', '5', '0', '5', '1', '5', '8', '0', '0', '7', '0', '4', '1', '8', '7', '3', '9', '4', '3', '1', '9', '2', '3', '9', '7', '9', '7', '4', '0', '0', '7', '5', '4', '5', '5', '9', '9', '3', '6', '4', '5', '1', '6', '8', '4', '6', '6', '8', '6', '1', '6', '5', '0', '2', '1', '0', '9', '2', '4', '5', '2', '0', '7', '8', '8', '1', '9', '2', '8', '6', '8', '4', '2', '7', '7', '7', '6', '8', '2', '1', '9', '8', '2', '4', '8', '3', '1', '6', '4', '3', '5', '0', '4', '9', '3', '6', '8', '3', '9', '1', '1', '9', '7', '2', '2', '7', '4', '1', '7', '3', '5', '0', '0', '9', '3', '8', '3', '0', '9', '4', '2', '9', '9', '2', '2', '2', '2', '5', '3', '9', '4', '2', '7', '5', '3', '8', '2', '6', '6', '6', '4', '7', '2', '8', '3', '2', '5', '5', '8', '3', '8', '6', '1', '0', '2', '8', '4', '6', '0', '5', '6', '2', '9', '4', '6', '0', '5', '9', '5', '0', '6', '2', '9', '0', '2', '4', '1', '3', '0', '0', '8', '4', '5', '0', '3', '4', '0', '1', '5', '4', '7', '8', '3', '0', '6', '8', '5', '3', '6', '4', '8', '6', '1', '0', '7', '0', '7', '3', '6', '8', '4', '9', '0', '0', '0', '3', '7', '8', '4', '5', '1', '1', '4', '6', '0', '6', '7', '2', '4', '7', '7', '4', '3', '3', '5', '7', '9', '6', '7', '1', '9', '3', '8', '3', '2', '7', '0', '2', '0', '3', '1', '5', '5', '4', '2', '9', '3', '8', '5', '8', '4', '2', '7', '9', '4', '2', '7', '9', '4', '9', '5', '1', '7', '2', '6', '8', '1', '8', '3', '0', '3', '0', '0', '4', '6', '5', '1', '7', '3', '6', '3', '1', '2', '8', '8', '2', '0', '1', '6', '8', '8', '3', '4', '8', '9', '7', '7', '3', '4', '2', '6', '3', '4', '6', '4', '0', '3', '2', '9', '7', '5', '5', '6', '4', '9', '3', '8', '4', '9', '6', '9', '0', '5', '3', '9', '9', '3', '5', '8', '8', '2', '8', '2', '8', '7', '9', '4', '9', '2', '7', '0', '9', '9', '8', '1', '4', '3', '3', '8', '6', '5', '5', '3', '7', '2', '5', '7', '2', '2', '5', '5', '3', '1', '4', '8', '9', '6', '4', '1', '3', '7', '8', '9', '4', '9', '9', '1', '0', '2', '5', '0', '8', '7', '0', '3', '8', '1', '5', '0', '0', '2', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '5', '1', '0', '3', '1', '4', '7', '8', '7', '7', '8', '4', '2', '4', '6', '0', '5', '5', '8', '4', '6', '4', '6', '5', '0', '6', '7', '1', '1', '1', '2', '6', '7', '3', '8', '8', '5', '6', '2', '3', '1', '6', '6', '6', '1', '3', '1', '0', '2', '8', '3', '9', '7', '3', '9', '6', '0', '9', '1', '3', '9', '8', '4', '3', '2', '8', '6', '0', '5', '4', '7', '5', '6', '9', '5', '8', '0', '0', '0', '1', '2', '2', '9', '3', '9', '7', '5', '9', '8', '1', '9', '6', '2', '4', '5', '4', '3', '5', '5', '4', '6', '2', '8', '4', '8', '1', '7', '2', '0', '8', '0', '7', '4', '9', '9', '2', '2', '4', '9', '6', '4', '4', '4', '3', '0', '0', '8', '8', '7', '0', '4', '5', '5', '9', '3', '9', '9', '6', '9', '9', '9', '5', '3', '1', '7', '4', '4', '2', '6', '8', '3', '5', '8', '6', '7', '2', '5', '2', '2', '4', '7', '9', '2', '9', '0', '2', '0', '3', '7', '6', '4', '6', '0', '5', '4', '9', '1', '9', '3', '9', '9', '7', '6', '4', '9', '3', '5', '5', '4', '4', '3', '9', '0', '4', '8', '1', '3', '4', '7', '0', '3', '2', '3', '4', '0', '5', '1', '3', '2', '8', '2', '9', '6', '2', '1', '9', '7', '7', '1', '2', '2', '5', '4', '5', '9', '1', '8', '3', '9', '8', '1', '0', '4', '3', '5', '6', '2', '2', '5', '6', '1', '4', '9', '2', '8', '5', '2', '9', '0', '6', '4', '9', '3', '3', '6', '3', '1', '3', '5', '3', '3', '2', '2', '2', '4', '3', '4', '6', '9', '7', '2', '2', '5', '6', '4', '3', '6', '8', '8', '9', '5', '8', '5', '5', '5', '1', '6', '8', '8', '4', '2', '8', '1', '3', '1', '7', '1', '1', '5', '4', '8', '2', '8', '5', '6', '7', '1', '0', '4', '9', '2', '3', '2', '3', '6', '2', '2', '2', '4', '4', '2', '7', '0', '5', '4', '0', '0', '5', '5', '9', '7', '9', '1', '8', '7', '2', '1', '2', '4', '6', '4', '7', '0', '1', '8', '9', '6', '9', '1', '1', '5', '6', '5', '9', '3', '7', '8', '0', '5', '9', '0', '6', '6', '8', '7', '6', '6', '1', '2', '2', '8', '9', '1', '7', '3', '2', '4', '2', '6', '1', '1', '5', '0', '4', '1', '2', '1', '3', '1', '4', '9', '3', '7', '8', '1', '7', '8', '9', '0', '6', '1', '2', '8', '3', '9', '5', '7', '3', '5', '4', '4', '8', '7', '4', '7', '9', '8', '2', '5', '2', '9', '5', '2', '9', '9', '1', '8', '5', '1', '1', '3', '0', '3', '4', '5', '6', '4', '6', '8', '6', '5', '0', '5', '3', '9', '7', '3', '3', '6', '5', '0', '9', '8', '1', '5', '6', '9', '5', '9', '7', '8', '5', '9', '5', '7', '6', '0', '6', '2', '5', '3', '8', '3', '8', '8', '2', '9', '3', '8', '7', '9', '6', '3', '1', '7', '8', '3', '8', '8', '1', '3', '3', '0', '1', '7', '2', '5', '7', '4', '1', '7', '8', '3', '6', '1', '6', '6', '9', '0', '0', '6', '4', '9', '6', '8', '5', '5', '2', '7', '2', '5', '9', '1', '0', '2', '6', '2', '7', '4', '8', '1', '9', '9', '9', '4', '8', '9', '5', '0', '9', '9', '9', '3', '2', '9', '4', '4', '3', '9', '0', '8', '0', '6', '4', '1', '1', '7', '4', '1', '8', '1', '6', '6', '9', '6', '2', '3', '7', '2', '8', '8', '6', '8', '0', '6', '0', '7', '9', '5', '5', '0', '8', '3', '7', '4', '5', '6', '3', '0', '1', '0', '5', '0', '3', '4', '4', '0', '6', '8', '6', '4', '8', '8', '5', '4', '1', '0', '6', '0', '3', '2', '5', '5', '1', '7', '3', '9', '1', '1', '3', '6', '1', '4', '3', '2', '6', '4', '9', '1', '6', '8', '5', '7', '0', '8', '6', '1', '3', '6', '6', '3', '1', '5', '7', '6', '1', '0', '6', '5', '0', '3', '9', '4', '3', '8', '9', '4', '2', '9', '7', '5', '7', '5', '2', '3', '1', '5', '1', '8', '2', '7', '3', '1', '6', '6', '2', '6', '6', '8', '4', '5', '3', '8', '0', '6', '3', '2', '5', '2', '5', '2', '7', '5', '6', '1', '8', '6', '1', '3', '6', '0', '2', '0', '3', '6', '9', '7', '7', '1', '0', '2', '5', '4', '1', '3', '7', '8', '5', '9', '8', '6', '6', '2', '5', '5', '5', '0', '7', '5', '9', '7', '6', '4', '8', '3', '9', '9', '2', '7', '6', '3', '5', '8', '4', '1', '9', '7', '3', '7', '2', '6', '5', '7', '5', '7', '3', '1', '9', '1', '0', '3', '6', '7', '1', '6', '5', '4', '1', '8', '1', '5', '0', '1', '1', '1', '9', '3', '8', '7', '0', '6', '8', '3', '7', '5', '5', '8', '9', '8', '5', '6', '2', '6', '0', '6', '2', '9', '4', '6', '5', '8', '6', '3', '7', '8', '8', '2', '9', '0', '7', '6', '4', '4', '6', '8', '6', '6', '6', '6', '7', '3', '2', '8', '7', '6', '0', '9', '4', '3', '7', '6', '8', '2', '8', '2', '9', '3', '9', '3', '0', '9', '5', '9', '4', '7', '8', '2', '5', '9', '7', '3', '6', '0', '1', '4', '2', '6', '9', '8', '2', '8', '8', '5', '5', '1', '8', '7', '0', '0', '2', '8', '5', '2', '3', '1', '1', '6', '8', '9', '6', '0', '5', '8', '9', '8', '5', '1', '8', '3', '1', '1', '8', '5', '6', '7', '5', '1', '1', '8', '0', '7', '3', '1', '0', '7', '7', '3', '2', '7', '9', '5', '7', '8', '7', '8', '6', '8', '0', '9', '1', '5', '7', '3', '7', '5', '3', '9', '9', '8', '2', '2', '5', '6', '9', '0', '5', '5', '2', '6', '1', '8', '8', '9', '4', '3', '0', '0', '8', '7', '0', '5', '8', '7', '1', '4', '5', '7', '8', '8', '5', '8', '9', '6', '9', '2', '2', '4', '1', '5', '0', '7', '0', '3', '7', '0', '1', '0', '7', '0', '5', '5', '1', '8', '4', '0', '4', '0', '0', '0', '5', '1', '1', '0', '8', '0', '4', '3', '3', '6', '8', '0', '0', '4', '1', '6', '5', '3', '4', '4', '4', '1', '3', '8', '0', '4', '6', '9', '7', '2', '6', '0', '1', '4', '6', '8', '6', '2', '7', '0', '9', '4', '1', '4', '6', '6', '9', '8', '2', '4', '6', '5', '1', '7', '0', '5', '3', '8', '2', '5', '7', '4', '1', '2', '4', '8', '7', '7', '4', '5', '6', '7', '1', '9', '2', '8', '9', '3', '8', '2', '6', '9', '7', '9', '6', '3', '2', '5', '4', '7', '0', '4', '7', '8', '3', '6', '0', '5', '9', '2', '0', '6', '1', '9', '8', '1', '4', '8', '0', '3', '2', '2', '7', '8', '2', '7', '5', '4', '2', '7', '2', '4', '6', '1', '7', '3', '6', '8', '5', '7', '9', '1', '8', '5', '8', '4', '0', '5', '9', '7', '5', '4', '4', '7', '0', '3', '6', '2', '8', '9', '7', '3', '3', '1', '5', '5', '9', '8', '5', '4', '9', '7', '6', '9', '4', '6', '0', '1', '1', '7', '3', '6', '8', '3', '9', '1', '2', '1', '4', '3', '3', '2', '0', '6', '7', '4', '2', '9', '9', '9', '2', '2', '4', '3', '2', '0', '6', '5', '6', '6', '9', '7', '2', '4', '7', '0', '4', '1', '3', '8', '3', '2', '4', '1', '8', '5', '3', '6', '0', '1', '0', '2', '2', '4', '0', '2', '9', '3', '2', '3', '8', '4', '0', '1', '0', '6', '9', '8', '2', '6', '6', '7', '9', '2', '4', '6', '7', '0', '2', '7', '1', '2', '7', '2', '6', '5', '3', '3', '4', '9', '7', '7', '4', '9', '1', '3', '0', '5', '0', '8', '3', '1', '0', '4', '1', '5', '0', '3', '7', '1', '4', '3', '6', '4', '2', '1', '1', '1', '1', '6', '0', '2', '0', '6', '1', '9', '5', '1', '3', '4', '7', '7', '7', '8', '5', '8', '3', '0', '0', '7', '5', '0', '7', '2', '0', '0', '3', '8', '2', '5', '3', '7', '9', '5', '7', '2', '5', '9', '6', '7', '5', '5', '7', '8', '0', '0', '7', '6', '8', '9', '2', '8', '3', '4', '7', '7', '0', '8', '2', '4', '8', '2', '0', '1', '7', '5', '9', '8', '7', '3', '7', '5', '3', '6', '8', '2', '8', '3', '0', '6', '2', '3', '9', '5', '6', '2', '8', '0', '9', '8', '0', '1', '9', '8', '5', '6', '7', '7', '3', '3', '7', '0', '3', '0', '6', '4', '8', '3', '3', '2', '4', '3', '0', '9', '9', '2', '3', '3', '5', '5', '0', '1', '6', '6', '6', '3', '1', '7', '8', '4', '4', '1', '8', '4', '3', '4', '8', '5', '8', '9', '0', '3', '8', '2', '1', '2', '1', '4', '8', '1', '8', '9', '8', '9', '6', '2', '5', '5', '4', '8', '0', '2', '6', '1', '5', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '5', '7', '6', '2', '3', '1', '2', '6', '5', '8', '0', '5', '2', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '1', '6', '2', '1', '6', '5', '4', '1', '0', '7', '6', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '8', '6', '0', '5', '4', '3', '1', '6', '2', '3', '2', '9', '9', '9', '9', '9', '6', '7', '5', '9', '9', '9', '4', '5', '6', '3', '6', '9', '3', '5', '9', '9', '9', '9', '9', '9', '5', '5', '0', '6', '1', '0', '6', '5', '2', '6', '7', '2', '1', '2', '9', '9', '9', '6', '4', '3', '5', '4', '9', '9', '4', '3', '3', '0', '9', '5', '2', '5', '2', '3', '5', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '9', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '3', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '4', '7', '3', '1', '2', '7', '0', '3', '5', '7', '7', '5', '4', '7', '7', '6', '8', '0', '8', '2', '4', '5', '3', '8', '1', '1', '1', '2', '6', '3', '9', '1', '5', '1', '4', '9', '3', '6', '1', '0', '1', '1', '6', '0', '5', '8', '1', '1', '6', '6', '7', '0', '5', '9', '1', '3', '2', '4', '0', '8', '2', '2', '1', '4', '2', '4', '5', '1', '6', '2', '3', '9', '6', '9', '6', '8', '1', '6', '1', '9', '8', '0', '2', '2', '9', '6', '2', '4', '7', '9', '1', '1', '6', '9', '3', '7', '7', '7', '9', '1', '9', '4', '8', '5', '8', '8', '6', '1', '3', '6', 0};

	tau->num1 = bi_new((const char[]){'1', '8', '2', '2', '9', '2', '6', '1', '7', '2', '9', '5', '6', '2', '3', '8', '1', '9', '9', '9', '9', '4', '3', '1', '9', '5', '3', '6', '4', '1', '5', '0', '7', '9', '1', '1', '4', '1', '3', '1', '0', '8', '4', '2', '4', '6', '2', '6', '6', '0', '6', '5', '8', '4', '7', '2', '7', '9', '4', '0', '5', '2', '5', '1', '9', '5', '9', '0', '8', '4', '7', '5', '6', '5', '7', '8', '9', '3', '4', '9', '4', '0', '2', '7', '1', '8', '3', '2', '4', '0', '0', '4', '8', '3', '9', '8', '5', '7', '1', '4', '2', '9', '2', '8', '2', '1', '2', '6', '2', '0', '4', '0', '3', '2', '0', '2', '7', '7', '7', '7', '1', '3', '7', '8', '3', '6', '0', '4', '3', '6', '6', '2', '0', '2', '0', '7', '0', '7', '5', '9', '5', '5', '5', '6', '2', '6', '4', '0', '1', '8', '5', '2', '5', '8', '8', '0', '7', '8', '4', '4', '0', '6', '9', '1', '8', '2', '9', '0', '6', '4', '1', '2', '4', '9', '5', '1', '5', '0', '8', '2', '1', '8', '9', '2', '9', '8', '5', '5', '9', '1', '4', '9', '1', '7', '6', '1', '8', '4', '5', '0', '2', '8', '0', '8', '4', '8', '9', '1', '2', '0', '0', '7', '2', '8', '4', '4', '9', '9', '2', '6', '8', '7', '3', '9', '2', '8', '0', '7', '2', '8', '7', '7', '7', '6', '7', '3', '5', '9', '7', '1', '4', '1', '8', '3', '4', '7', '2', '7', '0', '2', '6', '1', '8', '9', '6', '3', '7', '5', '0', '1', '4', '9', '7', '1', '8', '2', '4', '6', '9', '1', '1', '6', '5', '0', '7', '7', '6', '1', '3', '3', '7', '9', '8', '5', '9', '0', '9', '5', '7', '0', '0', '0', '9', '7', '3', '3', '0', '4', '5', '9', '7', '4', '8', '8', '0', '8', '4', '2', '8', '4', '0', '1', '7', '9', '7', '4', '2', '9', '1', '0', '0', '6', '4', '2', '4', '5', '8', '6', '9', '1', '8', '1', '7', '1', '9', '5', '1', '1', '8', '7', '4', '6', '1', '2', '1', '5', '1', '5', '1', '7', '2', '6', '5', '4', '6', '3', '2', '2', '8', '2', '2', '1', '6', '8', '6', '9', '9', '8', '7', '5', '4', '9', '1', '8', '2', '4', '2', '2', '4', '3', '3', '6', '3', '7', '2', '5', '9', '0', '8', '5', '1', '4', '1', '8', '6', '5', '4', '6', '2', '0', '4', '3', '5', '7', '6', '7', '9', '8', '4', '2', '3', '3', '8', '7', '1', '8', '4', '7', '7', '4', '4', '4', '7', '9', '2', '0', '7', '3', '9', '9', '3', '4', '2', '3', '6', '5', '8', '4', '8', '2', '3', '8', '2', '4', '2', '8', '1', '1', '9', '8', '1', '6', '3', '8', '1', '5', '0', '1', '0', '6', '7', '4', '8', '1', '0', '4', '5', '1', '6', '6', '0', '3', '7', '7', '3', '0', '6', '0', '5', '6', '2', '0', '1', '6', '1', '9', '6', '7', '6', '2', '5', '6', '1', '3', '3', '8', '4', '4', '1', '4', '3', '6', '0', '3', '8', '3', '3', '9', '0', '4', '4', '1', '4', '9', '5', '2', '6', '3', '4', '4', '3', '2', '1', '9', '0', '1', '1', '4', '6', '5', '7', '5', '4', '4', '4', '5', '4', '1', '7', '8', '4', '2', '4', '0', '2', '0', '9', '2', '4', '6', '1', '6', '5', '1', '5', '7', '2', '3', '3', '5', '0', '7', '7', '8', '7', '0', '7', '7', '4', '9', '8', '1', '7', '1', '2', '5', '7', '7', '2', '4', '6', '7', '9', '6', '2', '9', '2', '6', '3', '8', '6', '3', '5', '6', '3', '7', '3', '2', '8', '9', '9', '1', '2', '1', '5', '4', '8', '3', '1', '4', '3', '8', '1', '6', '7', '8', '9', '9', '8', '8', '5', '0', '4', '0', '4', '4', '5', '3', '6', '4', '0', '2', '3', '5', '2', '7', '3', '8', '1', '9', '5', '1', '3', '7', '8', '6', '3', '6', '5', '6', '4', '3', '9', '1', '2', '1', '2', '0', '1', '0', '3', '9', '7', '1', '2', '2', '8', '2', '2', '1', '2', '0', '7', '2', '0', '3', '5', '7', '4', '7', '7', '2', '3', '2', '0', '2', '9', '0', '9', '3', '3', '6', '5', '1', '6', '5', '4', '2', '7', '3', '6', '3', '6', '9', '2', '3', '9', '5', '3', '4', '5', '6', '3', '6', '1', '9', '7', '7', '4', '4', '1', '8', '0', '2', '4', '5', '4', '1', '9', '3', '5', '2', '6', '8', '2', '3', '8', '2', '4', '0', '1', '3', '8', '0', '1', '0', '9', '3', '1', '1', '0', '6', '8', '2', '7', '1', '2', '8', '4', '6', '3', '4', '3', '3', '6', '3', '8', '7', '0', '6', '9', '5', '3', '5', '8', '7', '7', '7', '4', '8', '5', '8', '2', '9', '3', '6', '1', '8', '7', '9', '3', '7', '4', '7', '2', '4', '7', '8', '5', '3', '0', '0', '7', '3', '3', '7', '9', '5', '1', '5', '1', '1', '0', '0', '6', '3', '0', '6', '7', '6', '8', '6', '9', '6', '2', '0', '5', '9', '0', '2', '3', '2', '1', '9', '1', '2', '2', '7', '1', '6', '5', '9', '7', '4', '3', '7', '3', '8', '7', '9', '3', '8', '5', '1', '6', '0', '5', '7', '4', '0', '5', '8', '4', '3', '1', '9', '1', '3', '3', '3', '6', '6', '0', '9', '9', '8', '5', '8', '3', '3', '3', '2', '8', '4', '8', '8', '7', '9', '7', '2', '9', '2', '5', '2', '0', '9', '5', '1', '9', '6', '2', '2', '8', '0', '5', '7', '7', '7', '8', '0', '8', '9', '7', '9', '1', '0', '1', '8', '0', '3', '5', '9', '4', '5', '0', '9', '1', '0', '5', '8', '5', '2', '5', '9', '0', '1', '5', '6', '3', '7', '7', '4', '6', '8', '5', '1', '2', '8', '4', '2', '7', '0', '4', '0', '6', '6', '3', '8', '4', '6', '2', '9', '0', '4', '1', '6', '2', '0', '5', '1', '0', '8', '2', '8', '6', '7', '3', '6', '9', '0', '6', '3', '4', '2', '0', '8', '6', '4', '4', '5', '2', '4', '7', '1', '3', '4', '6', '7', '1', '2', '4', '0', '0', '3', '9', '5', '4', '7', '5', '8', '7', '6', '6', '5', '9', '1', '7', '8', '8', '0', '4', '0', '7', '1', '1', '9', '5', '6', '6', '2', '3', '2', '5', '2', '2', '2', '9', '6', '0', '7', '4', '9', '0', '0', '1', '5', '8', '6', '0', '7', '4', '5', '9', '2', '2', '7', '9', '9', '0', '6', '3', '1', '5', '4', '1', '0', '5', '9', '3', '3', '8', '7', '1', '3', '9', '6', '5', '1', '4', '3', '2', '6', '3', '3', '0', '2', '7', '7', '1', '2', '8', '7', '8', '6', '4', '4', '8', '2', '1', '3', '6', '8', '4', '1', '6', '3', '1', '3', '1', '6', '9', '9', '6', '5', '5', '3', '5', '2', '7', '4', '0', '2', '7', '7', '9', '6', '0', '7', '1', '2', '8', '2', '7', '0', '8', '0', '2', '8', '2', '0', '6', '4', '3', '2', '5', '7', '2', '2', '8', '7', '2', '1', '6', '9', '0', '0', '8', '2', '7', '7', '0', '7', '5', '9', '3', '7', '1', '3', '4', '0', '9', '2', '4', '9', '6', '1', '6', '7', '5', '8', '0', '3', '2', '1', '3', '8', '5', '8', '2', '0', '1', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '8', '9', '6', '8', '5', '2', '1', '2', '2', '1', '5', '9', '7', '4', '8', '0', '4', '0', '8', '2', '6', '3', '6', '6', '9', '8', '2', '3', '3', '5', '8', '3', '5', '3', '5', '6', '4', '2', '7', '8', '2', '5', '2', '4', '4', '6', '4', '8', '5', '9', '0', '7', '7', '7', '6', '2', '9', '6', '7', '7', '4', '2', '7', '5', '3', '7', '1', '6', '1', '6', '0', '6', '3', '9', '7', '6', '5', '2', '6', '6', '7', '8', '2', '4', '1', '4', '8', '5', '8', '4', '0', '7', '4', '2', '3', '8', '7', '5', '9', '1', '8', '3', '3', '1', '5', '8', '6', '2', '9', '5', '1', '7', '4', '7', '1', '4', '0', '4', '8', '9', '6', '3', '3', '1', '5', '4', '3', '4', '9', '9', '1', '6', '6', '0', '0', '9', '0', '4', '4', '1', '9', '1', '6', '7', '4', '6', '0', '8', '6', '6', '6', '1', '3', '1', '8', '6', '5', '2', '7', '1', '3', '1', '6', '0', '9', '8', '0', '9', '4', '0', '2', '6', '8', '8', '8', '8', '8', '9', '5', '9', '3', '3', '8', '8', '6', '6', '1', '4', '4', '5', '4', '6', '7', '2', '9', '2', '1', '6', '5', '9', '1', '1', '3', '5', '4', '1', '6', '3', '5', '1', '4', '7', '0', '3', '0', '7', '8', '1', '6', '6', '5', '5', '4', '6', '5', '6', '0', '9', '2', '4', '5', '4', '8', '2', '6', '9', '7', '9', '4', '6', '3', '8', '9', '1', '7', '4', '7', '2', '8', '5', '2', '0', '2', '9', '2', '9', '5', '6', '6', '4', '5', '4', '9', '2', '0', '9', '9', '1', '5', '3', '2', '5', '7', '5', '7', '3', '1', '1', '7', '8', '5', '4', '6', '9', '6', '9', '7', '1', '4', '4', '5', '6', '0', '5', '9', '1', '7', '4', '9', '9', '2', '2', '6', '1', '0', '5', '1', '8', '7', '8', '5', '5', '7', '1', '4', '2', '5', '5', '4', '1', '8', '7', '4', '7', '6', '3', '1', '2', '7', '2', '9', '7', '6', '7', '3', '7', '6', '1', '7', '0', '8', '3', '9', '1', '7', '5', '9', '3', '1', '9', '2', '3', '3', '4', '9', '7', '6', '4', '4', '6', '1', '1', '9', '3', '8', '9', '1', '2', '7', '5', '1', '5', '7', '1', '6', '3', '6', '1', '9', '3', '7', '4', '7', '2', '6', '6', '9', '5', '6', '3', '7', '6', '8', '4', '4', '6', '1', '8', '7', '4', '7', '0', '7', '7', '9', '5', '0', '1', '4', '9', '4', '6', '0', '0', '7', '8', '6', '5', '7', '8', '5', '1', '9', '2', '9', '6', '6', '9', '4', '0', '3', '7', '3', '0', '7', '2', '7', '8', '9', '1', '1', '3', '5', '4', '7', '5', '3', '4', '5', '4', '2', '5', '8', '3', '5', '9', '4', '8', '6', '1', '8', '9', '0', '7', '7', '0', '0', '2', '1', '6', '9', '8', '9', '9', '9', '2', '1', '7', '5', '7', '1', '2', '0', '0', '0', '5', '6', '9', '0', '2', '0', '8', '7', '0', '3', '8', '6', '6', '0', '1', '2', '8', '2', '9', '1', '4', '2', '3', '2', '5', '0', '9', '4', '2', '6', '2', '3', '8', '5', '4', '2', '0', '6', '8', '9', '7', '2', '3', '3', '1', '4', '1', '8', '7', '1', '1', '6', '3', '3', '0', '0', '3', '4', '8', '7', '1', '5', '7', '5', '1', '9', '5', '4', '9', '1', '6', '3', '7', '9', '8', '7', '4', '9', '6', '6', '7', '8', '1', '4', '8', '0', '4', '6', '5', '8', '2', '6', '6', '9', '6', '8', '7', '6', '5', '0', '6', '1', '0', '7', '3', '5', '3', '8', '3', '5', '3', '4', '6', '3', '0', '3', '2', '2', '4', '1', '5', '1', '7', '5', '8', '8', '2', '4', '3', '0', '6', '6', '2', '7', '8', '5', '1', '4', '1', '3', '1', '9', '8', '1', '8', '1', '6', '0', '6', '7', '3', '3', '1', '9', '4', '2', '8', '3', '9', '9', '2', '1', '1', '8', '0', '8', '9', '5', '5', '1', '6', '5', '9', '3', '4', '9', '1', '9', '9', '6', '2', '8', '1', '8', '0', '2', '2', '3', '5', '9', '0', '1', '1', '0', '4', '7', '4', '9', '1', '6', '3', '0', '3', '5', '5', '7', '9', '2', '6', '8', '3', '4', '0', '6', '4', '8', '3', '6', '9', '8', '5', '0', '6', '7', '5', '2', '3', '6', '3', '9', '9', '1', '7', '9', '1', '1', '5', '2', '2', '2', '7', '0', '1', '8', '0', '1', '2', '9', '3', '7', '8', '5', '0', '1', '4', '1', '9', '3', '5', '8', '0', '4', '0', '5', '1', '2', '0', '2', '0', '4', '5', '8', '6', '7', '4', '1', '0', '6', '1', '2', '3', '5', '9', '6', '2', '7', '6', '6', '5', '8', '3', '9', '0', '7', '0', '9', '4', '0', '2', '1', '8', '7', '9', '2', '1', '5', '1', '7', '1', '4', '8', '3', '1', '1', '9', '1', '3', '9', '8', '9', '4', '8', '7', '0', '1', '3', '3', '0', '9', '1', '1', '1', '1', '0', '4', '4', '9', '0', '1', '6', '8', '3', '4', '0', '0', '9', '4', '9', '4', '8', '3', '8', '4', '6', '8', '1', '8', '2', '9', '9', '5', '1', '8', '0', '4', '1', '7', '6', '3', '5', '0', '7', '9', '4', '8', '9', '2', '2', '5', '9', '0', '7', '7', '4', '9', '2', '5', '4', '6', '6', '0', '8', '8', '1', '7', '1', '8', '7', '9', '2', '5', '9', '4', '6', '5', '9', '2', '1', '0', '2', '6', '5', '9', '7', '0', '4', '6', '7', '0', '0', '4', '4', '9', '8', '1', '9', '8', '9', '9', '0', '9', '6', '8', '6', '2', '0', '3', '9', '4', '6', '0', '0', '1', '7', '7', '4', '3', '0', '9', '4', '4', '7', '3', '8', '1', '1', '0', '5', '6', '9', '9', '1', '2', '9', '4', '1', '2', '8', '5', '4', '2', '8', '9', '1', '8', '8', '0', '8', '5', '5', '3', '6', '2', '7', '0', '7', '4', '0', '7', '6', '7', '0', '7', '2', '2', '5', '9', '3', '7', '3', '7', '7', '7', '2', '6', '6', '6', '9', '7', '3', '4', '4', '0', '9', '7', '7', '3', '6', '1', '2', '4', '3', '3', '3', '6', '3', '9', '7', '3', '0', '8', '0', '5', '1', '7', '6', '3', '0', '9', '1', '5', '0', '6', '8', '3', '6', '3', '1', '0', '7', '9', '5', '3', '1', '2', '6', '0', '7', '2', '3', '9', '5', '2', '0', '3', '6', '5', '2', '9', '0', '0', '3', '2', '1', '0', '5', '8', '4', '8', '8', '3', '9', '5', '0', '7', '9', '8', '1', '4', '5', '2', '3', '0', '7', '2', '9', '9', '4', '1', '7', '1', '8', '5', '7', '1', '5', '7', '9', '6', '2', '9', '7', '4', '5', '4', '9', '9', '5', '0', '2', '3', '5', '0', '5', '3', '1', '6', '0', '4', '0', '9', '1', '9', '8', '5', '9', '1', '9', '3', '7', '1', '8', '0', '2', '3', '3', '0', '7', '4', '1', '4', '8', '8', '0', '4', '4', '6', '2', '1', '7', '9', '2', '2', '8', '0', '0', '8', '3', '1', '7', '6', '6', '0', '4', '0', '9', '3', '8', '6', '5', '6', '3', '4', '4', '5', '7', '1', '0', '3', '4', '7', '7', '8', '5', '5', '3', '4', '5', '7', '1', '2', '1', '0', '8', '0', '5', '3', '0', '7', '3', '6', '3', '9', '4', '5', '3', '5', '9', '2', '3', '9', '3', '2', '6', '5', '1', '8', '6', '6', '0', '3', '0', '5', '1', '5', '0', '4', '1', '0', '6', '0', '9', '6', '6', '4', '3', '7', '3', '1', '3', '3', '2', '3', '6', '7', '2', '8', '3', '1', '5', '3', '9', '3', '2', '3', '5', '0', '0', '0', '6', '7', '9', '3', '7', '1', '0', '7', '5', '4', '1', '9', '5', '5', '4', '3', '7', '3', '6', '2', '4', '3', '3', '2', '4', '8', '3', '6', '1', '2', '4', '2', '5', '2', '5', '9', '4', '5', '8', '6', '8', '8', '0', '2', '3', '5', '3', '9', '1', '6', '7', '6', '6', '1', '8', '1', '5', '3', '2', '3', '7', '5', '8', '5', '5', '5', '0', '4', '8', '8', '6', '9', '0', '1', '4', '3', '2', '2', '2', '1', '3', '4', '9', '7', '3', '3', '3', '5', '4', '0', '9', '7', '3', '9', '1', '1', '5', '6', '0', '2', '1', '0', '7', '5', '9', '4', '1', '9', '0', '7', '6', '5', '8', '8', '5', '8', '0', '1', '4', '3', '2', '2', '6', '6', '2', '9', '6', '9', '3', '5', '1', '6', '6', '7', '5', '6', '9', '0', '0', '7', '6', '6', '4', '4', '9', '8', '3', '3', '3', '6', '8', '2', '1', '5', '5', '8', '1', '5', '6', '5', '1', '4', '1', '0', '9', '6', '1', '7', '8', '7', '8', '5', '1', '8', '1', '0', '1', '0', '3', '7', '4', '4', '5', '1', '9', '7', '3', '8', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '2', '3', '7', '6', '8', '7', '3', '4', '1', '9', '4', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8', '3', '7', '8', '3', '4', '5', '8', '9', '2', '3', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '3', '9', '4', '5', '6', '8', '3', '7', '6', '7', '0', '0', '0', '0', '0', '3', '2', '4', '0', '0', '0', '5', '4', '3', '6', '3', '0', '6', '4', '0', '0', '0', '0', '0', '0', '4', '4', '9', '3', '8', '9', '3', '4', '7', '3', '2', '7', '8', '7', '0', '0', '0', '3', '5', '6', '4', '5', '0', '0', '5', '6', '6', '9', '0', '4', '7', '4', '7', '6', '4', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '5', '2', '6', '8', '7', '2', '9', '6', '4', '2', '2', '4', '5', '2', '2', '3', '1', '9', '1', '7', '5', '4', '6', '1', '8', '8', '8', '7', '3', '6', '0', '8', '4', '8', '5', '0', '6', '3', '8', '9', '8', '8', '3', '9', '4', '1', '8', '8', '3', '3', '2', '9', '4', '1', '0', '8', '3', '7', '3', '0', '3', '9', '8', '6', '7', '5', '3', '7', '6', '1', '3', '7', '5', '5', '2', '7', '0', '7', '7', '1', '0', '0', '1', '8', '3', '6', '2', '0', '8', '1', '1', '9', '1', '0', '8', '8', '9', '3', '4', '3', '0', '6', '4', '5', '8', '3', '6', '2', '3', '6', 0}, 10, NULL);
	tau->num2 = bi_new((const char[]){'6', '2', '7', '3', '5', '6', '4', '7', '4', '8', '1', '4', '3', '5', '0', '6', '2', '0', '1', '7', '1', '1', '3', '4', '8', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '9', '9', '2', '7', '4', '3', '4', '3', '8', '8', '1', '5', '4', '6', '8', '3', '3', '0', '3', '2', '4', '3', '8', '5', '2', '1', '3', '1', '7', '9', '4', '9', '7', '8', '5', '5', '8', '1', '5', '5', '8', '5', '1', '0', '5', '8', '8', '6', '0', '7', '6', '6', '5', '6', '0', '0', '5', '4', '8', '5', '6', '5', '9', '5', '3', '7', '1', '4', '6', '6', '4', '0', '2', '2', '3', '5', '9', '2', '0', '4', '5', '2', '5', '7', '7', '0', '1', '4', '2', '4', '6', '0', '0', '1', '0', '0', '6', '0', '3', '3', '5', '9', '2', '2', '8', '0', '2', '9', '0', '0', '9', '4', '5', '6', '9', '8', '2', '8', '3', '0', '0', '7', '1', '3', '4', '8', '2', '7', '5', '3', '4', '2', '4', '3', '3', '1', '0', '6', '2', '5', '2', '1', '1', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '5', '3', '6', '9', '4', '4', '1', '0', '0', '3', '9', '1', '3', '6', '3', '7', '4', '1', '9', '1', '6', '4', '9', '3', '2', '6', '0', '1', '8', '7', '8', '1', '2', '3', '6', '2', '0', '7', '6', '6', '4', '4', '9', '0', '1', '7', '1', '9', '9', '2', '1', '2', '9', '4', '6', '5', '4', '9', '5', '4', '2', '0', '5', '2', '7', '1', '3', '3', '5', '7', '2', '6', '1', '8', '4', '6', '5', '6', '6', '1', '2', '9', '7', '7', '8', '0', '8', '5', '5', '7', '0', '0', '0', '0', '4', '3', '6', '8', '2', '8', '5', '4', '9', '0', '0', '8', '8', '0', '7', '7', '7', '3', '7', '5', '5', '7', '9', '8', '1', '1', '1', '8', '1', '8', '7', '2', '5', '1', '7', '5', '3', '9', '4', '6', '0', '1', '2', '4', '8', '3', '4', '7', '4', '8', '8', '2', '8', '6', '0', '8', '9', '3', '8', '7', '2', '1', '7', '8', '9', '8', '4', '7', '3', '8', '4', '0', '0', '5', '9', '2', '5', '6', '8', '6', '9', '5', '1', '4', '2', '6', '6', '7', '7', '1', '8', '4', '7', '4', '6', '3', '3', '4', '4', '8', '3', '1', '4', '0', '1', '7', '5', '8', '4', '9', '1', '8', '9', '8', '8', '4', '8', '9', '4', '4', '4', '6', '9', '6', '8', '7', '4', '1', '1', '0', '5', '0', '4', '1', '6', '1', '7', '5', '0', '8', '7', '3', '5', '0', '9', '3', '5', '8', '5', '2', '8', '8', '3', '1', '2', '3', '2', '9', '8', '0', '3', '6', '4', '8', '0', '4', '3', '7', '8', '9', '9', '5', '2', '9', '1', '3', '7', '1', '0', '6', '6', '6', '0', '4', '0', '0', '8', '0', '6', '0', '0', '7', '7', '6', '0', '1', '5', '7', '5', '8', '6', '9', '1', '5', '2', '4', '3', '8', '0', '5', '5', '2', '8', '8', '6', '4', '2', '3', '1', '6', '7', '5', '2', '9', '5', '9', '6', '2', '3', '6', '9', '6', '9', '9', '0', '4', '3', '8', '9', '0', '7', '3', '6', '4', '0', '8', '8', '4', '1', '1', '8', '2', '8', '8', '6', '1', '7', '8', '6', '8', '8', '0', '6', '5', '6', '7', '1', '7', '9', '7', '6', '8', '4', '5', '8', '9', '7', '1', '4', '6', '1', '2', '2', '6', '9', '7', '3', '1', '5', '2', '6', '5', '7', '4', '6', '0', '3', '5', '9', '5', '6', '5', '7', '9', '7', '9', '2', '8', '7', '1', '0', '5', '9', '6', '4', '1', '9', '5', '8', '4', '6', '3', '1', '1', '4', '2', '5', '6', '2', '4', '5', '9', '4', '9', '8', '2', '3', '4', '5', '6', '9', '0', '5', '0', '9', '5', '1', '6', '6', '1', '9', '8', '6', '9', '7', '5', '3', '8', '7', '1', '3', '2', '9', '6', '5', '8', '1', '7', '5', '8', '6', '7', '5', '7', '1', '0', '3', '2', '2', '6', '2', '9', '5', '2', '3', '6', '2', '4', '0', '0', '0', '1', '4', '3', '1', '2', '9', '1', '1', '1', '5', '7', '8', '0', '3', '1', '0', '2', '6', '6', '6', '2', '6', '9', '4', '9', '9', '5', '8', '0', '5', '1', '4', '6', '4', '1', '3', '5', '7', '1', '0', '4', '4', '5', '9', '1', '9', '9', '3', '8', '5', '3', '8', '9', '7', '6', '6', '2', '6', '6', '9', '7', '1', '9', '2', '7', '4', '9', '8', '6', '4', '4', '2', '8', '3', '1', '7', '8', '1', '2', '3', '2', '2', '5', '0', '2', '8', '2', '4', '8', '1', '5', '8', '7', '2', '9', '6', '9', '1', '5', '8', '8', '3', '0', '5', '7', '4', '5', '7', '0', '6', '1', '1', '2', '0', '9', '8', '0', '6', '5', '6', '6', '6', '3', '1', '4', '3', '3', '5', '5', '4', '2', '5', '1', '3', '7', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '3', '9', '6', '7', '8', '1', '0', '8', '3', '6', '6', '5', '9', '3', '6', '2', '0', '6', '5', '6', '2', '4', '8', '6', '0', '9', '8', '0', '2', '6', '8', '1', '3', '7', '8', '4', '8', '8', '0', '8', '4', '4', '5', '2', '5', '9', '1', '7', '4', '1', '2', '7', '3', '7', '1', '1', '8', '2', '4', '1', '9', '2', '5', '7', '2', '1', '1', '1', '5', '4', '7', '0', '9', '2', '1', '0', '4', '5', '1', '6', '7', '7', '6', '2', '9', '0', '9', '6', '5', '4', '7', '3', '5', '4', '7', '3', '6', '2', '3', '1', '7', '8', '0', '9', '3', '4', '7', '7', '4', '4', '7', '9', '3', '7', '3', '3', '1', '6', '3', '3', '1', '7', '3', '1', '0', '2', '0', '7', '5', '5', '6', '0', '4', '0', '5', '1', '3', '8', '2', '3', '2', '2', '9', '4', '7', '1', '2', '6', '3', '1', '0', '7', '2', '7', '8', '0', '8', '5', '5', '5', '1', '8', '2', '6', '8', '9', '8', '4', '2', '7', '7', '0', '7', '7', '6', '5', '5', '8', '0', '6', '3', '8', '4', '2', '2', '3', '5', '7', '6', '6', '6', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '2', '5', '9', '0', '8', '6', '7', '3', '8', '5', '0', '5', '7', '2', '4', '6', '8', '2', '9', '0', '3', '3', '7', '7', '2', '2', '6', '3', '0', '5', '3', '5', '0', '1', '7', '6', '0', '8', '8', '2', '5', '7', '5', '0', '2', '3', '3', '3', '0', '0', '4', '1', '5', '1', '2', '1', '1', '4', '3', '4', '9', '4', '4', '5', '7', '3', '6', '4', '1', '8', '2', '5', '3', '4', '8', '4', '4', '9', '7', '4', '9', '8', '7', '1', '5', '6', '9', '9', '0', '0', '3', '7', '8', '5', '4', '2', '8', '3', '7', '4', '4', '8', '4', '4', '6', '9', '7', '8', '8', '9', '3', '5', '2', '1', '6', '9', '0', '4', '6', '5', '9', '6', '7', '5', '2', '1', '1', '8', '5', '0', '9', '2', '7', '2', '3', '8', '7', '0', '1', '7', '1', '5', '7', '1', '5', '5', '4', '9', '0', '4', '7', '6', '3', '9', '8', '8', '0', '6', '7', '1', '5', '4', '6', '3', '3', '2', '7', '4', '8', '9', '5', '5', '0', '2', '7', '2', '8', '7', '6', '7', '3', '7', '8', '5', '3', '9', '5', '4', '6', '6', '8', '1', '5', '8', '1', '7', '1', '6', '1', '3', '5', '6', '2', '6', '9', '6', '8', '0', '9', '8', '5', '6', '8', '0', '9', '2', '6', '1', '6', '5', '2', '1', '7', '6', '5', '0', '6', '7', '1', '6', '8', '0', '4', '0', '3', '7', '9', '1', '9', '5', '2', '2', '6', '5', '1', '3', '6', '5', '5', '9', '6', '3', '1', '8', '1', '1', '7', '5', '0', '7', '2', '7', '0', '0', '0', '5', '2', '8', '7', '5', '1', '9', '3', '2', '8', '5', '5', '1', '9', '6', '6', '7', '1', '3', '7', '7', '9', '1', '9', '7', '0', '2', '0', '5', '9', '2', '1', '8', '2', '4', '4', '7', '4', '3', '2', '5', '3', '5', '3', '2', '6', '0', '6', '1', '4', '0', '4', '9', '6', '6', '2', '8', '2', '1', '0', '5', '0', '7', '6', '3', '5', '0', '5', '8', '2', '1', '3', '7', '8', '0', '4', '9', '6', '9', '4', '0', '3', '4', '5', '9', '4', '2', '4', '3', '9', '4', '6', '2', '1', '1', '5', '2', '5', '4', '1', '6', '9', '7', '3', '4', '1', '7', '9', '7', '3', '2', '1', '2', '0', '9', '8', '1', '1', '8', '3', '5', '4', '1', '6', '0', '3', '3', '1', '4', '5', '6', '0', '5', '5', '2', '5', '7', '3', '8', '3', '8', '9', '7', '6', '6', '9', '0', '3', '6', '1', '1', '9', '8', '8', '3', '6', '2', '2', '3', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '3', '0', '0', '3', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '6', '7', '6', '6', '1', '3', '1', '0', '6', '4', '5', '4', '7', '8', '5', '5', '4', '4', '6', '1', '9', '5', '4', '9', '6', '3', '2', '6', '2', '4', '6', '8', '4', '0', '9', '3', '4', '5', '7', '1', '4', '1', '8', '7', '4', '7', '6', '7', '0', '0', '4', '9', '9', '8', '1', '9', '0', '3', '5', '4', '1', '4', '9', '7', '6', '9', '4', '7', '3', '3', '0', '9', '6', '4', '9', '1', '9', '4', '4', '9', '3', '1', '5', '1', '5', '5', '6', '0', '1', '4', '6', '3', '2', '2', '1', '4', '7', '9', '1', '7', '4', '4', '9', '1', '4', '8', '3', '8', '2', '7', '0', '0', '6', '1', '4', '4', '3', '0', '3', '4', '8', '1', '9', '9', '4', '6', '7', '2', '9', '2', '0', '2', '9', '5', '9', '8', '5', '3', '0', '7', '8', '9', '8', '4', '9', '9', '3', '5', '5', '1', '2', '4', '9', '0', '0', '5', '7', '8', '5', '0', '2', '2', '6', '8', '9', '2', '7', '5', '8', '4', '5', '0', '2', '7', '6', '3', '0', '1', '3', '7', '0', '5', '5', '7', '4', '6', '5', '1', '5', '0', '1', '8', '8', '7', '7', '9', '4', '3', '2', '1', '6', '3', '5', '7', '7', '2', '9', '9', '9', '6', '8', '6', '4', '1', '0', '5', '6', '8', '5', '0', '8', '9', '7', '7', '5', '0', '5', '7', '5', '0', '2', '9', '1', '4', '6', '8', '3', '0', '2', '8', '9', '7', '1', '7', '0', '1', '7', '4', '7', '4', '5', '9', '1', '8', '7', '5', '8', '3', '2', '4', '5', '0', '6', '0', '4', '7', '5', '8', '1', '7', '6', '8', '5', '8', '2', '4', '5', '4', '9', '0', '2', '3', '3', '9', '1', '8', '2', '8', '5', '9', '1', '1', '3', '6', '4', '2', '5', '1', '2', '1', '0', '4', '7', '3', '1', '8', '9', '2', '9', '7', '9', '4', '3', '6', '3', '4', '6', '9', '7', '1', '5', '2', '2', '3', '9', '4', '8', '9', '5', '8', '4', '2', '9', '1', '7', '4', '5', '3', '3', '9', '2', '3', '7', '4', '6', '3', '9', '9', '4', '4', '2', '6', '7', '9', '7', '3', '8', '0', '4', '5', '5', '4', '7', '1', '4', '5', '2', '6', '5', '1', '2', '2', '0', '3', '1', '5', '3', '8', '0', '8', '1', '0', '3', '4', '6', '6', '9', '7', '3', '4', '9', '6', '9', '3', '8', '1', '1', '4', '7', '3', '9', '1', '8', '0', '1', '6', '6', '1', '4', '9', '1', '8', '5', '4', '6', '9', '3', '9', '4', '1', '3', '8', '7', '9', '3', '3', '0', '8', '5', '0', '1', '7', '0', '5', '2', '5', '4', '6', '6', '2', '1', '0', '5', '7', '6', '4', '4', '1', '3', '9', '1', '5', '4', '2', '2', '4', '3', '2', '6', '8', '1', '5', '6', '4', '0', '4', '8', '0', '8', '1', '4', '5', '2', '8', '7', '2', '4', '8', '3', '1', '4', '4', '9', '2', '6', '3', '6', '9', '8', '8', '6', '6', '1', '4', '7', '6', '6', '6', '1', '0', '2', '7', '2', '5', '6', '7', '4', '2', '4', '8', '2', '0', '7', '6', '8', '6', '0', '1', '1', '7', '4', '2', '7', '0', '1', '2', '6', '6', '7', '5', '3', '4', '6', '0', '5', '6', '4', '3', '3', '6', '3', '9', '2', '8', '9', '3', '2', '8', '8', '4', '9', '3', '4', '3', '8', '1', '1', '4', '1', '2', '8', '1', '9', '4', '1', '6', '9', '2', '1', '6', '7', '3', '5', '4', '6', '3', '5', '6', '1', '0', '8', '7', '1', '5', '8', '9', '3', '3', '8', '5', '3', '2', '9', '4', '4', '8', '5', '1', '6', '8', '1', '5', '8', '3', '9', '6', '2', '7', '5', '4', '3', '3', '3', '8', '8', '5', '0', '9', '6', '9', '2', '9', '3', '7', '8', '4', '6', '1', '7', '6', '7', '4', '2', '1', '5', '9', '6', '3', '5', '0', '9', '5', '1', '6', '8', '0', '0', '0', '2', '1', '6', '0', '8', '1', '3', '5', '7', '6', '3', '0', '7', '3', '2', '4', '2', '7', '5', '7', '1', '5', '9', '7', '1', '3', '2', '8', '4', '5', '4', '1', '7', '5', '0', '5', '9', '0', '0', '5', '9', '3', '5', '8', '6', '9', '1', '8', '5', '1', '8', '1', '5', '5', '4', '8', '2', '4', '1', '8', '0', '9', '8', '3', '1', '2', '8', '2', '4', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '0', '0', '0', '0', '6', '9', '6', '9', '8', '8', '3', '6', '7', '4', '5', '5', '4', '9', '4', '8', '9', '0', '8', '6', '6', '7', '1', '2', '6', '1', '7', '8', '6', '2', '7', '2', '5', '6', '4', '5', '3', '2', '2', '8', '3', '9', '2', '6', '3', '2', '4', '1', '4', '7', '8', '3', '1', '5', '3', '6', '8', '9', '5', '3', '0', '9', '2', '1', '7', '4', '1', '1', '5', '0', '7', '1', '8', '1', '9', '1', '0', '6', '3', '8', '2', '5', '5', '0', '5', '4', '6', '8', '3', '8', '1', '2', '9', '0', '5', '3', '5', '7', '4', '4', '7', '5', '3', '5', '1', '9', '5', '4', '0', '4', '9', '2', '2', '7', '9', '1', '4', '8', '7', '6', '9', '7', '0', '2', '6', '3', '7', '1', '9', '9', '7', '5', '2', '9', '3', '7', '1', '2', '0', '4', '6', '6', '7', '8', '4', '0', '0', '1', '0', '3', '8', '5', '4', '7', '3', '8', '1', '7', '9', '1', '1', '4', '2', '5', '4', '4', '0', '2', '4', '0', '9', '0', '8', '3', '0', '4', '7', '8', '4', '4', '5', '0', '0', '8', '6', '8', '0', '9', '2', '6', '9', '9', '7', '3', '6', '4', '7', '2', '2', '5', '5', '9', '9', '5', '2', '5', '5', '9', '4', '2', '7', '7', '2', '0', '7', '0', '3', '6', '9', '0', '0', '9', '2', '4', '3', '0', '3', '2', '4', '5', '1', '0', '3', '3', '0', '2', '1', '6', '8', '1', '3', '4', '8', '9', '5', '6', '3', '5', '8', '5', '1', '3', '6', '7', '3', '2', '4', '9', '4', '5', '1', '0', '0', '2', '1', '6', '4', '3', '2', '2', '7', '3', '1', '3', '0', '3', '0', '1', '9', '6', '3', '7', '4', '5', '8', '7', '1', '4', '9', '7', '1', '5', '8', '7', '2', '4', '5', '3', '3', '8', '0', '4', '3', '4', '4', '0', '1', '8', '7', '5', '2', '1', '4', '9', '7', '3', '9', '7', '7', '2', '5', '4', '9', '0', '0', '2', '7', '8', '8', '3', '9', '2', '6', '2', '8', '5', '9', '2', '7', '3', '7', '7', '7', '4', '3', '3', '9', '3', '4', '5', '5', '3', '4', '5', '9', '7', '4', '9', '8', '6', '2', '5', '3', '2', '4', '0', '5', '6', '9', '2', '5', '6', '6', '0', '4', '7', '5', '0', '7', '1', '0', '1', '0', '7', '5', '0', '1', '6', '2', '4', '4', '3', '6', '8', '6', '0', '6', '6', '0', '1', '2', '6', '4', '9', '8', '0', '8', '8', '2', '5', '2', '6', '4', '7', '4', '9', '4', '4', '0', '1', '4', '1', '3', '9', '3', '6', '8', '0', '2', '6', '8', '4', '8', '9', '3', '0', '7', '1', '4', '7', '8', '5', '8', '4', '0', '7', '7', '3', '3', '3', '1', '2', '2', '7', '2', '0', '8', '0', '4', '3', '3', '3', '9', '8', '8', '9', '3', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '6', '4', '2', '1', '9', '1', '4', '2', '4', '3', '4', '9', '1', '4', '7', '8', '9', '4', '6', '1', '7', '6', '9', '4', '7', '6', '4', '7', '0', '6', '5', '6', '8', '1', '1', '4', '6', '7', '1', '4', '9', '6', '1', '3', '4', '9', '7', '9', '8', '1', '7', '1', '2', '9', '3', '4', '0', '4', '5', '5', '3', '7', '4', '7', '6', '2', '4', '5', '2', '6', '3', '6', '0', '5', '8', '3', '7', '3', '2', '8', '8', '9', '0', '3', '2', '9', '9', '1', '9', '1', '3', '4', '4', '6', '7', '2', '5', '5', '5', '2', '3', '0', '0', '6', '9', '5', '4', '8', '1', '2', '4', '7', '3', '1', '4', '0', '2', '2', '3', '6', '6', '1', '6', '3', '3', '8', '3', '0', '1', '2', '1', '2', '0', '2', '6', '9', '3', '8', '0', '6', '1', '0', '7', '5', '6', '5', '5', '2', '3', '5', '1', '1', '5', '2', '6', '2', '6', '5', '9', '4', '2', '4', '4', '4', '9', '6', '7', '9', '5', '5', '2', '7', '8', '3', '1', '9', '4', '9', '6', '8', '6', '8', '9', '5', '0', '3', '7', '3', '4', '8', '9', '1', '3', '5', '4', '0', '1', '1', '9', '2', '7', '0', '5', '6', '0', '0', '9', '4', '6', '2', '3', '5', '2', '8', '1', '5', '3', '5', '7', '5', '5', '8', '7', '5', '7', '3', '9', '4', '5', '4', '8', '1', '0', '0', '9', '8', '8', '7', '0', '6', '7', '1', '8', '6', '7', '4', '9', '9', '1', '4', '1', '7', '3', '8', '7', '7', '2', '9', '8', '4', '9', '1', '6', '0', '5', '3', '0', '0', '4', '6', '9', '1', '6', '2', '7', '4', '6', '8', '2', '4', '6', '0', '5', '2', '6', '5', '6', '3', '8', '4', '6', '0', '7', '2', '1', '5', '9', '5', '5', '9', '8', '1', '1', '5', '9', '4', '7', '5', '5', '3', '9', '0', '0', '3', '0', '5', '0', '5', '5', '0', '7', '6', '2', '8', '0', '9', '9', '7', '1', '5', '1', '6', '5', '9', '0', '1', '5', '6', '0', '7', '0', '2', '4', '4', '3', '5', '5', '4', '1', '7', '8', '0', '6', '7', '4', '1', '2', '7', '7', '8', '0', '1', '7', '5', '0', '7', '1', '5', '7', '2', '2', '7', '9', '9', '2', '3', '9', '1', '9', '4', '8', '0', '9', '3', '6', '1', '0', '9', '1', '4', '8', '0', '6', '4', '7', '9', '0', '6', '9', '5', '1', '1', '7', '1', '5', '6', '4', '6', '7', '3', '0', '6', '7', '5', '2', '0', '5', '5', '4', '5', '2', '5', '3', '7', '9', '0', '7', '7', '4', '1', '4', '6', '9', '8', '8', '2', '6', '7', '5', '5', '7', '0', '9', '0', '1', '2', '4', '2', '8', '2', '1', '4', '9', '6', '6', '3', '3', '9', '4', '7', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '2', '0', '0', '0', '2', '0', '0', '3', '8', '8', '3', '5', '6', '2', '7', '6', '1', '5', '0', '6', '6', '8', '5', '5', '8', '6', '7', '7', '5', '9', '4', '2', '8', '3', '2', '5', '1', '2', '5', '2', '6', '5', '4', '5', '2', '7', '4', '8', '2', '3', '6', '1', '3', '7', '8', '5', '6', '8', '4', '4', '1', '9', '7', '0', '2', '4', '3', '5', '4', '6', '3', '6', '8', '3', '9', '6', '4', '6', '3', '3', '8', '1', '7', '5', '2', '8', '6', '0', '2', '5', '9', '8', '9', '9', '5', '3', '1', '9', '9', '6', '8', '9', '3', '0', '9', '6', '5', '2', '3', '0', '9', '9', '2', '7', '9', '7', '8', '6', '3', '8', '3', '7', '3', '1', '6', '3', '5', '0', '0', '6', '1', '2', '8', '9', '3', '2', '2', '1', '3', '3', '9', '6', '5', '7', '5', '9', '3', '8', '2', '6', '0', '2', '1', '7', '3', '2', '0', '5', '0', '9', '8', '5', '6', '9', '0', '9', '0', '7', '7', '8', '6', '0', '8', '1', '3', '6', '2', '7', '2', '8', '9', '3', '0', '6', '8', '4', '2', '3', '3', '6', '6', '1', '3', '1', '4', '3', '0', '7', '6', '3', '2', '5', '6', '5', '4', '4', '5', '8', '0', '1', '3', '9', '4', '0', '1', '2', '1', '3', '5', '9', '1', '9', '7', '3', '3', '0', '8', '2', '6', '1', '6', '4', '4', '4', '4', '6', '0', '8', '7', '3', '3', '7', '2', '8', '5', '0', '2', '9', '7', '3', '2', '8', '1', '5', '2', '4', '6', '4', '5', '1', '7', '7', '0', '4', '8', '2', '6', '0', '9', '8', '0', '7', '8', '2', '8', '2', '0', '1', '2', '8', '0', '1', '6', '5', '7', '2', '6', '7', '7', '0', '7', '9', '1', '4', '3', '4', '3', '6', '6', '4', '4', '7', '1', '4', '5', '3', '2', '9', '1', '2', '8', '9', '0', '6', '6', '9', '3', '8', '6', '1', '7', '3', '9', '4', '5', '4', '3', '8', '7', '3', '9', '4', '7', '9', '0', '6', '0', '0', '2', '2', '1', '5', '4', '5', '5', '8', '0', '6', '7', '6', '3', '7', '5', '6', '6', '2', '2', '7', '4', '7', '4', '1', '1', '1', '2', '7', '6', '0', '4', '8', '7', '0', '0', '0', '8', '4', '6', '0', '9', '9', '0', '7', '5', '9', '3', '4', '4', '4', '4', '3', '4', '1', '0', '8', '8', '4', '4', '9', '4', '5', '5', '7', '7', '6', '9', '9', '0', '3', '2', '7', '1', '7', '4', '1', '1', '0', '8', '9', '1', '2', '6', '9', '9', '0', '3', '0', '9', '9', '3', '0', '7', '2', '9', '7', '9', '6', '8', '0', '2', '1', '4', '1', '0', '8', '1', '4', '9', '2', '7', '0', '9', '9', '8', '1', '4', '3', '3', '8', '6', '5', '5', '3', '7', '2', '5', '7', '2', '2', '5', '5', '3', '1', '4', '8', '9', '6', '4', '1', '3', '7', '8', '9', '4', '9', '9', '1', '0', '2', '5', '0', '8', '7', '0', '3', '8', '1', '5', '0', '0', '2', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '2', '2', '0', '8', '5', '9', '9', '2', '9', '1', '0', '1', '7', '6', '5', '3', '4', '4', '8', '5', '0', '9', '2', '4', '2', '0', '5', '0', '9', '9', '1', '8', '5', '7', '7', '5', '1', '4', '3', '0', '5', '1', '7', '2', '3', '8', '8', '1', '7', '2', '7', '0', '8', '2', '3', '2', '1', '6', '3', '6', '3', '3', '5', '5', '6', '5', '2', '7', '9', '0', '7', '6', '3', '9', '0', '8', '4', '0', '3', '6', '9', '8', '7', '8', '2', '3', '1', '3', '8', '1', '1', '7', '9', '7', '5', '8', '3', '7', '5', '9', '2', '4', '6', '3', '6', '5', '4', '5', '4', '0', '7', '6', '1', '6', '3', '2', '2', '0', '3', '7', '2', '5', '4', '0', '8', '7', '4', '4', '1', '4', '4', '8', '4', '9', '0', '3', '5', '5', '0', '1', '9', '9', '9', '1', '1', '7', '5', '1', '9', '2', '4', '5', '1', '1', '8', '4', '7', '0', '4', '8', '7', '4', '3', '8', '0', '8', '3', '5', '9', '1', '0', '3', '8', '0', '1', '6', '8', '8', '9', '3', '7', '2', '1', '0', '5', '6', '3', '6', '2', '3', '4', '7', '5', '9', '2', '9', '6', '3', '4', '4', '3', '2', '5', '0', '5', '5', '2', '8', '9', '1', '0', '0', '5', '7', '4', '4', '9', '0', '5', '5', '9', '1', '8', '3', '2', '0', '5', '0', '7', '8', '8', '2', '0', '2', '7', '6', '3', '5', '0', '1', '3', '6', '5', '6', '0', '9', '1', '9', '7', '7', '7', '1', '6', '4', '4', '6', '1', '2', '5', '5', '1', '4', '0', '1', '0', '0', '0', '7', '4', '4', '0', '2', '4', '8', '6', '5', '9', '8', '7', '3', '7', '3', '7', '6', '2', '9', '0', '4', '7', '5', '1', '1', '7', '7', '6', '2', '3', '7', '0', '8', '8', '0', '7', '6', '4', '7', '9', '8', '4', '5', '8', '0', '1', '6', '3', '6', '8', '7', '0', '8', '9', '5', '4', '5', '8', '8', '6', '3', '5', '5', '4', '1', '0', '7', '2', '8', '4', '3', '4', '8', '9', '2', '8', '7', '5', '7', '0', '3', '6', '7', '8', '8', '1', '5', '5', '2', '5', '0', '4', '0', '7', '4', '4', '8', '7', '8', '8', '0', '5', '2', '5', '1', '3', '8', '8', '2', '7', '1', '3', '1', '4', '6', '6', '2', '5', '0', '9', '3', '5', '9', '9', '1', '7', '5', '2', '3', '6', '9', '4', '3', '3', '4', '8', '7', '6', '9', '1', '0', '8', '0', '4', '9', '2', '1', '5', '2', '0', '5', '3', '4', '0', '7', '6', '3', '7', '4', '7', '5', '6', '1', '8', '9', '4', '4', '8', '7', '1', '0', '5', '2', '3', '9', '9', '2', '9', '0', '8', '4', '0', '4', '8', '6', '5', '3', '0', '6', '4', '7', '0', '8', '2', '8', '8', '2', '1', '5', '6', '3', '6', '7', '5', '1', '7', '8', '9', '5', '7', '1', '5', '3', '8', '8', '7', '6', '4', '4', '0', '1', '9', '1', '1', '9', '0', '8', '1', '1', '3', '3', '1', '5', '9', '1', '1', '1', '7', '6', '5', '8', '0', '1', '1', '7', '1', '5', '9', '9', '8', '1', '3', '4', '1', '8', '2', '0', '1', '1', '4', '2', '9', '9', '6', '7', '8', '1', '5', '9', '6', '6', '0', '0', '1', '9', '0', '3', '3', '7', '9', '8', '0', '4', '1', '9', '2', '9', '1', '5', '4', '3', '6', '7', '2', '7', '1', '9', '0', '6', '2', '4', '7', '7', '1', '3', '9', '6', '2', '4', '0', '8', '4', '0', '4', '1', '7', '9', '4', '1', '4', '9', '5', '2', '0', '7', '8', '2', '5', '7', '7', '9', '1', '3', '7', '6', '0', '4', '5', '7', '4', '1', '4', '6', '5', '7', '3', '9', '6', '8', '9', '8', '6', '7', '3', '1', '5', '3', '4', '6', '8', '5', '7', '4', '4', '1', '6', '0', '2', '7', '6', '8', '4', '8', '3', '9', '4', '1', '0', '8', '9', '1', '0', '6', '7', '2', '2', '7', '8', '0', '0', '0', '7', '5', '4', '0', '2', '1', '6', '7', '9', '5', '0', '1', '2', '9', '7', '9', '5', '5', '6', '0', '8', '5', '4', '8', '8', '6', '9', '5', '4', '2', '5', '0', '4', '0', '6', '8', '2', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '5', '3', '3', '6', '7', '4', '2', '6', '8', '9', '6', '2', '8', '9', '9', '0', '5', '1', '7', '5', '9', '2', '3', '3', '2', '2', '9', '7', '8', '0', '3', '5', '0', '8', '0', '0', '0', '9', '6', '8', '7', '4', '3', '0', '0', '9', '9', '1', '3', '7', '3', '0', '2', '0', '2', '1', '1', '6', '9', '2', '3', '2', '6', '8', '7', '3', '7', '8', '3', '5', '4', '4', '8', '5', '0', '4', '2', '3', '1', '9', '3', '8', '7', '3', '7', '2', '4', '4', '5', '1', '8', '0', '9', '8', '6', '1', '1', '1', '7', '1', '4', '8', '7', '1', '9', '4', '8', '9', '6', '3', '5', '5', '3', '1', '0', '8', '2', '8', '9', '4', '5', '9', '0', '7', '1', '5', '6', '9', '8', '4', '3', '2', '2', '1', '5', '3', '7', '1', '6', '9', '6', '1', '2', '8', '6', '9', '2', '8', '4', '9', '3', '8', '2', '1', '8', '1', '4', '7', '0', '4', '6', '8', '6', '6', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '2', '1', '6', '1', '3', '8', '6', '2', '0', '0', '9', '9', '8', '9', '2', '3', '7', '7', '2', '4', '9', '5', '2', '3', '9', '0', '8', '0', '4', '1', '3', '2', '4', '5', '6', '0', '3', '0', '8', '0', '2', '6', '7', '1', '3', '5', '0', '1', '3', '1', '7', '2', '2', '3', '7', '2', 0}, 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(large_subtractions, test_o500c_minus_o500d)
{
	/* clang-format off */
	const char expected[] = "2769315567803400000000000000000000007608363883953254006036722809375820000000000000000946001900602100087560002500051243171865731050750745462388288171212746300721613469564396741836389979086904304472475224061238205255674131396893464663867829125664459890000000000000000000000000000000000000000000000000000000000000000000000055751571788169002287927112563608472464633056032554997220900156740475785919449000000000000000000000000093870073695755688436933812779613089249764460305002386268274730485837806748795573874707795014946007865785192966940373072789113547534542583594861890770021698776997999999999999999999999999999999999999944444444444444444";

	tau->num1 = bi_new((const char[]){'2', '7', '6', '9', '3', '1', '5', '5', '6', '7', '8', '0', '3', '4', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '7', '6', '0', '8', '3', '6', '3', '9', '8', '3', '9', '5', '3', '2', '5', '4', '0', '0', '5', '0', '3', '6', '7', '2', '2', '8', '0', '9', '3', '7', '5', '8', '2', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '9', '4', '6', '0', '0', '1', '9', '0', '0', '6', '0', '2', '1', '0', '0', '0', '8', '7', '5', '6', '0', '0', '0', '2', '5', '0', '0', '0', '5', '1', '2', '4', '3', '1', '7', '1', '8', '6', '5', '7', '3', '1', '0', '5', '0', '7', '5', '0', '7', '4', '5', '4', '6', '2', '3', '8', '8', '2', '8', '8', '1', '7', '1', '2', '1', '2', '7', '4', '6', '3', '0', '0', '7', '2', '1', '6', '1', '3', '4', '6', '9', '5', '6', '4', '3', '9', '6', '7', '4', '1', '8', '3', '6', '3', '8', '9', '9', '7', '9', '0', '8', '6', '9', '0', '4', '3', '0', '4', '4', '7', '2', '4', '7', '6', '0', '0', '1', '8', '3', '9', '0', '1', '5', '9', '8', '3', '0', '3', '3', '4', '5', '1', '9', '0', '9', '1', '7', '4', '6', '6', '3', '4', '6', '4', '6', '6', '3', '8', '6', '7', '8', '2', '9', '1', '2', '5', '6', '6', '4', '4', '5', '9', '8', '9', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '5', '5', '7', '5', '1', '5', '7', '1', '7', '8', '8', '1', '6', '9', '0', '0', '2', '2', '8', '7', '9', '2', '7', '1', '1', '2', '6', '7', '4', '7', '1', '9', '5', '8', '3', '5', '7', '5', '7', '4', '4', '1', '6', '7', '1', '4', '3', '6', '6', '4', '9', '9', '7', '2', '2', '0', '9', '0', '0', '1', '5', '6', '7', '4', '0', '4', '7', '5', '7', '8', '5', '9', '1', '9', '4', '4', '9', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '9', '3', '8', '7', '0', '0', '7', '3', '6', '9', '5', '7', '5', '5', '6', '8', '8', '4', '3', '6', '9', '3', '3', '8', '1', '2', '7', '7', '9', '6', '1', '3', '0', '8', '9', '2', '4', '9', '7', '6', '4', '4', '6', '1', '1', '9', '3', '8', '9', '1', '2', '7', '5', '1', '5', '7', '1', '6', '3', '6', '1', '9', '3', '7', '4', '7', '2', '6', '6', '9', '5', '6', '3', '7', '6', '8', '4', '4', '6', '1', '8', '7', '4', '7', '0', '7', '7', '9', '5', '0', '1', '4', '9', '4', '6', '0', '0', '7', '8', '6', '5', '7', '8', '5', '1', '9', '2', '9', '6', '6', '9', '4', '0', '3', '7', '3', '0', '7', '2', '7', '8', '9', '1', '1', '3', '5', '4', '7', '5', '3', '4', '5', '4', '2', '5', '8', '3', '5', '9', '4', '8', '6', '1', '8', '9', '0', '7', '7', '0', '0', '2', '1', '6', '9', '8', '9', '9', '9', '2', '1', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', 0}, 10, NULL);
	tau->num2 = bi_new((const char[]){'9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '2', '2', '2', '2', '2', '2', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', 0}, 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(large_subtractions, test_o500d_minus_o500c)
{
	/* clang-format off */
	const char expected[] = "-2769315567803400000000000000000000007608363883953254006036722809375820000000000000000946001900602100087560002500051243171865731050750745462388288171212746300721613469564396741836389979086904304472475224061238205255674131396893464663867829125664459890000000000000000000000000000000000000000000000000000000000000000000000055751571788169002287927112563608472464633056032554997220900156740475785919449000000000000000000000000093870073695755688436933812779613089249764460305002386268274730485837806748795573874707795014946007865785192966940373072789113547534542583594861890770021698776997999999999999999999999999999999999999944444444444444444";

	tau->num1 = bi_new("99999999999000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000777777777777777777777777770000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111111111111111111111110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000888888888888888888888888888888888888000000000000000000000000000000000000000000000000000000000000000000000000000222222000000000000000000000000000000000000055555555555555555", 10, NULL);
	tau->num2 = bi_new("2769315567803400000000000000000000007608363983953254005036722809375820000000000000000946001900602100087560002500051243171865731050750745462388288171212746300721613469564396741836389979086904304472476001839015983033451909174663464663867829125664459890000000000000000000000000000000000000000000000000000000000000000000000055751571788169002287927112674719583575744167143664997220900156740475785919449000000000000000000000000093870073695755688436933812779613089249764461193891275157163619374726695637684461874707795014946007865785192966940373072789113547534542583594861890770021698999219999999999999999999999999999999999999999999999999999999", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	REQUIRE_PTR_NE(tau->num2, NULL);
	tau->output = bi_subtract(tau->num1, tau->num2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

/* ################################################################### */
/* ###################### bi_subtract_int ############################ */
/* ################################################################### */

/* invalid_inputs */

TEST(invalid_inputs, test_null_minus_0i)
{
	CHECK_PTR_EQ(bi_subtract_int(NULL, 0), NULL);
}

TEST(invalid_inputs, test_null_minus_1i)
{
	CHECK_PTR_EQ(bi_subtract_int(NULL, 1), NULL);
}

TEST(invalid_inputs, test_null_minus_neg1i)
{
	CHECK_PTR_EQ(bi_subtract_int(NULL, -1), NULL);
}

/* NaN */

TEST_F(invalid_inputs, test_NaN_minus_0i)
{
	tau->output = bi_subtract_int(&(tau->num1), 0);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_NaN_minus_490998i)
{
	tau->output = bi_subtract_int(&(tau->num1), 490998);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

/* simple_subtractions */

TEST_F(simple_subtractions, test_0_minus_0i)
{
	const char expected[] = "0";

	tau->num1 = bi_new("0", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, 0);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_subtractions, test_1_minus_0i)
{
	const char expected[] = "1";

	tau->num1 = bi_new("1", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, 0);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_subtractions, test_0_minus_1i)
{
	const char expected[] = "-1";

	tau->num1 = bi_new("0", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, 1);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_subtractions, test_1_minus_1i)
{
	const char expected[] = "0";

	tau->num1 = bi_new("1", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, 1);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_subtractions, test_1000000000_minus_50000i)
{
	const char expected[] = "999950000";

	tau->num1 = bi_new("1000000000", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, 50000);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

/************** REVERSE OF PREVIOUS *****************/

TEST_F(simple_subtractions, test_50000_minus_100000000i)
{
	const char expected[] = "-999950000";

	tau->num1 = bi_new("50000", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, 1000000000);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

/* same_number_subtractions */

TEST_F(same_number_subtractions, test_1000000001_minus_1000000001i)
{
	const char expected[] = "0";

	tau->num1 = bi_new("1000000001", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, 1000000001);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(same_number_subtractions, test_longnum_minus_longi)
{
	const char expected[] = "0";

	tau->num1 = bi_new("1000000001000000001", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, 1000000001000000001);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

/* long_subtractions */

TEST_F(long_subtractions, test_long9s_minus_1i)
{
	/* clang-format off */
	const char expected[] = "999999999999999999999999999999999999999999998";

	tau->num1 = bi_new("999999999_999999999_999999999_999999999_999999999", 10, NULL);
	/* clang-format oon */
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, 1);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(long_subtractions, test_1_minus_long9si)
{
	const char expected[] = "-999999999999999998";

	tau->num1 = bi_new("1", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, 999999999999999999);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(long_subtractions, test_4000000000678_minus_999999000i)
{
	const char expected[] = "3999000001678";

	tau->num1 = bi_new("4000000000678", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, 999999000);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

/****************** REVERSE OF PREVIOUS *******************/

TEST_F(long_subtractions, test_999999000_minus_4000000000678i)
{
	const char expected[] = "-3999000001678";

	tau->num1 = bi_new("999999000", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, 4000000000678);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

/* negative_subtractions */

TEST_F(negative_subtractions, test_neg1_minus_neg1i)
{
	const char expected[] = "0";

	tau->num1 = bi_new("-1", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, -1);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(negative_subtractions, test_1_minus_neg1i)
{
	const char expected[] = "2";

	tau->num1 = bi_new("1", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, -1);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(negative_subtractions, test_neg1_minus_1i)
{
	const char expected[] = "-2";

	tau->num1 = bi_new("-1", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, 1);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

/* large_subtractions */

TEST_F(large_subtractions, test_o1kb_minus_longmax)
{
	/* clang-format off */
	const char expected[] = "18229261729562381999943195364150791141310842462660658472794052519590847565789349402718324004839857142928212620403202777713783604366202070759555626401852588078440691829064124951508218929855914917618450280848912007284499268739280728777673597141834727026189637501497182469116507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363725908514186546204357679842338718477444792073993423658482382428119816381501067481045166037730605620161967625613384414360383390441495263443219011465754445417842402092461651572335077870774981712577246796292638635637328991215483143816789988504044536402352738195137863656439121201039712282212072035747723202909336516542736369239534563619774418024541935268238240138010931106827128463433638706953587774858293618793747247853007337951511006306768696205902321912271659743738793851605740584319133366099858333284887972925209519622805777808979101803594509105852590156377468512842704066384629041620510828673690634208644524713467124003954758766591788040711956623252229607490015860745922799063154105933871396514326330277128786448213684163131699655352740277960712827080282064325722872169008277075937134092496167580321385820199999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999900000000000004896852122159748040826366982335835356427825244648590777629677427537161606397652667824148584074238759183315862951747140489633154349916600904419167460866613186527131609809402688888959338866144546729216591135416351470307816655465609245482697946389174728520292956645492099153257573117854696971445605917499226105187855714255418747631272976737617083917593192334976446119389127515716361937472669563768446187470779501494600786578519296694037307278911354753454258359486189077002169899921757120005690208703866012829142325094262385420689723314187116330034871575195491637987496678148046582669687650610735383534630322415175882430662785141319818160673319428399211808955165934919962818022359011047491630355792683406483698506752363991791152227018012937850141935804051202045867410612359627665839070940218792151714831191398948701330911110449016834009494838468182995180417635079489225907749254660881718792594659210265970467004498198990968620394600177430944738110569912941285428918808553627074076707225937377726669734409773612433363973080517630915068363107953126072395203652900321058488395079814523072994171857157962974549950235053160409198591937180233074148804462179228008317660409386563445710347785534571210805307363945359239326518660305150410609664373133236728315393235000679371075419554373624332483612425259458688023539167661815323758555048869014322213497333540973911560210759419076588580143226629693516675690076644983336821558156514109617878518101037445197384000000000000423768734194700000000008378345892300000000000001394568376700000324000543630640000004493893473278700035645005669047476411111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111152687296422452231917546188873608485063898839418833294108373039867537613755270771001836208109885521393791060429";

	tau->num1 = bi_new("18229261729562381999943195364150791141310842462660658472794052519590847565789349402718324004839857142928212620403202777713783604366202070759555626401852588078440691829064124951508218929855914917618450280848912007284499268739280728777673597141834727026189637501497182469116507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363725908514186546204357679842338718477444792073993423658482382428119816381501067481045166037730605620161967625613384414360383390441495263443219011465754445417842402092461651572335077870774981712577246796292638635637328991215483143816789988504044536402352738195137863656439121201039712282212072035747723202909336516542736369239534563619774418024541935268238240138010931106827128463433638706953587774858293618793747247853007337951511006306768696205902321912271659743738793851605740584319133366099858333284887972925209519622805777808979101803594509105852590156377468512842704066384629041620510828673690634208644524713467124003954758766591788040711956623252229607490015860745922799063154105933871396514326330277128786448213684163131699655352740277960712827080282064325722872169008277075937134092496167580321385820199999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999900000000000004896852122159748040826366982335835356427825244648590777629677427537161606397652667824148584074238759183315862951747140489633154349916600904419167460866613186527131609809402688888959338866144546729216591135416351470307816655465609245482697946389174728520292956645492099153257573117854696971445605917499226105187855714255418747631272976737617083917593192334976446119389127515716361937472669563768446187470779501494600786578519296694037307278911354753454258359486189077002169899921757120005690208703866012829142325094262385420689723314187116330034871575195491637987496678148046582669687650610735383534630322415175882430662785141319818160673319428399211808955165934919962818022359011047491630355792683406483698506752363991791152227018012937850141935804051202045867410612359627665839070940218792151714831191398948701330911110449016834009494838468182995180417635079489225907749254660881718792594659210265970467004498198990968620394600177430944738110569912941285428918808553627074076707225937377726669734409773612433363973080517630915068363107953126072395203652900321058488395079814523072994171857157962974549950235053160409198591937180233074148804462179228008317660409386563445710347785534571210805307363945359239326518660305150410609664373133236728315393235000679371075419554373624332483612425259458688023539167661815323758555048869014322213497333540973911560210759419076588580143226629693516675690076644983336821558156514109617878518101037445197384000000000000423768734194700000000008378345892300000000000001394568376700000324000543630640000004493893473278700035645005669047476411111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111152687296422452231917546188873608485063898839418833294108373039867537613755270771001836208119108893430645836236", 10, NULL);
	/* clang-format on */
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_subtract_int(tau->num1, LLONG_MAX);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}
