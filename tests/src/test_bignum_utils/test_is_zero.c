#include "tests.h"

bignum num1 = {.len = 0, .is_negative = false, .num = NULL};

/**
 * setup - initialises variables for tests.
 */
void setup(void)
{
	num1 = (bignum){.len = 0, .is_negative = false, .num = NULL};
}

/**
 * teardown - resets variables for tests.
 */
void teardown(void) {}

TestSuite(null_inputs);

Test(null_inputs, test_NULL, .description = "NULL", .timeout = 2.0)
{
	cr_assert(zero(chr, is_zero(NULL)));
}

TestSuite(null_array, .init = setup, .fini = teardown);

Test(null_array, test_nullarray_len0,
	 .description = "len = 0; null_array;", .timeout = 2.0)
{
	cr_assert(eq(chr, is_zero(&num1), true));
}

Test(null_array, test_nullarray_len0_neg,
	 .description = "len = 0; is_negative; null_array;", .timeout = 2.0)
{
	num1.is_negative = true;
	cr_assert(eq(chr, is_zero(&num1), true));
}

Test(null_array, test_nullarray_len1,
	 .description = "len = 1; null_array;", .timeout = 2.0)
{
	num1.len = 1;
	cr_assert(eq(chr, is_zero(&num1), true));
}

Test(null_array, test_nullarray_len1_neg,
	 .description = "len = 1; is_negative; null_array;", .timeout = 2.0)
{
	num1.len = 1;
	num1.is_negative = true;
	cr_assert(eq(chr, is_zero(&num1), true));
}

TestSuite(incorrect_length, .init = setup, .fini = teardown);

Test(incorrect_length, test_all_zeros,
	 .description = "{0,0,0,0,0}", .timeout = 2.0)
{
	u_int in1[] = {0, 0, 0, 0, 0};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};

	cr_assert(eq(chr, is_zero(&num1), true));
}

Test(incorrect_length, test_lsd_is1,
	 .description = "{1,0,0,0,0}", .timeout = 2.0)
{
	u_int in1[] = {1, 0, 0, 0, 0};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};

	cr_assert(eq(chr, is_zero(&num1), false));
}

Test(incorrect_length, test_middle_is1,
	 .description = "{0,0,1,0,0}", .timeout = 2.0)
{
	u_int in1[] = {0, 0, 1, 0, 0};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};

	cr_assert(eq(chr, is_zero(&num1), false));
}

TestSuite(incorrect_length_and_negative, .init = setup, .fini = teardown);

Test(incorrect_length_and_negative, test_all_zeros,
	 .description = "{0,0,0,0,0}", .timeout = 2.0)
{
	u_int in1[] = {0, 0, 0, 0, 0};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};

	cr_assert(eq(chr, is_zero(&num1), true));
}

Test(incorrect_length_and_negative, test_lsd_is1,
	 .description = "{1,0,0,0,0}", .timeout = 2.0)
{
	u_int in1[] = {1, 0, 0, 0, 0};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};

	cr_assert(eq(chr, is_zero(&num1), false));
}

Test(incorrect_length_and_negative, test_middle_is1,
	 .description = "{0,0,1,0,0}", .timeout = 2.0)
{
	u_int in1[] = {0, 0, 1, 0, 0};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};

	cr_assert(eq(chr, is_zero(&num1), false));
}

TestSuite(normal_input, .init = setup, .fini = teardown);

Test(normal_input, test_0, .description = "0", .timeout = 2.0)
{
	num1 = (bignum){.len = 1, .is_negative = false, .num = (u_int[1]){0}};

	cr_assert(eq(chr, is_zero(&num1), true));
}

Test(normal_input, test_neg0, .description = "-0", .timeout = 2.0)
{
	num1 = (bignum){.len = 1, .is_negative = true, .num = (u_int[1]){0}};

	cr_assert(eq(chr, is_zero(&num1), true));
}

Test(normal_input, test_1, .description = "1", .timeout = 2.0)
{
	num1 = (bignum){.len = 1, .is_negative = false, .num = (u_int[1]){1}};

	cr_assert(eq(chr, is_zero(&num1), false));
}

Test(normal_input, test_neg1, .description = "-1", .timeout = 2.0)
{
	num1 = (bignum){.len = 1, .is_negative = true, .num = (u_int[1]){1}};

	cr_assert(eq(chr, is_zero(&num1), false));
}

Test(normal_input, test_msd_is1,
	 .description = "{0,0,0,0,1}", .timeout = 2.0)
{
	u_int in1[] = {0, 0, 0, 0, 1};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};

	cr_assert(eq(chr, is_zero(&num1), false));
}

Test(normal_input, test_msd_is_neg1,
	 .description = "{0,0,0,0,1}", .timeout = 2.0)
{
	u_int in1[] = {0, 0, 0, 0, 1};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};

	cr_assert(eq(chr, is_zero(&num1), false));
}
