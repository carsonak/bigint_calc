#include "test_math.h"

TEST(invalid_inputs, test_NULL) { CHECK(bi_iszero(NULL) == false); }

struct null_array
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(null_array) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(null_array) { tau->output = bi_delete(tau->output); }

TEST_F(null_array, test_nullarray_len0)
{
	CHECK(bi_iszero(&(tau->num1)) == false);
}

TEST_F(null_array, test_nullarray_len0_neg)
{
	tau->num1.is_negative = true;
	CHECK(bi_iszero(&(tau->num1)) == false);
}

TEST_F(null_array, test_nullarray_len1)
{
	tau->num1.len = 1;
	CHECK(bi_iszero(&(tau->num1)) == false);
}

TEST_F(null_array, test_nullarray_len1_neg)
{
	tau->num1.len = 1;
	tau->num1.is_negative = true;
	CHECK(bi_iszero(&(tau->num1)) == false);
}

struct incorrect_length
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(incorrect_length) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(incorrect_length) { tau->output = bi_delete(tau->output); }

TEST_F(incorrect_length, test_all_zeros)
{
	digit_ty in1[] = {0, 0, 0, 0, 0};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};

	CHECK(bi_iszero(&(tau->num1)) == true);
}

TEST_F(incorrect_length, test_lsd_is1)
{
	digit_ty in1[] = {1, 0, 0, 0, 0};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};

	CHECK(bi_iszero(&(tau->num1)) == false);
}

TEST_F(incorrect_length, test_middle_is1)
{
	digit_ty in1[] = {0, 0, 1, 0, 0};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};

	CHECK(bi_iszero(&(tau->num1)) == false);
}

struct incorrect_length_and_negative
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(incorrect_length_and_negative) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(incorrect_length_and_negative)
{
	tau->output = bi_delete(tau->output);
}

TEST_F(incorrect_length_and_negative, test_all_zeros)
{
	digit_ty in1[] = {0, 0, 0, 0, 0};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};

	CHECK(bi_iszero(&(tau->num1)) == true);
}

TEST_F(incorrect_length_and_negative, test_lsd_is1)
{
	digit_ty in1[] = {1, 0, 0, 0, 0};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};

	CHECK(bi_iszero(&(tau->num1)) == false);
}

TEST_F(incorrect_length_and_negative, test_middle_is1)
{
	digit_ty in1[] = {0, 0, 1, 0, 0};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};

	CHECK(bi_iszero(&(tau->num1)) == false);
}

struct normal_input
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(normal_input) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(normal_input) { tau->output = bi_delete(tau->output); }

TEST_F(normal_input, test_0)
{
	tau->num1 =
		(bigint){.len = 1, .is_negative = false, .num = (digit_ty[1]){0}};

	CHECK(bi_iszero(&(tau->num1)) == true);
}

TEST_F(normal_input, test_neg0)
{
	tau->num1 =
		(bigint){.len = 1, .is_negative = true, .num = (digit_ty[1]){0}};

	CHECK(bi_iszero(&(tau->num1)) == true);
}

TEST_F(normal_input, test_1)
{
	tau->num1 =
		(bigint){.len = 1, .is_negative = false, .num = (digit_ty[1]){1}};

	CHECK(bi_iszero(&(tau->num1)) == false);
}

TEST_F(normal_input, test_neg1)
{
	tau->num1 =
		(bigint){.len = 1, .is_negative = true, .num = (digit_ty[1]){1}};

	CHECK(bi_iszero(&(tau->num1)) == false);
}

TEST_F(normal_input, test_msd_is1)
{
	digit_ty in1[] = {0, 0, 0, 0, 1};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};

	CHECK(bi_iszero(&(tau->num1)) == false);
}

TEST_F(normal_input, test_msd_is_neg1)
{
	digit_ty in1[] = {0, 0, 0, 0, 1};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};

	CHECK(bi_iszero(&(tau->num1)) == false);
}
