#include "tests.h"

char *output = NULL;

/**
 * setup - setup some variables.
 */
void setup(void) {}

/**
 * teardown - cleanup some variables.
 */
void teardown(void)
{
	output = free_n_null(output);
}

TestSuite(null_inputs);

Test(null_inputs, test_NULL_len0, .description = "(NULL, 0)", .timeout = 2.0)
{
	output = uintarray_to_str(NULL, 0);

	cr_assert(eq(str, output, "{NULL}"));
}

Test(null_inputs, test_0_len0, .description = "({0}, 0)", .timeout = 2.0)
{
	uint arr[] = {0};

	output = uintarray_to_str(arr, 0);

	cr_assert(eq(str, output, "{NULL}"));
}

Test(null_inputs, test_NULL_len1, .description = "(NULL, 1)", .timeout = 2.0)
{
	output = uintarray_to_str(NULL, 1);

	cr_assert(eq(str, output, "{NULL}"));
}

TestSuite(len1_inputs, .init = setup, .fini = teardown);

Test(len1_inputs, test_0_len1, .description = "({0}, 1)", .timeout = 2.0)
{
	uint arr[] = {0};

	output = uintarray_to_str(arr, sizeof(arr) / sizeof(*arr));

	cr_assert(eq(str, output, "{0}"));
}

Test(len1_inputs, test_1_len1, .description = "({1}, 1)", .timeout = 2.0)
{
	uint arr[] = {1};

	output = uintarray_to_str(arr, sizeof(arr) / sizeof(*arr));

	cr_assert(eq(str, output, "{1}"));
}

Test(len1_inputs, test_999999999_len1, .description = "({999999999}, 1)",
	 .timeout = 2.0)
{
	uint arr[] = {999999999};

	output = uintarray_to_str(arr, sizeof(arr) / sizeof(*arr));

	cr_assert(eq(str, output, "{999999999}"));
}

TestSuite(longer_inputs, .init = setup, .fini = teardown);

Test(longer_inputs, test_1_2_len2, .description = "({1, 2}, 2)",
	 .timeout = 2.0)
{
	uint arr[] = {1, 2};

	output = uintarray_to_str(arr, sizeof(arr) / sizeof(*arr));

	cr_assert(eq(str, output, "{1, 2}"));
}

Test(longer_inputs, test_1_2_3_len3, .description = "({1, 2, 3}, 3)",
	 .timeout = 2.0)
{
	uint arr[] = {1, 2, 3};

	output = uintarray_to_str(arr, sizeof(arr) / sizeof(*arr));

	cr_assert(eq(str, output, "{1, 2, 3}"));
}

Test(longer_inputs, test_1_2_3_4_len4, .description = "({1, 2, 3, 4}, 4)",
	 .timeout = 2.0)
{
	uint arr[] = {1, 2, 3, 4};

	output = uintarray_to_str(arr, sizeof(arr) / sizeof(*arr));

	cr_assert(eq(str, output, "{1, 2, 3, 4}"));
}

Test(longer_inputs, test_9s_len5,
	 .description = "({999999999, 999999999, 999999999, 999999999, 999999999}, 5)",
	 .timeout = 2.0)
{
	uint arr[] = {999999999, 999999999, 999999999, 999999999, 999999999};

	output = uintarray_to_str(arr, sizeof(arr) / sizeof(*arr));

	cr_assert(eq(str, output, "{999999999, 999999999, 999999999, 999999999, 999999999}"));
}
