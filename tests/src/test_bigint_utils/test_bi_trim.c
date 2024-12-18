#include "tests.h"

bigint num1 = {.len = 0, .is_negative = false, .num = NULL};
bigint expected = {.len = 0, .is_negative = false, .num = NULL};

/**
 * setup - initialises variables for tests.
 */
void setup(void)
{
	num1 = (bigint){.len = 0, .is_negative = false, .num = NULL};
	expected = (bigint){.len = 0, .is_negative = false, .num = NULL};
}

/**
 * teardown - resets variables for tests.
 */
void teardown(void) {}

TestSuite(null_inputs);

Test(null_inputs, test_NULL, .description = "NULL", .timeout = 1.0)
{
	bi_trim(NULL);
}

TestSuite(null_array, .init = setup, .fini = teardown);

Test(null_array, test_nullarray, .description = "null_array", .timeout = 1.0)
{
	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(zero(ptr, num1.num));
}

Test(null_array, test_nullarray_neg,
	 .description = "is_negative; null_array", .timeout = 1.0)
{
	num1.is_negative = true;

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(zero(ptr, num1.num));
}

Test(null_array, test_nullarray_len5,
	 .description = "len = 5; null_array", .timeout = 1.0)
{
	num1.len = 5;

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(null_array, test_nullarray_len5_neg,
	 .description = "len = 5; is_negative; null_array", .timeout = 1.0)
{
	num1.len = 5;
	num1.is_negative = true;

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

TestSuite(trailing_zeros, .init = setup, .fini = teardown);

Test(trailing_zeros, test_one_zero, .description = "0", .timeout = 1.0)
{
	u_int in1[1];
	u_int out[] = {0};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(trailing_zeros, test_several_zeros, .description = "{0,0,0,0}", .timeout = 1.0)
{
	u_int in1[] = {0, 0, 0, 0};
	u_int out[] = {0};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(trailing_zeros, test_0001, .description = "{1,0,0,0}", .timeout = 1.0)
{
	u_int in1[] = {1, 0, 0, 0};
	u_int out[] = {1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(trailing_zeros, test_0100, .description = "{0,0,1,0}", .timeout = 1.0)
{
	u_int in1[] = {0, 0, 1, 0};
	u_int out[] = {0, 0, 1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

TestSuite(trailing_zeros_negative, .init = setup, .fini = teardown);

Test(trailing_zeros_negative, test_one_zero, .description = "0", .timeout = 1.0)
{
	u_int in1[1];
	u_int out[] = {0};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(trailing_zeros_negative, test_several_zeros, .description = "{0,0,0,0}", .timeout = 1.0)
{
	u_int in1[] = {0, 0, 0, 0};
	u_int out[] = {0};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(trailing_zeros_negative, test_0001, .description = "{1,0,0,0}", .timeout = 1.0)
{
	u_int in1[] = {1, 0, 0, 0};
	u_int out[] = {1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(trailing_zeros_negative, test_0100, .description = "{0,0,1,0}", .timeout = 1.0)
{
	u_int in1[] = {0, 0, 1, 0};
	u_int out[] = {0, 0, 1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

TestSuite(normal_input, .init = setup, .fini = teardown);

Test(normal_input, test_1, .description = "1", .timeout = 1.0)
{
	u_int in1[] = {1};
	u_int out[] = {1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(normal_input, test_neg1, .description = "is_negative; 1", .timeout = 1.0)
{
	u_int in1[] = {1};
	u_int out[] = {1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(normal_input, test_100, .description = "{0,0,1}", .timeout = 1.0)
{
	u_int in1[] = {0, 0, 1};
	u_int out[] = {0, 0, 1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(normal_input, test_neg100, .description = "is_negative; {0,0,1}", .timeout = 1.0)
{
	u_int in1[] = {0, 0, 1};
	u_int out[] = {0, 0, 1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	bi_trim(&num1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}
