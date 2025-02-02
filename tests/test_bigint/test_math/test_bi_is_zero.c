#include "tests.h"

bigint num1 = {.len = 0, .is_negative = false, .num = NULL};

/**
 * setup - initialises variables for tests.
 */
void setup(void)
{
	num1 = (bigint){.len = 0, .is_negative = false, .num = NULL};
}

/**
 * teardown - resets variables for tests.
 */
void teardown(void) {}

TestSuite(null_inputs);

TEST(null_inputs, test_NULL) { cr_assert(zero(chr, bi_is_zero(NULL))); }

struct null_array
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(null_array)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(null_array) {}

TEST(null_array, test_nullarray_len0) { EXPECT(bi_is_zero(&num1) == true, ""); }

TEST(null_array, test_nullarray_len0_neg)
{
	num1.is_negative = true;
	EXPECT(bi_is_zero(&num1) == true, "");
}

TEST(null_array, test_nullarray_len1)
{
	num1.len = 1;
	EXPECT(bi_is_zero(&num1) == true, "");
}

TEST(null_array, test_nullarray_len1_neg)
{
	num1.len = 1;
	num1.is_negative = true;
	EXPECT(bi_is_zero(&num1) == true, "");
}

struct incorrect_length
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(incorrect_length)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(incorrect_length) {}

TEST(incorrect_length, test_all_zeros)
{
	u_int in1[] = {0, 0, 0, 0, 0};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};

	EXPECT(bi_is_zero(&num1) == true, "");
}

TEST(incorrect_length, test_lsd_is1)
{
	u_int in1[] = {1, 0, 0, 0, 0};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};

	EXPECT(bi_is_zero(&num1) == false, "");
}

TEST(incorrect_length, test_middle_is1)
{
	u_int in1[] = {0, 0, 1, 0, 0};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};

	EXPECT(bi_is_zero(&num1) == false, "");
}

struct incorrect_length_and_negative
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(incorrect_length_and_negative)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(incorrect_length_and_negative) {}

TEST(incorrect_length_and_negative, test_all_zeros)
{
	u_int in1[] = {0, 0, 0, 0, 0};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};

	EXPECT(bi_is_zero(&num1) == true, "");
}

TEST(incorrect_length_and_negative, test_lsd_is1)
{
	u_int in1[] = {1, 0, 0, 0, 0};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};

	EXPECT(bi_is_zero(&num1) == false, "");
}

TEST(incorrect_length_and_negative, test_middle_is1)
{
	u_int in1[] = {0, 0, 1, 0, 0};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};

	EXPECT(bi_is_zero(&num1) == false, "");
}

struct normal_input
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(normal_input)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(normal_input) {}

TEST(normal_input, test_0)
{
	num1 = (bigint){.len = 1, .is_negative = false, .num = (u_int[1]){0}};

	EXPECT(bi_is_zero(&num1) == true, "");
}

TEST(normal_input, test_neg0)
{
	num1 = (bigint){.len = 1, .is_negative = true, .num = (u_int[1]){0}};

	EXPECT(bi_is_zero(&num1) == true, "");
}

TEST(normal_input, test_1)
{
	num1 = (bigint){.len = 1, .is_negative = false, .num = (u_int[1]){1}};

	EXPECT(bi_is_zero(&num1) == false, "");
}

TEST(normal_input, test_neg1)
{
	num1 = (bigint){.len = 1, .is_negative = true, .num = (u_int[1]){1}};

	EXPECT(bi_is_zero(&num1) == false, "");
}

TEST(normal_input, test_msd_is1)
{
	u_int in1[] = {0, 0, 0, 0, 1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};

	EXPECT(bi_is_zero(&num1) == false, "");
}

TEST(normal_input, test_msd_is_neg1)
{
	u_int in1[] = {0, 0, 0, 0, 1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};

	EXPECT(bi_is_zero(&num1) == false, "");
}
