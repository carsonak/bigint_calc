#include "tests.h"

bignum base = {.len = 0, .is_negative = false, .num = NULL};
bignum exponent = {.len = 0, .is_negative = false, .num = NULL};
bignum expected = {.len = 0, .is_negative = false, .num = NULL};

/**
 * setup - initialises variables for tests.
 */
void setup(void) {}

/**
 * teardown - resets variables for tests.
 */
void teardown(void)
{
	base.len = 0;
	base.is_negative = false;
	base.num = NULL;

	exponent.len = 0;
	exponent.is_negative = false;
	exponent.num = NULL;

	expected.len = 0;
	expected.is_negative = false;
	expected.num = NULL;
}

TestSuite(null_inputs, .init = setup, .fini = teardown);

Test(null_inputs, test_null_pow_null,
	 .description = "NULL ^ NULL = NULL", .timeout = 3.0)
{
	bignum *output = bn_power(NULL, NULL);

	cr_expect(zero(ptr, output));
}

Test(null_inputs, test_1_pow_null,
	 .description = "1 ^ NULL = NULL", .timeout = 3.0)
{
	uint bs[] = {1};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	bignum *output = bn_power(&base, NULL);

	cr_expect(zero(ptr, output));
}

Test(null_inputs, test_null_pow_1,
	 .description = "NULL ^ 1 = NULL", .timeout = 3.0)
{
	uint exp[] = {1};

	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	bignum *output = bn_power(NULL, &exponent);

	cr_expect(zero(ptr, output));
}

Test(null_inputs, test_0_pow_null,
	 .description = "0 ^ NULL = NULL", .timeout = 3.0)
{
	uint bs[] = {0};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	bignum *output = bn_power(&base, NULL);

	cr_expect(zero(ptr, output));
}

Test(null_inputs, test_null_pow_0,
	 .description = "NULL ^ 0 = NULL", .timeout = 3.0)
{
	uint exp[] = {0};

	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	bignum *output = bn_power(NULL, &exponent);

	cr_expect(zero(ptr, output));
}

Test(null_inputs, test_minus1_pow_null,
	 .description = "-1 ^ NULL = NULL", .timeout = 3.0)
{
	uint bs[] = {1};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	base.is_negative = true;
	bignum *output = bn_power(&base, NULL);

	cr_expect(zero(ptr, output));
}

Test(null_inputs, test_null_pow_minus1,
	 .description = "NULL ^ -1 = NULL", .timeout = 3.0)
{
	uint exp[] = {1};

	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	exponent.is_negative = true;
	bignum *output = bn_power(NULL, &exponent);

	cr_expect(zero(ptr, output));
}

TestSuite(zero_len_arrays, .init = setup, .fini = teardown);

Test(zero_len_arrays, test_null_pow_null,
	 .description = "null_array ^ null_array = 0", .timeout = 3.0)
{
	uint out[1] = {0};

	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bignum *output = bn_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = free_bignum(output);
}

Test(zero_len_arrays, test_4490998_pow_null,
	 .description = "4,490,998 ^ null_array = 1", .timeout = 3.0)
{
	uint bs[] = {4490998}, out[1] = {1};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bignum *output = bn_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = free_bignum(output);
}

Test(zero_len_arrays, test_null_pow_largenum,
	 .description = "null_array ^ 99992175,712000569,0,0,6086,232509426,238542068 = 0",
	 .timeout = 3.0)
{
	uint exp[] = {238542068, 232509426, 6086, 0, 0, 712000569, 99992175}, out[1] = {0};

	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bignum *output = bn_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = free_bignum(output);
}

TestSuite(simple_multiplications, .init = setup, .fini = teardown);
