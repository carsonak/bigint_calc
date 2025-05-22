#include "tests.h"

TEST(bigint_free, null_pointer) { CHECK_PTR_EQ(_bi_free(NULL), NULL); }

#define TESTS_BIGINT_LENGTH (len_type)10

struct bigint_alloc
{
	bigint *output;
};

TEST_F_SETUP(bigint_alloc) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(bigint_alloc) { tau->output = _bi_free(tau->output); }

TEST_F(bigint_alloc, alloc_zero_len_bigint)
{
	tau->output = _bi_alloc(0);

	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK(tau->output->len == 0);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(bigint_alloc, alloc_a_sized_bigint)
{
	tau->output = _bi_alloc(TESTS_BIGINT_LENGTH);

	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK(tau->output->len == TESTS_BIGINT_LENGTH);
	CHECK_PTR_NE(tau->output->num, NULL);
	/* memory checkers should not complain. */
	tau->output->num[TESTS_BIGINT_LENGTH - 1] = DUMMY_VALUE;
}

/* ################################################################### */
/* ######################### _bi_resize ############################## */
/* ################################################################### */

struct bigint_resize
{
	bigint *output;
};

TEST_F_SETUP(bigint_resize) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(bigint_resize) { tau->output = _bi_free(tau->output); }

TEST_F(bigint_resize, resize_NULL_to_zero_len_bigint)
{
	tau->output = _bi_resize(NULL, 0);

	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK(tau->output->len == 0);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(bigint_resize, resize_NULL_to_sized_bigint)
{
	tau->output = _bi_resize(NULL, TESTS_BIGINT_LENGTH);

	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK(tau->output->len == TESTS_BIGINT_LENGTH);
	CHECK_PTR_NE(tau->output->num, NULL);
	/* memory checkers should not complain. */
	tau->output->num[TESTS_BIGINT_LENGTH - 1] = DUMMY_VALUE;
}

TEST_F(bigint_resize, resize_zero_len_to_zero_len_bigint)
{
	tau->output = _bi_alloc(0);
	bigint *const tmp = _bi_resize(tau->output, 0);

	REQUIRE_PTR_NE(tmp, NULL);
	tau->output = tmp;
	CHECK(tau->output->len == 0);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(bigint_resize, resize_zero_len_to_sized_bigint)
{
	tau->output = _bi_alloc(0);
	bigint *const tmp = _bi_resize(tau->output, TESTS_BIGINT_LENGTH);

	REQUIRE_PTR_NE(tmp, NULL);
	tau->output = tmp;
	CHECK(tau->output->len == TESTS_BIGINT_LENGTH);
	CHECK_PTR_NE(tau->output->num, NULL);
	/* memory checkers should not complain. */
	tau->output->num[TESTS_BIGINT_LENGTH - 1] = DUMMY_VALUE;
}

struct bigint_resize_sized
{
	bigint *output;
};

TEST_F_SETUP(bigint_resize_sized) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(bigint_resize_sized) { tau->output = _bi_free(tau->output); }

TEST_F(bigint_resize_sized, resize_to_zero_len_bigint)
{
	tau->output = _bi_alloc(TESTS_BIGINT_LENGTH);

	REQUIRE_PTR_NE(tau->output, NULL);
	tau->output->num[0] = DUMMY_VALUE;
	tau->output->num[TESTS_BIGINT_LENGTH - 1] = DUMMY_VALUE;

	tau->output = _bi_resize(tau->output, 0);

	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK(tau->output->len == 0);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(bigint_resize_sized, resize_to_smaller_len_bigint)
{
	tau->output = _bi_alloc(TESTS_BIGINT_LENGTH);

	REQUIRE_PTR_NE(tau->output, NULL);
	tau->output->num[0] = DUMMY_VALUE;
	tau->output->num[TESTS_BIGINT_LENGTH - 1] = DUMMY_VALUE;

	const len_type new_len = TESTS_BIGINT_LENGTH - 6;
	tau->output = _bi_resize(tau->output, new_len);

	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK(tau->output->len == new_len);
	CHECK_PTR_NE(tau->output->num, NULL);
	CHECK(
		tau->output->num[0] == DUMMY_VALUE, "data should have been copied over"
	);
	/* memory checkers should not complain. */
	tau->output->num[new_len - 1] = DUMMY_VALUE;
}

TEST_F(bigint_resize_sized, resize_to_bigger_len_bigint)
{
	tau->output = _bi_alloc(TESTS_BIGINT_LENGTH);

	REQUIRE_PTR_NE(tau->output, NULL);
	tau->output->num[0] = DUMMY_VALUE;
	tau->output->num[TESTS_BIGINT_LENGTH - 1] = DUMMY_VALUE;

	const len_type new_len = TESTS_BIGINT_LENGTH + 3;
	tau->output = _bi_resize(tau->output, new_len);

	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK(tau->output->len == new_len);
	CHECK_PTR_NE(tau->output->num, NULL);
	CHECK(
		tau->output->num[0] == DUMMY_VALUE, "data should have been copied over"
	);
	CHECK(
		tau->output->num[TESTS_BIGINT_LENGTH - 1] == DUMMY_VALUE,
		"data should have been copied over"
	);
	/* memory checkers should not complain. */
	tau->output->num[new_len - 1] = DUMMY_VALUE;
}

TEST_F(bigint_resize_sized, resize_to_even_bigger_len_bigint)
{
	tau->output = _bi_alloc(TESTS_BIGINT_LENGTH);

	REQUIRE_PTR_NE(tau->output, NULL);
	tau->output->num[0] = DUMMY_VALUE;
	tau->output->num[TESTS_BIGINT_LENGTH - 1] = DUMMY_VALUE;

	const len_type new_len = TESTS_BIGINT_LENGTH * 10;
	tau->output = _bi_resize(tau->output, new_len);

	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK(tau->output->len == new_len);
	CHECK_PTR_NE(tau->output->num, NULL);
	CHECK(
		tau->output->num[0] == DUMMY_VALUE, "data should have been copied over"
	);
	CHECK(
		tau->output->num[TESTS_BIGINT_LENGTH - 1] == DUMMY_VALUE,
		"data should have been copied over"
	);
	/* memory checkers should not complain. */
	tau->output->num[new_len - 1] = DUMMY_VALUE;
}

#undef TESTS_BIGINT_LENGTH

/* ################################################################### */
/* ########################### _bi_trim ############################## */
/* ################################################################### */

TEST(trim_invalid_inputs, test_NULL)
{
	_bi_trim(NULL); /* should handle NULL safely */
}

struct trim_null_array
{
	bigint num1, expected, *output;
};

TEST_F_SETUP(trim_null_array) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(trim_null_array) { tau->output = bi_delete(tau->output); }

TEST_F(trim_null_array, test_NaN)
{
	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == 0);
	CHECK(tau->num1.is_negative == false);
	CHECK_PTR_EQ(tau->num1.num, NULL);
}

TEST_F(trim_null_array, test_NaN_neg)
{
	tau->num1.is_negative = true;

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == 0);
	CHECK(tau->num1.is_negative == false);
	CHECK_PTR_EQ(tau->num1.num, NULL);
}

TEST_F(trim_null_array, test_NaN_len5)
{
	tau->num1.len = 5;

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == 0);
	CHECK(tau->num1.is_negative == false);
	CHECK_PTR_EQ(tau->num1.num, NULL);
}

TEST_F(trim_null_array, test_NaN_len5_neg)
{
	tau->num1.len = 5;
	tau->num1.is_negative = true;

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == 0);
	CHECK(tau->num1.is_negative == false);
	CHECK_PTR_EQ(tau->num1.num, NULL);
}

struct trim_trailing_zeros
{
	bigint num1, expected, *output;
};

TEST_F_SETUP(trim_trailing_zeros) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(trim_trailing_zeros) { tau->output = bi_delete(tau->output); }

TEST_F(trim_trailing_zeros, test_one_zero)
{
	u_int in1[] = {0};
	u_int out[] = {0};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == tau->expected.len);
	CHECK(tau->num1.is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1.num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(trim_trailing_zeros, test_several_zeros)
{
	u_int in1[] = {0, 0, 0, 0};
	u_int out[] = {0};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == tau->expected.len);
	CHECK(tau->num1.is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1.num, tau->expected.num,
		sizeof(*tau->expected.num) * tau->expected.len
	);
}

TEST_F(trim_trailing_zeros, test_0001)
{
	u_int in1[] = {1, 0, 0, 0};
	u_int out[] = {1};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == tau->expected.len);
	CHECK(tau->num1.is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1.num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(trim_trailing_zeros, test_0100)
{
	u_int in1[] = {0, 0, 1, 0};
	u_int out[] = {0, 0, 1};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == tau->expected.len);
	CHECK(tau->num1.is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1.num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

struct trim_trailing_zeros_negative
{
	bigint num1, expected, *output;
};

TEST_F_SETUP(trim_trailing_zeros_negative) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(trim_trailing_zeros_negative)
{
	tau->output = bi_delete(tau->output);
}

TEST_F(trim_trailing_zeros_negative, test_one_zero)
{
	u_int in1[] = {0};
	u_int out[] = {0};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == tau->expected.len);
	CHECK(tau->num1.is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1.num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(trim_trailing_zeros_negative, test_several_zeros)
{
	u_int in1[] = {0, 0, 0, 0};
	u_int out[] = {0};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == tau->expected.len);
	CHECK(tau->num1.is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1.num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(trim_trailing_zeros_negative, test_0001)
{
	u_int in1[] = {1, 0, 0, 0};
	u_int out[] = {1};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == tau->expected.len);
	CHECK(tau->num1.is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1.num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(trim_trailing_zeros_negative, test_0100)
{
	u_int in1[] = {0, 0, 1, 0};
	u_int out[] = {0, 0, 1};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == tau->expected.len);
	CHECK(tau->num1.is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1.num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

struct trim_normal_input
{
	bigint num1, expected, *output;
};

TEST_F_SETUP(trim_normal_input) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(trim_normal_input) { tau->output = bi_delete(tau->output); }

TEST_F(trim_normal_input, test_1)
{
	u_int in1[] = {1};
	u_int out[] = {1};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == tau->expected.len);
	CHECK(tau->num1.is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1.num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(trim_normal_input, test_neg1)
{
	u_int in1[] = {1};
	u_int out[] = {1};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == tau->expected.len);
	CHECK(tau->num1.is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1.num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(trim_normal_input, test_100)
{
	u_int in1[] = {0, 0, 1};
	u_int out[] = {0, 0, 1};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == tau->expected.len);
	CHECK(tau->num1.is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1.num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(trim_normal_input, test_neg100)
{
	u_int in1[] = {0, 0, 1};
	u_int out[] = {0, 0, 1};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	_bi_trim(&(tau->num1));

	CHECK(tau->num1.len == tau->expected.len);
	CHECK(tau->num1.is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1.num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

/* ################################################################### */
/* ######################## _bi_move ################################# */
/* ################################################################### */

/* ################################################################### */
/* ######################## _bi_dup ################################## */
/* ################################################################### */
