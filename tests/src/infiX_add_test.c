#include "tests.h"

/**
 * trim_u4b_array - trims zeros from end of an array
 * @arr: pointer to a u4b_array struct
 */
void trim_u4b_array(u4b_array *arr)
{
	if (!arr)
		return;

	while (!arr->array[arr->len - 1] && arr->len > 1)
		--arr->len;
}

u4b_array *num1 = NULL;
u4b_array *num2 = NULL;
u4b_array *expected = NULL;

/**
 * setup_arrays - allocates memory for arrays
 */
void setup_arrays(void)
{
	num1 = check_malloc(1 * sizeof(*num1));
	num2 = check_malloc(1 * sizeof(*num2));
	expected = check_malloc(1 * sizeof(*expected));
	if (!num1 || !num2 || !expected)
		exit(1);

	num1->array = check_malloc(11 * sizeof(num1->array));
	num2->array = check_malloc(11 * sizeof(num2->array));
	expected->array = check_malloc(11 * sizeof(expected->array));
	if (!num1->array || !num2->array || !expected->array)
		exit(1);
}

/**
 * teardown_arrays - deallocates memory for arrays
 */
void teardown_arrays(void)
{
	free_u4b_array(num1);
	free_u4b_array(num2);
	free_u4b_array(expected);
}

TestSuite(simple_additions, .init = setup_arrays, .fini = teardown_arrays);

Test(simple_additions, test_0_plus_0,
	 .description = "0 + 0 = 0", .timeout = 0.5)
{
	uint32_t in1[] = {0}, out[] = {0};

	num1->len = sizeof(in1) / sizeof(*in1);
	num1->is_negative = 0;
	memmove(num1->array, in1, sizeof(in1));
	expected->len = sizeof(out) / sizeof(*out);
	expected->is_negative = 0;
	memmove(expected->array, out, sizeof(out));

	u4b_array *output = infiX_addition(num1, num1);

	cr_expect(eq(sz, output->len, expected->len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected->len], output->array, expected->array));
}

Test(simple_additions, test_1_plus_0,
	 .description = "1 + 0 = 1", .timeout = 0.5)
{
	uint32_t in1[] = {1}, in2[] = {0}, out[] = {1};

	num1->len = sizeof(in1) / sizeof(*in1);
	num1->is_negative = 0;
	memmove(num1->array, in1, sizeof(in1));
	num2->len = sizeof(in2) / sizeof(*in2);
	num2->is_negative = 0;
	memmove(num2->array, in2, sizeof(in2));
	expected->len = sizeof(out) / sizeof(*out);
	expected->is_negative = 0;
	memmove(expected->array, out, sizeof(out));

	u4b_array *output = infiX_addition(num1, num2);

	cr_expect(eq(sz, output->len, expected->len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected->len], output->array, expected->array));
}

Test(simple_additions, test_0_plus_1,
	 .description = "0 + 1 = 1", .timeout = 0.5)
{
	uint32_t in1[] = {0}, in2[] = {1}, out[] = {1};

	num1->len = sizeof(in1) / sizeof(*in1);
	num1->is_negative = 0;
	memmove(num1->array, in1, sizeof(in1));
	num2->len = sizeof(in2) / sizeof(*in2);
	num2->is_negative = 0;
	memmove(num2->array, in2, sizeof(in2));
	expected->len = sizeof(out) / sizeof(*out);
	expected->is_negative = 0;
	memmove(expected->array, out, sizeof(out));

	u4b_array *output = infiX_addition(num1, num2);

	cr_expect(eq(sz, output->len, expected->len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected->len], output->array, expected->array));
}

Test(simple_additions, test_1_plus_1,
	 .description = "1 + 1 = 2", .timeout = 0.5)
{
	uint32_t in1[] = {1}, out[] = {2};

	num1->len = sizeof(in1) / sizeof(*in1);
	num1->is_negative = 0;
	memmove(num1->array, in1, sizeof(in1));
	expected->len = sizeof(out) / sizeof(*out);
	expected->is_negative = 0;
	memmove(expected->array, out, sizeof(out));

	u4b_array *output = infiX_addition(num1, num1);

	cr_expect(eq(sz, output->len, expected->len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected->len], output->array, expected->array));
}

Test(simple_additions, test_999999999_plus_1,
	 .description = "999,999,999 + 1 = 1,000,000,000", .timeout = 0.5)
{
	uint32_t in1[] = {1000000000 - 1}, in2[] = {1}, out[] = {0, 1};

	num1->len = sizeof(in1) / sizeof(*in1);
	num1->is_negative = 0;
	memmove(num1->array, in1, sizeof(in1));
	num2->len = sizeof(in2) / sizeof(*in2);
	num2->is_negative = 0;
	memmove(num2->array, in2, sizeof(in2));
	expected->len = sizeof(out) / sizeof(*out);
	expected->is_negative = 0;
	memmove(expected->array, out, sizeof(out));

	u4b_array *output = infiX_addition(num1, num2);

	cr_expect(eq(sz, output->len, expected->len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected->len], output->array, expected->array));
}

Test(simple_additions, test_999999999_plus_999999999,
	 .description = "999,999,999 + 999,999,999 = 1,999,999,998",
	 .timeout = 0.5)
{
	uint32_t in1[] = {1000000000 - 1}, in2[] = {1000000000 - 1};
	uint32_t out[] = {999999998, 1};

	num1->len = sizeof(in1) / sizeof(*in1);
	num1->is_negative = 0;
	memmove(num1->array, in1, sizeof(in1));
	num2->len = sizeof(in2) / sizeof(*in2);
	num2->is_negative = 0;
	memmove(num2->array, in2, sizeof(in2));
	expected->len = sizeof(out) / sizeof(*out);
	expected->is_negative = 0;
	memmove(expected->array, out, sizeof(out));

	u4b_array *output = infiX_addition(num1, num2);

	cr_expect(eq(sz, output->len, expected->len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected->len], output->array, expected->array));
}

Test(simple_additions, test_minus1_plus_1,
	 .description = "-1 + 1 = 0", .timeout = 0.5)
{
	uint32_t in1[] = {1}, in2[] = {1};
	uint32_t out[] = {0};

	num1->len = sizeof(in1) / sizeof(*in1);
	num1->is_negative = 1;
	memmove(num1->array, in1, sizeof(in1));
	num2->len = sizeof(in2) / sizeof(*in2);
	num2->is_negative = 0;
	memmove(num2->array, in2, sizeof(in2));
	expected->len = sizeof(out) / sizeof(*out);
	expected->is_negative = 0;
	memmove(expected->array, out, sizeof(out));

	u4b_array *output = infiX_addition(num1, num2);

	cr_expect(eq(sz, output->len, expected->len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected->len], output->array, expected->array));
}

Test(simple_additions, test_1_plus_minus1,
	 .description = "1 + -1 = 0", .timeout = 0.5)
{
	uint32_t in1[] = {1}, in2[] = {1};
	uint32_t out[] = {0};

	num1->len = sizeof(in1) / sizeof(*in1);
	num1->is_negative = 0;
	memmove(num1->array, in1, sizeof(in1));
	num2->len = sizeof(in2) / sizeof(*in2);
	num2->is_negative = 1;
	memmove(num2->array, in2, sizeof(in2));
	expected->len = sizeof(out) / sizeof(*out);
	expected->is_negative = 0;
	memmove(expected->array, out, sizeof(out));

	u4b_array *output = infiX_addition(num1, num2);

	cr_expect(eq(sz, output->len, expected->len));
	cr_expect(zero(chr, output->is_negative, expected->is_negative));
	cr_expect(eq(u32[expected->len], output->array, expected->array));
}

Test(simple_additions, test_minus1_plus_minus1,
	 .description = "-1 + -1 = -2", .timeout = 0.5)
{
	uint32_t in1[] = {1}, in2[] = {1};
	uint32_t out[] = {2};

	num1->len = sizeof(in1) / sizeof(*in1);
	num1->is_negative = 1;
	memmove(num1->array, in1, sizeof(in1));
	num2->len = sizeof(in2) / sizeof(*in2);
	num2->is_negative = 1;
	memmove(num2->array, in2, sizeof(in2));
	expected->len = sizeof(out) / sizeof(*out);
	expected->is_negative = 1;
	memmove(expected->array, out, sizeof(out));

	u4b_array *output = infiX_addition(num1, num2);

	cr_expect(eq(sz, output->len, expected->len));
	cr_expect(eq(chr, output->is_negative, expected->is_negative));
	cr_expect(eq(u32[expected->len], output->array, expected->array));
}

Test(simple_additions, test_100000000_plus_minus50000,
	 .description = "100,000,000 + -50,000 = 99,950,000",
	 .timeout = 0.5)
{
	uint32_t in1[] = {100000000}, in2[] = {50000};
	uint32_t out[] = {99950000};

	num1->len = sizeof(in1) / sizeof(*in1);
	num1->is_negative = 0;
	memmove(num1->array, in1, sizeof(in1));
	num2->len = sizeof(in2) / sizeof(*in2);
	num2->is_negative = 1;
	memmove(num2->array, in2, sizeof(in2));
	expected->len = sizeof(out) / sizeof(*out);
	expected->is_negative = 0;
	memmove(expected->array, out, sizeof(out));

	u4b_array *output = infiX_addition(num1, num2);

	cr_expect(eq(sz, output->len, expected->len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected->len], output->array, expected->array));
}

Test(simple_additions, test_minus50000_plus_100000000,
	 .description = "-100,000,000 + 50,000 = -99,950,000",
	 .timeout = 0.5)
{
	uint32_t in1[] = {100000000}, in2[] = {50000};
	uint32_t out[] = {99950000};

	num1->len = sizeof(in1) / sizeof(*in1);
	num1->is_negative = 1;
	memmove(num1->array, in1, sizeof(in1));
	num2->len = sizeof(in2) / sizeof(*in2);
	num2->is_negative = 0;
	memmove(num2->array, in2, sizeof(in2));
	expected->len = sizeof(out) / sizeof(*out);
	expected->is_negative = 1;
	memmove(expected->array, out, sizeof(out));

	u4b_array *output = infiX_addition(num1, num2);

	cr_expect(eq(sz, output->len, expected->len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected->len], output->array, expected->array));
}
