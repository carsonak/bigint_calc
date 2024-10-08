#include "tests.h"

bignum in1 = {0};

/**
 * setup - setup some variables.
 */
void setup(void)
{
	in1 = (bignum){0};
}

/**
 * teardown - cleanup some variables.
 */
void teardown(void) {}

TestSuite(int_to_bignum_malloc);

Test(int_to_bignum_malloc, test_0, .description = "0", .timeout = 2.0)
{
	bignum expected = {.len = 1, .is_negative = false, .num = (u_int[]){0}};
	bignum *output = int_to_bignuma(0);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bn_free(output);
}

Test(int_to_bignum_malloc, test_neg_1, .description = "-1", .timeout = 2.0)
{
	bignum expected = {.len = 1, .is_negative = true, .num = (u_int[]){1}};
	bignum *output = int_to_bignuma(-1);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bn_free(output);
}

Test(int_to_bignum_malloc, test_1000000000, .description = "1,000,000,000", .timeout = 2.0)
{
	bignum expected = {.len = 2, .is_negative = false, .num = (u_int[]){0, 1}};
	bignum *output = int_to_bignuma(1000000000);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bn_free(output);
}

Test(int_to_bignum_malloc, test_neg_1000000000,
	 .description = "-1,000,000,000", .timeout = 2.0)
{
	bignum expected = {.len = 2, .is_negative = true, .num = (u_int[]){0, 1}};
	bignum *output = int_to_bignuma(-1000000000);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bn_free(output);
}

Test(int_to_bignum_malloc, test_LLONG_MIN,
	 .description = "LLONG_MIN + 1 (-9223372036854775807)", .timeout = 2.0)
{
	bignum expected = {
		.len = 3, .is_negative = true, .num = (u_int[]){854775807, 223372036, 9}};
	bignum *output = int_to_bignuma(LLONG_MIN + 1);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bn_free(output);
}

Test(int_to_bignum_malloc, test_LLONG_MAX,
	 .description = "LLONG_MAX (9223372036854775807)", .timeout = 2.0)
{
	bignum expected = {
		.len = 3, .is_negative = false, .num = (u_int[]){854775807, 223372036, 9}};
	bignum *output = int_to_bignuma(LLONG_MAX);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bn_free(output);
}

TestSuite(int_to_bignum_nomalloc, .init = setup, .fini = teardown);

Test(int_to_bignum_nomalloc, test_NULL,
	 .description = "NULL parameter", .timeout = 2.0)
{
	cr_expect(zero(chr, int_to_bignum(NULL, 0)));
}

Test(int_to_bignum_nomalloc, test_0_len,
	 .description = "bignum with length 0", .timeout = 2.0)
{
	in1.num = (u_int[1]){0};
	cr_expect(zero(chr, int_to_bignum(&in1, 0)));
}

Test(int_to_bignum_nomalloc, test_nullarray,
	 .description = "bignum with a null array", .timeout = 2.0)
{
	in1.len = 1;
	cr_expect(zero(chr, int_to_bignum(&in1, 0)));
}

Test(int_to_bignum_nomalloc, test_0, .description = "0", .timeout = 2.0)
{
	bignum expected = {.len = 1, .is_negative = false, .num = (u_int[]){0}};

	in1 = (bignum){.len = 1, .is_negative = false, .num = (u_int[]){0}};

	cr_expect(eq(chr, int_to_bignum(&in1, 0), 1));
	cr_expect(eq(sz, in1.len, expected.len));
	cr_expect(eq(chr, in1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], in1.num, expected.num));
}

Test(int_to_bignum_nomalloc, test_neg_1, .description = "-1", .timeout = 2.0)
{
	bignum expected = {.len = 1, .is_negative = true, .num = (u_int[]){1}};

	in1 = (bignum){.len = 1, .is_negative = false, .num = (u_int[]){0}};

	cr_expect(eq(chr, int_to_bignum(&in1, -1), 1));
	cr_expect(eq(sz, in1.len, expected.len));
	cr_expect(eq(chr, in1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], in1.num, expected.num));
}

Test(int_to_bignum_nomalloc, test_1000000000, .description = "1,000,000,000", .timeout = 2.0)
{
	bignum expected = {.len = 2, .is_negative = false, .num = (u_int[]){0, 1}};

	in1 = (bignum){.len = 2, .is_negative = false, .num = (u_int[2]){0}};

	cr_expect(eq(chr, int_to_bignum(&in1, 1000000000), 1));
	cr_expect(eq(sz, in1.len, expected.len));
	cr_expect(eq(chr, in1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], in1.num, expected.num));
}

Test(int_to_bignum_nomalloc, test_neg_1000000000,
	 .description = "-1,000,000,000", .timeout = 2.0)
{
	bignum expected = {.len = 2, .is_negative = true, .num = (u_int[]){0, 1}};

	in1 = (bignum){.len = 2, .is_negative = false, .num = (u_int[2]){0}};

	cr_expect(eq(chr, int_to_bignum(&in1, -1000000000), 1));
	cr_expect(eq(sz, in1.len, expected.len));
	cr_expect(eq(chr, in1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], in1.num, expected.num));
}

Test(int_to_bignum_nomalloc, test_LLONG_MIN,
	 .description = "LLONG_MIN + 1 (-9223372036854775807)", .timeout = 2.0)
{
	bignum expected = {
		.len = 3, .is_negative = true, .num = (u_int[]){854775807, 223372036, 9}};

	in1 = (bignum){.len = 3, .is_negative = false, .num = (u_int[3]){0}};

	cr_expect(eq(chr, int_to_bignum(&in1, LLONG_MIN + 1), 1));
	cr_expect(eq(sz, in1.len, expected.len));
	cr_expect(eq(chr, in1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], in1.num, expected.num));
}

Test(int_to_bignum_nomalloc, test_LLONG_MAX,
	 .description = "LLONG_MAX (9223372036854775807)", .timeout = 2.0)
{
	bignum expected = {
		.len = 3, .is_negative = false, .num = (u_int[]){854775807, 223372036, 9}};

	in1 = (bignum){.len = 3, .is_negative = false, .num = (u_int[3]){0}};

	cr_expect(eq(chr, int_to_bignum(&in1, LLONG_MAX), 1));
	cr_expect(eq(sz, in1.len, expected.len));
	cr_expect(eq(chr, in1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], in1.num, expected.num));
}
