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

TestSuite(null_inputs, .init = setup, .fini = teardown);

Test(null_inputs, test_NULL_len0, .description = "(NULL, 0)", .timeout = 3.0)
{
	output = uint_array_to_str(NULL, 0);

	cr_assert(eq(str, output, "{NULL}"));
}

Test(null_inputs, test_0_len0, .description = "({0}, 0)", .timeout = 3.0)
{
	unsigned int arr[] = {0};

	output = uint_array_to_str(arr, 0);

	cr_assert(eq(str, output, "{NULL}"));
}

Test(null_inputs, test_NULL_len1, .description = "(NULL, 1)", .timeout = 3.0)
{
	output = uint_array_to_str(NULL, 1);

	cr_assert(eq(str, output, "{NULL}"));
}

TestSuite(len1_inputs, .init = setup, .fini = teardown);

Test(len1_inputs, test_0_len1, .description = "({0}, 1)", .timeout = 3.0)
{
	unsigned int arr[] = {0};

	output = uint_array_to_str(arr, sizeof(arr) / sizeof(*arr));

	cr_assert(eq(str, output, "{0}"));
}

Test(len1_inputs, test_1_len1, .description = "({1}, 1)", .timeout = 3.0)
{
	unsigned int arr[] = {1};

	output = uint_array_to_str(arr, sizeof(arr) / sizeof(*arr));

	cr_assert(eq(str, output, "{1}"));
}

Test(len1_inputs, test_999999999_len1, .description = "({999999999}, 1)",
	 .timeout = 3.0)
{
	unsigned int arr[] = {999999999};

	output = uint_array_to_str(arr, sizeof(arr) / sizeof(*arr));

	cr_assert(eq(str, output, "{999999999}"));
}

TestSuite(longer_inputs, .init = setup, .fini = teardown);

Test(longer_inputs, test_1_2_len2, .description = "({1, 2}, 2)",
	 .timeout = 3.0)
{
	unsigned int arr[] = {1, 2};

	output = uint_array_to_str(arr, sizeof(arr) / sizeof(*arr));

	cr_assert(eq(str, output, "{1, 2}"));
}

Test(longer_inputs, test_1_2_3_len3, .description = "({1, 2, 3}, 3)",
	 .timeout = 3.0)
{
	unsigned int arr[] = {1, 2, 3};

	output = uint_array_to_str(arr, sizeof(arr) / sizeof(*arr));

	cr_assert(eq(str, output, "{1, 2, 3}"));
}

Test(longer_inputs, test_1_2_3_4_len4, .description = "({1, 2, 3, 4}, 4)",
	 .timeout = 3.0)
{
	unsigned int arr[] = {1, 2, 3, 4};

	output = uint_array_to_str(arr, sizeof(arr) / sizeof(*arr));

	cr_assert(eq(str, output, "{1, 2, 3, 4}"));
}

Test(longer_inputs, test_9s_len5,
	 .description = "({999999999, 999999999, 999999999, 999999999, 999999999}, 5)",
	 .timeout = 3.0)
{
	unsigned int arr[] = {999999999, 999999999, 999999999, 999999999, 999999999};

	output = uint_array_to_str(arr, sizeof(arr) / sizeof(*arr));

	cr_assert(eq(str, output, "{999999999, 999999999, 999999999, 999999999, 999999999}"));
}
