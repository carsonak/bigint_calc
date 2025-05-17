#include "tests.h"

struct invalid_inputs
{
	bigint num1, num2;
};

TEST_F_SETUP(invalid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(invalid_inputs) { (void)tau; }

TEST(invalid_inputs, test_NULL_cmp_NULL)
{
	CHECK(bi_compare(NULL, NULL) == 0);
}

TEST_F(invalid_inputs, test_0_cmp_NULL)
{
	u_int in1[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare(&(tau->num1), NULL) == 0);
}

TEST_F(invalid_inputs, test_NULL_cmp_0)
{
	u_int in1[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare(NULL, &(tau->num1)) == 0);
}

TEST_F(invalid_inputs, test_1_cmp_NULL)
{
	u_int in1[] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare(&(tau->num1), NULL) == 0);
}

TEST_F(invalid_inputs, test_NULL_cmp_1)
{
	u_int in1[] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare(NULL, &(tau->num1)) == 0);
}

TEST_F(invalid_inputs, test_NULL_cmp_NaN)
{
	CHECK(bi_compare(NULL, &(tau->num2)) == 0);
}

TEST_F(invalid_inputs, test_NaN_cmp_NULL)
{
	CHECK(bi_compare(&(tau->num1), NULL) == 0);
}

struct null_arrays
{
	bigint num1, num2;
};

TEST_F_SETUP(null_arrays) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(null_arrays) { (void)tau; }

TEST_F(null_arrays, test_NaN_cmp_NaN)
{
	CHECK(bi_compare(&(tau->num1), &(tau->num2)) == 0);
}

TEST_F(null_arrays, test_0_cmp_NaN)
{
	u_int in1[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare(&(tau->num1), &(tau->num2)) == 0);
}

TEST_F(null_arrays, test_NaN_cmp_0)
{
	u_int in2[1] = {0};

	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	CHECK(bi_compare(&(tau->num1), &(tau->num2)) == 0);
}

TEST_F(null_arrays, test_1_cmp_NaN)
{
	u_int in1[] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare(&(tau->num1), &(tau->num2)) == 0);
}

TEST_F(null_arrays, test_NaN_cmp_1)
{
	u_int in2[] = {1};

	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	CHECK(bi_compare(&(tau->num1), &(tau->num2)) == 0);
}

struct trailing_zeros
{
	bigint num1, num2;
};

TEST_F_SETUP(trailing_zeros) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(trailing_zeros) { (void)tau; }

TEST_F(trailing_zeros, test_long0_cmp_short0)
{
	/* clang-format off */
	u_int in1[] = {1, 8272, 3, 4, 12345, 5, 6, 9, 7, 8, 9284, 1098, 38476, 0, 0, 0, 0, 0, 0};
	u_int in2[] = {9, 8, 49247, 3, 2, 83749, 64154, 99990, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	/* clang-format on */

	tau->num1.len = (sizeof(in1) / sizeof(*in1));
	tau->num1.num = in1;
	tau->num2.len = (sizeof(in2) / sizeof(*in2));
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) > 0);
}

TEST_F(trailing_zeros, test_long_cmp_short0)
{
	/* clang-format off */
	u_int in1[] = {1, 8272, 3, 4, 12345, 5, 6, 9, 7, 8, 9284, 1098, 38476};
	u_int in2[] = {9, 8, 49247, 3, 2, 83749, 64154, 9999, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	/* clang-format on */

	tau->num1.len = (sizeof(in1) / sizeof(*in1));
	tau->num1.num = in1;
	tau->num2.len = (sizeof(in2) / sizeof(*in2));
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) > 0);
}

TEST_F(trailing_zeros, test_long0_cmp_short)
{
	/* clang-format off */
	u_int in1[] = {1, 8272, 3, 4, 12345, 5, 6, 9, 7, 8, 9284, 1098, 38476, 0, 0, 0, 0, 0, 0};
	u_int in2[] = {9, 8, 49247, 3, 2, 83749, 64154, 9999};
	/* clang-format on */

	tau->num1.len = (sizeof(in1) / sizeof(*in1));
	tau->num1.num = in1;
	tau->num2.len = (sizeof(in2) / sizeof(*in2));
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) > 0);
}

struct compare_arrays
{
	bigint num1, num2;
};

TEST_F_SETUP(compare_arrays) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(compare_arrays) { (void)tau; }

TEST_F(compare_arrays, test_0_cmp_0)
{
	u_int in1[1];
	u_int in2[1];

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) == 0);
}

TEST_F(compare_arrays, test_1_cmp_1)
{
	u_int in1[] = {1};
	u_int in2[] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) == 0);
}

TEST_F(compare_arrays, test_1_cmp_0)
{
	u_int in1[] = {1};
	u_int in2[1];

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) > 0);
}

TEST_F(compare_arrays, test_0_cmp_1)
{
	u_int in1[1];
	u_int in2[] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) < 0);
}

TEST_F(compare_arrays, test_2000000001_cmp_1000000002)
{
	u_int in1[] = {1, 2};
	u_int in2[] = {2, 1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) > 0);
}

TEST_F(compare_arrays, test_1000000002_cmp_2000000001)
{
	u_int in1[] = {2, 1};
	u_int in2[] = {1, 2};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) < 0);
}

struct compare_long_arrays
{
	bigint num1, num2;
};

TEST_F_SETUP(compare_long_arrays) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(compare_long_arrays) { (void)tau; }

TEST_F(compare_long_arrays, test_eqlong_cmp_eqlong)
{
	u_int in1[] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};
	u_int in2[] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) == 0);
}

TEST_F(compare_long_arrays, test_biglong_cmp_smalllong)
{
	u_int in1[] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};
	u_int in2[] = {9, 8, 7, 6, 5, 5, 4, 3, 2, 1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) > 0);
}

TEST_F(compare_long_arrays, test_smalllong_cmp_biglong)
{
	u_int in1[] = {9, 8, 7, 6, 5, 5, 4, 3, 2, 1};
	u_int in2[] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) < 0);
}

struct compare_different_len_arrays
{
	bigint num1, num2;
};

TEST_F_SETUP(compare_different_len_arrays) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(compare_different_len_arrays) { (void)tau; }

TEST_F(compare_different_len_arrays, test_long_cmp_short)
{
	u_int in1[] = {1, 2, 3, 4, 5, 5, 6, 9, 7, 8};
	u_int in2[] = {9, 8, 4, 3, 2, 9};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) > 0);
}

TEST_F(compare_different_len_arrays, test_short_cmp_long)
{
	u_int in1[] = {9, 8, 4, 3, 2, 9};
	u_int in2[] = {1, 2, 3, 4, 5, 5, 6, 9, 7, 8};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) < 0);
}

struct negative_numbers
{
	bigint num1, num2;
};

TEST_F_SETUP(negative_numbers) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(negative_numbers) { (void)tau; }

TEST_F(negative_numbers, test_neg1_cmp_neg1)
{
	u_int in1[] = {1};
	u_int in2[] = {1};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};
	tau->num2 = (bigint){.len = sizeof(in2) / sizeof(*in2),
						 .is_negative = true,
						 .num = in2};

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) == 0);
}

TEST_F(negative_numbers, test_1_cmp_neg1)
{
	u_int in1[] = {1};
	u_int in2[] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2 = (bigint){.len = sizeof(in2) / sizeof(*in2),
						 .is_negative = true,
						 .num = in2};

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) > 0);
}

TEST_F(negative_numbers, test_neg1_cmp_1)
{
	u_int in1[] = {1};
	u_int in2[] = {1};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK(bi_compare(&(tau->num1), &(tau->num2)) < 0);
}

/* ################################################################### */
/* ####################### bi_compare_int ############################ */
/* ################################################################### */

TEST(invalid_inputs, test_NULL_cmp_0i) { CHECK(bi_compare_int(NULL, 0) == 0); }

TEST(invalid_inputs, test_NULL_cmp_1i) { CHECK(bi_compare_int(NULL, 1) == 0); }

TEST(invalid_inputs, test_NULL_cmp_neg1i)
{
	CHECK(bi_compare_int(NULL, -1) == 0);
}

TEST_F(null_arrays, test_NaN_cmp_0i)
{
	CHECK(bi_compare_int(&(tau->num1), 0) == 0);
}

TEST_F(null_arrays, test_NaN_cmp_1i)
{
	CHECK(bi_compare_int(&(tau->num1), 1) == 0);
}

TEST_F(null_arrays, test_NaN_cmp_neg1i)
{
	CHECK(bi_compare_int(&(tau->num1), -1) == 0);
}

struct simple_compares
{
	bigint num1;
};

TEST_F_SETUP(simple_compares) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(simple_compares) { (void)tau; }

TEST_F(simple_compares, test_0_cmp_0i)
{
	u_int in1[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), 0) == 0);
}

TEST_F(simple_compares, test_1_cmp_1i)
{
	u_int in1[1] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), 1) == 0);
}

TEST_F(simple_compares, test_neg1_cmp_neg1i)
{
	u_int in1[1] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.is_negative = true;
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), -1) == 0);
}

TEST_F(simple_compares, test_0_cmp_1i)
{
	u_int in1[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), 1) < 0);
}

TEST_F(simple_compares, test_1_cmp_0i)
{
	u_int in1[1] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), 0) > 0);
}

TEST_F(simple_compares, test_0_cmp_neg1i)
{
	u_int in1[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), -1) > 0);
}

TEST_F(simple_compares, test_neg1_cmp_0i)
{
	u_int in1[1] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.is_negative = true;
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), 0) < 0);
}

TEST_F(simple_compares, test_44_cmp_intmaxmaxi)
{
	u_int in1[1] = {44};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), INTMAX_MAX) < 0);
}

TEST_F(simple_compares, test_44_cmp_intmaxmini)
{
	u_int in1[1] = {44};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), INTMAX_MIN) > 0);
}

TEST_F(simple_compares, test_neg44_cmp_intmaxmaxi)
{
	u_int in1[1] = {44};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.is_negative = true;
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), INTMAX_MAX) < 0);
}

TEST_F(simple_compares, test_neg44_cmp_intmaxmini)
{
	u_int in1[1] = {44};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.is_negative = true;
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), INTMAX_MIN) > 0);
}

TEST_F(simple_compares, test_longnum_cmp_intmaxmaxi)
{
	u_int in1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), INTMAX_MAX) > 0);
}

TEST_F(simple_compares, test_longnum_cmp_intmaxmini)
{
	u_int in1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), INTMAX_MIN) > 0);
}

TEST_F(simple_compares, test_neglongnum_cmp_intmaxmaxi)
{
	u_int in1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.is_negative = true;
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), INTMAX_MAX) < 0);
}

TEST_F(simple_compares, test_neglongnum_cmp_intmaxmini)
{
	u_int in1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.is_negative = true;
	tau->num1.num = in1;
	CHECK(bi_compare_int(&(tau->num1), INTMAX_MIN) < 0);
}
