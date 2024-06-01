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

TestSuite(simple_additions);

Test(simple_additions, test_1_plus_1,
	 .description = "1 + 1 = 2", .timeout = 1.0)
{
	uint32_t in1[] = {1}, out[] = {2};
	u4b_array num1 = {
		.len = sizeof(in1) / sizeof(*in1),
		.is_negative = 0,
		.array = in1};
	u4b_array expected = {
		.len = sizeof(out) / sizeof(*out),
		.is_negative = 0,
		.array = out};
	u4b_array *output = infiX_addition(&num1, &num1);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}

Test(simple_additions, test_0_plus_0,
	 .description = "0 + 0 = 0", .timeout = 1.0)
{
	uint32_t in1[] = {0}, out[] = {0};
	u4b_array num1 = {
		.len = sizeof(in1) / sizeof(*in1),
		.is_negative = 0,
		.array = in1};
	u4b_array expected = {
		.len = sizeof(out) / sizeof(*out),
		.is_negative = 0,
		.array = out};
	u4b_array *output = infiX_addition(&num1, &num1);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}
