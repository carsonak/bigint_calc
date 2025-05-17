#include "tests.h"

struct invalid_inputs
{
	bigint num1, expected, *output;
};

TEST_F_SETUP(invalid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(invalid_inputs) { tau->output = bi_delete(tau->output); }

TEST(invalid_inputs, test_null_shift_r)
{
	CHECK_PTR_EQ(bi_shift_r(NULL, 1), NULL);
	CHECK_PTR_EQ(bi_shift_r(NULL, 0), NULL);
	CHECK_PTR_EQ(bi_shift_r(NULL, -1), NULL);
}

TEST_F(invalid_inputs, test_NaN_shift_r_neg1)
{
	CHECK_PTR_EQ(bi_shift_r(&(tau->num1), -1), NULL);
}

TEST_F(invalid_inputs, test_NaN_shift_r_0)
{
	tau->output = bi_shift_r(&(tau->num1), 0);
	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_NaN_shift_r_1)
{
	tau->output = bi_shift_r(&(tau->num1), 1);
	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_shift_r_neg1)
{
	u_int in1[1] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK_PTR_EQ(bi_shift_r(&(tau->num1), -1), NULL);
}

struct simple_shifts
{
	bigint num1, expected, *output;
};

TEST_F_SETUP(simple_shifts) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(simple_shifts) { tau->output = bi_delete(tau->output); }

TEST_F(simple_shifts, test_0_shift_r_0)
{
	u_int in1[1] = {0}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_r(&tau->num1, 0);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_3_shift_r_0)
{
	u_int in1[1] = {3}, out[1] = {3};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_r(&tau->num1, 0);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_0_shift_r_3)
{
	u_int in1[1] = {0}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_r(&tau->num1, 3);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_0_shift_r_maxi)
{
	u_int in1[1] = {0}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_r(&tau->num1, LENTYPE_MAX);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_0_shift_r_half_maxi)
{
	u_int in1[1] = {0}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_r(&tau->num1, LENTYPE_MAX / 2);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_3_shift_r_1)
{
	u_int in1[] = {3}, out[] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_r(&tau->num1, 1);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_3_shift_r_5)
{
	u_int in1[] = {3}, out[] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_r(&tau->num1, 5);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_3_shift_r_maxi)
{
	u_int in1[] = {3}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_r(&tau->num1, LENTYPE_MAX);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_3_shift_r_half_maxi)
{
	u_int in1[] = {3}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_r(&tau->num1, LENTYPE_MAX / 2);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_large_shift_r_1)
{
	u_int in1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	u_int out[] = {2, 3, 4, 5, 6, 7, 8, 9};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_r(&tau->num1, 1);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_large_shift_r_5)
{
	u_int in1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	u_int out[] = {6, 7, 8, 9};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_r(&tau->num1, 5);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_large_shift_r_maxi)
{
	u_int in1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_r(&tau->num1, LENTYPE_MAX);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_large_shift_r_half_maxi)
{
	u_int in1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_r(&tau->num1, LENTYPE_MAX / 2);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

/* ################################################################### */
/* ########################### bi_shift_l ############################ */
/* ################################################################### */

TEST(invalid_inputs, test_null_shift_l)
{
	CHECK_PTR_EQ(bi_shift_l(NULL, 1), NULL);
	CHECK_PTR_EQ(bi_shift_l(NULL, 0), NULL);
	CHECK_PTR_EQ(bi_shift_l(NULL, -1), NULL);
}

TEST_F(invalid_inputs, test_NaN_shift_l_neg1)
{
	CHECK_PTR_EQ(bi_shift_l(&(tau->num1), -1), NULL);
}

TEST_F(invalid_inputs, test_NaN_shift_l_0)
{
	tau->output = bi_shift_l(&(tau->num1), 0);
	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_NaN_shift_l_1)
{
	tau->output = bi_shift_l(&(tau->num1), 1);
	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_shift_l_neg1)
{
	u_int in1[1] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK_PTR_EQ(bi_shift_l(&(tau->num1), -1), NULL);
}

/* simple_shifts */

TEST_F(simple_shifts, test_0_shift_l_0)
{
	u_int in1[1] = {0}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_l(&tau->num1, 0);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_3_shift_l_0)
{
	u_int in1[1] = {3}, out[1] = {3};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_l(&tau->num1, 0);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_0_shift_l_3)
{
	u_int in1[1] = {0}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_l(&tau->num1, 3);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_0_shift_l_maxi)
{
	u_int in1[1] = {0}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_l(&tau->num1, LENTYPE_MAX);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_0_shift_l_half_maxi)
{
	u_int in1[1] = {0}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_l(&tau->num1, LENTYPE_MAX / 2);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_3_shift_l_1)
{
	u_int in1[] = {3}, out[] = {0, 3};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_l(&tau->num1, 1);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_3_shift_l_5)
{
	u_int in1[] = {3}, out[] = {0, 0, 0, 0, 0, 3};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_l(&tau->num1, 5);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_3_shift_l_maxi)
{
	u_int in1[] = {3};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK_PTR_EQ(bi_shift_l(&tau->num1, LENTYPE_MAX), NULL);
}

TEST_F(simple_shifts, test_3_shift_l_half_maxi)
{
	u_int in1[] = {3};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK_PTR_EQ(bi_shift_l(&tau->num1, LENTYPE_MAX / 2), NULL);
}

TEST_F(simple_shifts, test_large_shift_l_1)
{
	u_int in1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	u_int out[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_l(&tau->num1, 1);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_large_shift_l_5)
{
	u_int in1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	u_int out[] = {0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_shift_l(&tau->num1, 5);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(simple_shifts, test_large_shift_l_maxi)
{
	u_int in1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK_PTR_EQ(bi_shift_l(&tau->num1, LENTYPE_MAX), NULL);
}

TEST_F(simple_shifts, test_large_shift_l_half_maxi)
{
	u_int in1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	CHECK_PTR_EQ(bi_shift_l(&tau->num1, LENTYPE_MAX / 2), NULL);
}
