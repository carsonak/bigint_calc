#include "tests.h"

uint32_t blank1[10] = {0};
u4b_array num1 = {.len = 0, .is_negative = 0, .array = blank1};

uint32_t blank2[10] = {0};
u4b_array num2 = {.len = 0, .is_negative = 0, .array = blank2};

uint32_t blank3[10] = {0};
u4b_array expected = {.len = 0, .is_negative = 0, .array = blank3};

/**
 * trim_u4b_array - Dummy: trims zeros from end of an array
 * @arr: pointer to a u4b_array struct
 */
void trim_u4b_array(u4b_array *arr)
{
	if (!arr)
		return;

	while (!arr->array[arr->len - 1] && arr->len > 1)
		--arr->len;
}

/**
 * infiX_addition - Dummy
 * @n1: unused
 * @n2: unused
 *
 * Return: empty array, NULL on error
 */
u4b_array *infiX_addition(u4b_array *n1, u4b_array *n2)
{
	if (!n1 || !n2)
		return (NULL);

	return (alloc_u4b_array(1));
}

/**
 * setup - setups some variables for tests
 */
void setup(void)
{
	return;
}

/**
 * teardown - resets variables for tests.
 */
void teardown(void)
{
	num1.len = 0;
	num2.len = 0;
	expected.len = 0;
	memset(blank1, 0, sizeof(blank1));
	memset(blank2, 0, sizeof(blank1));
	memset(blank3, 0, sizeof(blank1));
}

TestSuite(simple_subtractions, .init = setup, .fini = teardown);

Test(simple_subtractions, test_100000000_plus_minus50000,
	 .description = "100,000,000 + -50,000 = 99,950,000",
	 .timeout = 0.5)
{
	uint32_t in1[] = {100000000}, in2[] = {50000};

	num1.len = sizeof(in1) / sizeof(*in1);
	memmove(num1.array, in1, sizeof(in1));

	num2.len = sizeof(in2) / sizeof(*in2);
	num2.is_negative = 1;
	memmove(num2.array, in2, sizeof(in2));

	u4b_array *output = infiX_subtraction(&num1, &num2);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}

Test(simple_subtractions, test_minus50000_plus_100000000,
	 .description = "-100,000,000 + 50,000 = -99,950,000",
	 .timeout = 0.5)
{
	uint32_t in1[] = {100000000}, in2[] = {50000};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.is_negative = 1;
	memmove(num1.array, in1, sizeof(in1));

	num2.len = sizeof(in2) / sizeof(*in2);
	memmove(num2.array, in2, sizeof(in2));

	u4b_array *output = infiX_subtraction(&num1, &num2);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}

Test(negative_additions, test_minus1_plus_1,
	 .description = "-1 + 1 = 0", .timeout = 0.5)
{
	uint32_t in1[] = {1}, in2[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.is_negative = 1;
	memmove(num1.array, in1, sizeof(in1));

	num2.len = sizeof(in2) / sizeof(*in2);
	memmove(num2.array, in2, sizeof(in2));

	u4b_array *output = infiX_addition(&num1, &num2);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}

Test(negative_additions, test_1_plus_minus1,
	 .description = "1 + -1 = 0", .timeout = 0.5)
{
	uint32_t in1[] = {1}, in2[] = {1};
	uint32_t out[] = {0};

	num1.len = sizeof(in1) / sizeof(*in1);
	memmove(num1.array, in1, sizeof(in1));

	num2.len = sizeof(in2) / sizeof(*in2);
	num2.is_negative = 1;
	memmove(num2.array, in2, sizeof(in2));

	expected.len = sizeof(out) / sizeof(*out);
	memmove(expected.array, out, sizeof(out));

	u4b_array *output = infiX_addition(&num1, &num2);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	// cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}
