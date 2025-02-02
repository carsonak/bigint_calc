#include "tests.h"

bigint num1 = {.len = 0, .is_negative = false, .num = NULL};
bigint num2 = {.len = 0, .is_negative = false, .num = NULL};

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

struct null_inputs
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(null_inputs)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(null_inputs) {}

TEST(null_inputs, test_NULL_cmp_NULL)
{
	EXPECT(bi_compare(NULL, NULL) == 0, "");
}

TEST(null_inputs, test_0_cmp_NULL)
{
	u_int in1[1];

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	EXPECT(bi_compare(&num1, NULL) == 0, "");
}

TEST(null_inputs, test_NULL_cmp_0)
{
	u_int in1[1];

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	EXPECT(bi_compare(NULL, &num1) == 0, "");
}

TEST(null_inputs, test_1_cmp_NULL)
{
	u_int in1[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	EXPECT(bi_compare(&num1, NULL) == 0, "");
}

TEST(null_inputs, test_NULL_cmp_1)
{
	u_int in1[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	EXPECT(bi_compare(NULL, &num1) == 0, "");
}

TEST(null_arrays, test_NULL_cmp_nullarray)
{
	EXPECT(bi_compare(NULL, &num2) == 0, "");
}

TEST(null_arrays, test_nullarray_cmp_NULL)
{
	EXPECT(bi_compare(&num1, NULL) == 0, "");
}

struct null_arrays
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(null_arrays)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(null_arrays) {}

TEST(null_arrays, test_nullarray_cmp_nullarray)
{
	EXPECT(bi_compare(&num1, &num2) == 0, "");
}

TEST(null_arrays, test_0_cmp_nullarray)
{
	u_int in1[1];

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	EXPECT(bi_compare(&num1, &num2) == 1, "");
}

TEST(null_arrays, test_nullarray_cmp_0)
{
	u_int in2[1];

	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;
	EXPECT(bi_compare(&num1, &num2), -1 ==, "");
}

TEST(null_arrays, test_1_cmp_nullarray)
{
	u_int in1[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	EXPECT(bi_compare(&num1, &num2) == 1, "");
}

TEST(null_arrays, test_nullarray_cmp_1)
{
	u_int in2[] = {1};

	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;
	EXPECT(bi_compare(&num1, &num2), -1 ==, "");
}

struct trailing_zeros
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(trailing_zeros)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(trailing_zeros) {}

TEST(trailing_zeros, test_long0_cmp_short0)
{
	/* clang-format off */
	u_int in1[] = {1, 8272, 3, 4, 12345, 5, 6, 9, 7, 8, 9284, 1098, 38476, 0, 0, 0, 0, 0, 0};
	u_int in2[] = {9, 8, 49247, 3, 2, 83749, 64154, 99990, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	/* clang-format on */

	num1.len = (sizeof(in1) / sizeof(*in1));
	num1.num = in1;
	num2.len = (sizeof(in2) / sizeof(*in2));
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) > 0, "");
}

TEST(trailing_zeros, test_long_cmp_short0)
{
	/* clang-format off */
	u_int in1[] = {1, 8272, 3, 4, 12345, 5, 6, 9, 7, 8, 9284, 1098, 38476};
	u_int in2[] = {9, 8, 49247, 3, 2, 83749, 64154, 9999, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	/* clang-format on */

	num1.len = (sizeof(in1) / sizeof(*in1));
	num1.num = in1;
	num2.len = (sizeof(in2) / sizeof(*in2));
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) > 0, "");
}

TEST(trailing_zeros, test_long0_cmp_short)
{
	/* clang-format off */
	u_int in1[] = {1, 8272, 3, 4, 12345, 5, 6, 9, 7, 8, 9284, 1098, 38476, 0, 0, 0, 0, 0, 0};
	u_int in2[] = {9, 8, 49247, 3, 2, 83749, 64154, 9999};
	/* clang-format on */

	num1.len = (sizeof(in1) / sizeof(*in1));
	num1.num = in1;
	num2.len = (sizeof(in2) / sizeof(*in2));
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) > 0, "");
}

struct compare_arrays
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(compare_arrays)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(compare_arrays) {}

TEST(compare_arrays, test_0_cmp_0)
{
	u_int in1[1];
	u_int in2[1];

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) == 0, "");
}

TEST(compare_arrays, test_1_cmp_1)
{
	u_int in1[] = {1};
	u_int in2[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) == 0, "");
}

TEST(compare_arrays, test_1_cmp_0)
{
	u_int in1[] = {1};
	u_int in2[1];

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) > 0, "");
}

TEST(compare_arrays, test_0_cmp_1)
{
	u_int in1[1];
	u_int in2[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) < 0, "");
}

TEST(compare_arrays, test_2000000001_cmp_1000000002)
{
	u_int in1[] = {1, 2};
	u_int in2[] = {2, 1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) > 0, "");
}

TEST(compare_arrays, test_1000000002_cmp_2000000001)
{
	u_int in1[] = {2, 1};
	u_int in2[] = {1, 2};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) < 0, "");
}

struct compare_long_arrays
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(compare_long_arrays)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(compare_long_arrays) {}

TEST(compare_long_arrays, test_eqlong_cmp_eqlong)
{
	u_int in1[] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};
	u_int in2[] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) == 0, "");
}

TEST(compare_long_arrays, test_biglong_cmp_smalllong)
{
	u_int in1[] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};
	u_int in2[] = {9, 8, 7, 6, 5, 5, 4, 3, 2, 1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) > 0, "");
}

TEST(compare_long_arrays, test_smalllong_cmp_biglong)
{
	u_int in1[] = {9, 8, 7, 6, 5, 5, 4, 3, 2, 1};
	u_int in2[] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) < 0, "");
}

struct compare_different_len_arrays
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(compare_different_len_arrays)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(compare_different_len_arrays) {}

TEST(compare_different_len_arrays, test_long_cmp_short)
{
	u_int in1[] = {1, 2, 3, 4, 5, 5, 6, 9, 7, 8};
	u_int in2[] = {9, 8, 4, 3, 2, 9};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) > 0, "");
}

TEST(compare_different_len_arrays, test_short_cmp_long)
{
	u_int in1[] = {9, 8, 4, 3, 2, 9};
	u_int in2[] = {1, 2, 3, 4, 5, 5, 6, 9, 7, 8};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) < 0, "");
}

struct negative_numbers
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(negative_numbers)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(negative_numbers) {}

TEST(negative_numbers, test_minus1_cmp_minus1)
{
	u_int in1[] = {1};
	u_int in2[] = {1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	num2 = (bigint){
		.len = sizeof(in2) / sizeof(*in2), .is_negative = true, .num = in2};

	EXPECT(bi_compare(&num1, &num2) == 0, "");
}

TEST(negative_numbers, test_1_cmp_minus1)
{
	u_int in1[] = {1};
	u_int in2[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	num2 = (bigint){
		.len = sizeof(in2) / sizeof(*in2), .is_negative = true, .num = in2};

	EXPECT(bi_compare(&num1, &num2) > 0, "");
}

TEST(negative_numbers, test_minus1_cmp_1)
{
	u_int in1[] = {1};
	u_int in2[] = {1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	num2.len = sizeof(in2) / sizeof(*in2);
	num2.num = in2;

	EXPECT(bi_compare(&num1, &num2) < 0, "");
}
