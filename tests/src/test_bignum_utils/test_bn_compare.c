#include "tests.h"

bignum num1 = {.len = 0, .is_negative = false, .num = NULL};
bignum num2 = {.len = 0, .is_negative = false, .num = NULL};

/**
 * setup - s.
 */
void setup(void) {}

/**
 * teardown - t.
 */
void teardown(void)
{
	num1.len = 0;
	num1.is_negative = false;
	num1.num = NULL;

	num2.len = 0;
	num2.is_negative = false;
	num2.num = NULL;
}

TestSuite(null_inputs, .init = setup, .fini = teardown);

Test(null_inputs, test_NULL_cmp_NULL,
	 .description = "compare(NULL, NULL) == 0", .timeout = 2.0)
{
	cr_assert(zero(long, bn_compare(NULL, NULL)));
}

Test(null_inputs, test_0_cmp_NULL,
	 .description = "compare(0, NULL) == 0", .timeout = 2.0)
{
	u_int in1[1];

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	cr_assert(zero(long, bn_compare(&num1, NULL)));
}

Test(null_inputs, test_NULL_cmp_0,
	 .description = "compare(NULL, 0) == 0", .timeout = 2.0)
{
	u_int in1[1];

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	cr_assert(zero(long, bn_compare(NULL, &num1)));
}

Test(null_inputs, test_1_cmp_NULL,
	 .description = "compare(1, NULL) == 0", .timeout = 2.0)
{
	u_int in1[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	cr_assert(zero(long, bn_compare(&num1, NULL)));
}

Test(null_inputs, test_NULL_cmp_1,
	 .description = "compare(NULL, 1) == 0", .timeout = 2.0)
{
	u_int in1[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	cr_assert(zero(long, bn_compare(NULL, &num1)));
}

Test(null_arrays, test_NULL_cmp_nullarray,
	 .description = "compare(NULL, null_array) == 0", .timeout = 2.0)
{
	cr_assert(zero(long, bn_compare(NULL, &num2)));
}

Test(null_arrays, test_nullarray_cmp_NULL,
	 .description = "compare(null_array, NULL) == 0", .timeout = 2.0)
{
	cr_assert(zero(long, bn_compare(&num1, NULL)));
}

TestSuite(null_arrays, .init = setup, .fini = teardown);

Test(null_arrays, test_nullarray_cmp_nullarray,
	 .description = "compare(null_array, null_array) == 0", .timeout = 2.0)
{
	cr_assert(zero(long, bn_compare(&num1, &num2)));
}

Test(null_arrays, test_0_cmp_nullarray,
	 .description = "compare(0, null_array) == 1", .timeout = 2.0)
{
	u_int in1[1];

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	cr_assert(eq(long, bn_compare(&num1, &num2), 1));
}

Test(null_arrays, test_nullarray_cmp_0,
	 .description = "compare(null_array, 0) == -1", .timeout = 2.0)
{
	u_int in2[1];

	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;
	cr_assert(eq(long, bn_compare(&num1, &num2), -1));
}

Test(null_arrays, test_1_cmp_nullarray,
	 .description = "compare(1, null_array) == 1", .timeout = 2.0)
{
	u_int in1[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	cr_assert(eq(long, bn_compare(&num1, &num2), 1));
}

Test(null_arrays, test_nullarray_cmp_1,
	 .description = "compare(null_array, 1) == -1", .timeout = 2.0)
{
	u_int in2[] = {1};

	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;
	cr_assert(eq(long, bn_compare(&num1, &num2), -1));
}

TestSuite(trailing_zeros, .init = setup, .fini = teardown);

Test(trailing_zeros, test_long0_cmp_short0,
	 .description = "compare(long_array0, short_array0) > 0", .timeout = 2.0)
{
	u_int in1[] = {1, 8272, 3, 4, 12345, 5, 6, 9, 7, 8, 9284, 1098, 38476, 0, 0, 0, 0, 0, 0};
	u_int in2[] = {9, 8, 49247, 3, 2, 83749, 64154, 99990, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	num1.len = (sizeof(in1) / sizeof(*in1));
	num1.num = in1;
	num2.len = (sizeof(in2) / sizeof(*in2));
	num2.num = in2;

	cr_assert(gt(long, bn_compare(&num1, &num2), 0));
}

Test(trailing_zeros, test_long_cmp_short0,
	 .description = "compare(long_array, short_array0) > 0", .timeout = 2.0)
{
	u_int in1[] = {1, 8272, 3, 4, 12345, 5, 6, 9, 7, 8, 9284, 1098, 38476};
	u_int in2[] = {9, 8, 49247, 3, 2, 83749, 64154, 9999, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	num1.len = (sizeof(in1) / sizeof(*in1));
	num1.num = in1;
	num2.len = (sizeof(in2) / sizeof(*in2));
	num2.num = in2;

	cr_assert(gt(long, bn_compare(&num1, &num2), 0));
}

Test(trailing_zeros, test_long0_cmp_short,
	 .description = "compare(long_array0, short_array) > 0", .timeout = 2.0)
{
	u_int in1[] = {1, 8272, 3, 4, 12345, 5, 6, 9, 7, 8, 9284, 1098, 38476, 0, 0, 0, 0, 0, 0};
	u_int in2[] = {9, 8, 49247, 3, 2, 83749, 64154, 9999};

	num1.len = (sizeof(in1) / sizeof(*in1));
	num1.num = in1;
	num2.len = (sizeof(in2) / sizeof(*in2));
	num2.num = in2;

	cr_assert(gt(long, bn_compare(&num1, &num2), 0));
}

TestSuite(compare_arrays, .init = setup, .fini = teardown);

Test(compare_arrays, test_0_cmp_0,
	 .description = "compare(0, 0) == 0(equal)", .timeout = 2.0)
{
	u_int in1[1];
	u_int in2[1];

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	cr_assert(zero(long, bn_compare(&num1, &num2)));
}

Test(compare_arrays, test_1_cmp_1,
	 .description = "compare(1, 1) == 0(equal)", .timeout = 2.0)
{
	u_int in1[] = {1};
	u_int in2[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	cr_assert(zero(long, bn_compare(&num1, &num2)));
}

Test(compare_arrays, test_1_cmp_0,
	 .description = "compare(1, 0) > 0", .timeout = 2.0)
{
	u_int in1[] = {1};
	u_int in2[1];

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	cr_assert(gt(long, bn_compare(&num1, &num2), 0));
}

Test(compare_arrays, test_0_cmp_1,
	 .description = "compare(0, 1) < 0", .timeout = 2.0)
{
	u_int in1[1];
	u_int in2[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	cr_assert(lt(long, bn_compare(&num1, &num2), 0));
}

Test(compare_arrays, test_2000000001_cmp_1000000002,
	 .description = "compare(2,000,000,001, 1,000,000,002) > 0",
	 .timeout = 2.0)
{
	u_int in1[] = {1, 2};
	u_int in2[] = {2, 1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	cr_assert(gt(long, bn_compare(&num1, &num2), 0));
}

Test(compare_arrays, test_1000000002_cmp_2000000001,
	 .description = "compare(1,000,000,002, 2,000,000,001) < 0",
	 .timeout = 2.0)
{
	u_int in1[] = {2, 1};
	u_int in2[] = {1, 2};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	cr_assert(lt(long, bn_compare(&num1, &num2), 0));
}

TestSuite(compare_long_arrays, .init = setup, .fini = teardown);

Test(compare_long_arrays, test_eqlong_cmp_eqlong,
	 .description = "compare(equal_long_array, equal_long_array) == 0",
	 .timeout = 2.0)
{
	u_int in1[] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};
	u_int in2[] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	cr_assert(zero(long, bn_compare(&num1, &num2)));
}

Test(compare_long_arrays, test_biglong_cmp_smalllong,
	 .description = "compare(big_long_array, small_long_array) > 0", .timeout = 2.0)
{
	u_int in1[] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};
	u_int in2[] = {9, 8, 7, 6, 5, 5, 4, 3, 2, 1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	cr_assert(gt(long, bn_compare(&num1, &num2), 0));
}

Test(compare_long_arrays, test_smalllong_cmp_biglong,
	 .description = "compare(small_long_array, big_long_array) < 0", .timeout = 2.0)
{
	u_int in1[] = {9, 8, 7, 6, 5, 5, 4, 3, 2, 1};
	u_int in2[] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	cr_assert(lt(long, bn_compare(&num1, &num2), 0));
}

TestSuite(compare_different_len_arrays, .init = setup, .fini = teardown);

Test(compare_different_len_arrays, test_long_cmp_short,
	 .description = "compare(long_array, short_array) > 0", .timeout = 2.0)
{
	u_int in1[] = {1, 2, 3, 4, 5, 5, 6, 9, 7, 8};
	u_int in2[] = {9, 8, 4, 3, 2, 9};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	cr_assert(gt(long, bn_compare(&num1, &num2), 0));
}

Test(compare_different_len_arrays, test_short_cmp_long,
	 .description = "compare(short_array, long_array) < 0", .timeout = 2.0)
{
	u_int in1[] = {9, 8, 4, 3, 2, 9};
	u_int in2[] = {1, 2, 3, 4, 5, 5, 6, 9, 7, 8};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	cr_assert(lt(long, bn_compare(&num1, &num2), 0));
}

TestSuite(negative_numbers, .init = setup, .fini = teardown);

Test(negative_numbers, test_minus1_cmp_minus1,
	 .description = "compare(-1, -1) == 0(equal)", .timeout = 2.0)
{
	u_int in1[] = {1};
	u_int in2[] = {1};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	num2 = (bignum){
		.len = sizeof(in2) / sizeof(*in2), .is_negative = true, .num = in2};

	cr_assert(zero(long, bn_compare(&num1, &num2)));
}

Test(negative_numbers, test_1_cmp_minus1,
	 .description = "compare(1, -1) > 0", .timeout = 2.0)
{
	u_int in1[] = {1};
	u_int in2[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2 = (bignum){
		.len = sizeof(in2) / sizeof(*in2), .is_negative = true, .num = in2};

	cr_assert(gt(long, bn_compare(&num1, &num2), 0));
}

Test(negative_numbers, test_minus1_cmp_1,
	 .description = "compare(-1, 1) < 0", .timeout = 2.0)
{
	u_int in1[] = {1};
	u_int in2[] = {1};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	cr_assert(lt(long, bn_compare(&num1, &num2), 0));
}
