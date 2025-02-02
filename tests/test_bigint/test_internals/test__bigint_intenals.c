#include "tests.h"

#define TESTS_BIGINT_LENGTH (size_t)10

struct bigint_alloc
{
	static bigint *n;
};

TEST_F_SETUP(bigint_alloc) { tau->n = NULL; }

TEST_F_TEARDOWN(bigint_alloc) { tau->n = bi_delete(tau->n); }

TEST_F(bigint_alloc, alloc_zero_len_bigint)
{
	tau->n = _bi_alloc(0);

	REQUIRE(tau->n, "alloc should return non-null");
	CHECK(tau->n->len == 0, "length should be %zu", 0);
	CHECK(tau->n->num == NULL, "array of digits should be NULL");
}

TEST_F(bigint_alloc, alloc_a_sized_bigint)
{
	tau->n = _bi_alloc(TESTS_BIGINT_LENGTH);

	REQUIRE(tau->n, "alloc should return non-null");
	CHECK(
		tau->n->len == TESTS_BIGINT_LENGTH, "length should be %zu",
		TESTS_BIGINT_LENGTH);
	CHECK(tau->n->num, "array of digits should be non-null");
	tau->n->num[TESTS_BIGINT_LENGTH - 1] =
		DUMMY_VALUE;  // memory checkers should not complain.
}

/*###################################################################*/
/*###################################################################*/

struct bigint_resize
{
	static bigint *n;
};

TEST_F_SETUP(bigint_resize)
{
	tau->n = _bi_alloc(TESTS_BIGINT_LENGTH);
	REQUIRE(tau->n, "Failed to create bigint");

	tau->n->num[TESTS_BIGINT_LENGTH - 1] = DUMMY_VALUE;
	tau->n->num[0] = DUMMY_VALUE;
}

TEST_F_TEARDOWN(bigint_resize) { tau->n = bi_delete(tau->n); }

TEST(bigint_resize, resize_NULL_to_zero_len_bigint)
{
	bigint *const n = _bi_resize(NULL, 0);

	REQUIRE(n, "resize should return non-null");
	CHECK(n->len == 0, "length should be %zu", 0);
	CHECK(n->num == NULL, "array of digits should be NULL");

	bi_delete(n);
}

TEST(bigint_resize, resize_NULL_to_sized_bigint)
{
	bigint *const n = _bi_resize(NULL, TESTS_BIGINT_LENGTH);

	REQUIRE(n, "resize should return non-null");
	CHECK(
		n->len == TESTS_BIGINT_LENGTH, "length should be %zu",
		TESTS_BIGINT_LENGTH);
	CHECK(n->num, "array of digits should be non-null");
	n->num[TESTS_BIGINT_LENGTH - 1] =
		DUMMY_VALUE;  // memory checkers should not complain.

	bi_delete(n);
}

TEST(bigint_resize, resize_zero_len_to_zero_len_bigint)
{
	bigint *n = _bi_alloc(0);
	bigint *const tmp = _bi_resize(n, 0);

	REQUIRE(tmp, "resize should return non-null");
	n = tmp;
	CHECK(n->len == 0, "length should be %zu", 0);
	CHECK(n->num == NULL, "array of digits should be NULL");

	bi_delete(n);
}

TEST(bigint_resize, resize_zero_len_to_sized_bigint)
{
	bigint *n = _bi_alloc(0);
	bigint *const tmp = _bi_resize(n, TESTS_BIGINT_LENGTH);

	REQUIRE(tmp, "resize should return non-null");
	n = tmp;
	CHECK(
		n->len == TESTS_BIGINT_LENGTH, "length should be %zu",
		TESTS_BIGINT_LENGTH);
	CHECK(n->num, "array of digits should be non-null");
	n->num[TESTS_BIGINT_LENGTH - 1] =
		DUMMY_VALUE;  // memory checkers should not complain.

	bi_delete(n);
}

TEST_F(bigint_resize, resize_to_zero_len_bigint)
{
	bigint *const tmp = _bi_resize(tau->n, 0);

	REQUIRE(tmp, "resize should return non-null");
	tau->n = tmp;
	CHECK(tau->n->len == 0, "length should be %zu", 0);
	CHECK(tau->n->num == NULL, "array of digits should be NULL");
}

TEST_F(bigint_resize, resize_to_smaller_len_bigint)
{
	const size_t new_len = TESTS_BIGINT_LENGTH - 3;
	bigint *const tmp = _bi_resize(tau->n, new_len);

	REQUIRE(tmp, "resize should return non-null");
	tau->n = tmp;
	CHECK(tau->n->len == new_len, "length should be %zu", new_len);
	CHECK(tau->n->num, "array of digits should be non-null");
	CHECK(tau->n->num[0] == DUMMY_VALUE, "data should have been copied over");
	tau->n->num[new_len - 1] =
		DUMMY_VALUE;  // memory checkers should not complain.
}

TEST_F(bigint_resize, resize_to_bigger_len_bigint)
{
	const size_t new_len = TESTS_BIGINT_LENGTH + 3;
	bigint *const tmp = _bi_resize(tau->n, new_len);

	REQUIRE(tmp, "resize should return non-null");
	tau->n = tmp;
	CHECK(tau->n->len == new_len, "length should be %zu", new_len);
	CHECK(tau->n->num, "array of digits should be non-null");
	CHECK(tau->n->num[0] == DUMMY_VALUE, "data should have been copied over");
	CHECK(
		tau->n->num[TESTS_BIGINT_LENGTH - 1] == DUMMY_VALUE,
		"data should have been copied over");
	tau->n->num[new_len - 1] =
		DUMMY_VALUE;  // memory checkers should not complain.
}

#undef BIGINT_LENGTH_TESTS

/*###################################################################*/
/*###################################################################*/

TEST(null_inputs, test_NULL) { bi_trim(NULL); }

struct null_array
{
	bigint num1, expected;
};

TEST_F_SETUP(null_array)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}

TEST_F_TEARDOWN(null_array) {}

TEST_F(null_array, test_nullarray)
{
	bi_trim(tau->num1);

	CHECK(tau->num1.len == tau->expected.len, "length should remain 0");
	CHECK(
		tau->num1.is_negative == tau->expected.is_negative,
		"negative flag should remain false");
	CHECK(tau->num1.num == NULL, "int array should remain NULL");
}

TEST_F(null_array, test_nullarray_neg)
{
	tau->num1.is_negative = true;

	bi_trim(tau->num1);

	CHECK(tau->num1.len == tau->expected.len, "length should remain 0");
	CHECK(
		tau->num1.is_negative == tau->expected.is_negative,
		"negative flag should be false");
	CHECK(tau->num1.num == NULL, "int array should remain NULL");
}

TEST_F(null_array, test_nullarray_len5)
{
	tau->num1.len = 5;

	bi_trim(tau->num1);

	CHECK(tau->num1.len == tau->expected.len, "length should be 0");
	CHECK(
		tau->num1.is_negative == tau->expected.is_negative,
		"negative flag should remain false");
	CHECK(tau->num1.num == NULL, "int array should remain NULL");
}

TEST_F(null_array, test_nullarray_len5_neg)
{
	tau->num1.len = 5;
	tau->num1.is_negative = true;

	bi_trim(tau->num1);

	CHECK(tau->num1.len == tau->expected.len, "length should be 0");
	CHECK(
		tau->num1.is_negative == tau->expected.is_negative,
		"negative flag should be false");
	CHECK(tau->num1.num == NULL, "int array should remain NULL");
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

TEST(trailing_zeros, test_one_zero)
{
	u_int in1[1];
	u_int out[] = {0};

	tau->num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(tau->num1);

	CHECK(
		tau->num1.len == tau->expected.len, "length should be %zu",
		tau->expected.len);
	CHECK(
		tau->num1.is_negative == tau->expected.is_negative,
		"negative flag should be %u", tau->expected.is_negative);
	CHECK_BUF_EQ(tau->num1.num, tau->expected.num, tau->expected.len, "");
}

TEST(trailing_zeros, test_several_zeros)
{
	u_int in1[] = {0, 0, 0, 0};
	u_int out[] = {0};

	tau->num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(tau->num1);

	CHECK(
		tau->num1.len == tau->expected.len, "length should be %zu",
		tau->expected.len);
	CHECK(
		tau->num1.is_negative == tau->expected.is_negative,
		"negative flag should be %u", tau->expected.is_negative);
	CHECK_BUF_EQ(tau->num1.num, tau->expected.num, tau->expected.len, "");
}

TEST(trailing_zeros, test_0001)
{
	u_int in1[] = {1, 0, 0, 0};
	u_int out[] = {1};

	tau->num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(tau->num1);

	CHECK(
		tau->num1.len == tau->expected.len, "length should be %zu",
		tau->expected.len);
	CHECK(
		tau->num1.is_negative == tau->expected.is_negative,
		"negative flag should be %u", tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1.num, tau->expected.num,
		tau->expected.len * sizeof(*tau->expected.num), "");
}

TEST(trailing_zeros, test_0100)
{
	u_int in1[] = {0, 0, 1, 0};
	u_int out[] = {0, 0, 1};

	tau->num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};
	tau->expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(tau->num1);

	CHECK(
		tau->num1.len == tau->expected.len, "length should be %zu",
		tau->expected.len);
	CHECK(
		tau->num1.is_negative == tau->expected.is_negative,
		"negative flag should be %u", tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1.num, tau->expected.num,
		tau->expected.len * sizeof(*tau->expected.num), "");
}

struct trailing_zeros_negative
{
	bigint num1;
	bigint expected;
};

TEST_F_SETUP(trailing_zeros_negative)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}

TEST_F_TEARDOWN(trailing_zeros_negative) {}

TEST(trailing_zeros_negative, test_one_zero)
{
	u_int in1[1];
	u_int out[] = {0};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(tau->num1);

	CHECK(
		tau->num1->len == tau->expected.len, "length should be %zu",
		tau->expected.len);
	CHECK(
		tau->num1->is_negative == tau->expected.is_negative,
		"negative flag should be %u", tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1->num, tau->expected.num,
		tau->expected.len * sizeof(*tau->expected.num), "");
}

TEST(trailing_zeros_negative, test_several_zeros)
{
	u_int in1[] = {0, 0, 0, 0};
	u_int out[] = {0};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(tau->num1);

	CHECK(
		tau->num1->len == tau->expected.len, "length should be %zu",
		tau->expected.len);
	CHECK(
		tau->num1->is_negative == tau->expected.is_negative,
		"negative flag should be %u", tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1->num, tau->expected.num,
		tau->expected.len * sizeof(*tau->expected.num), "");
}

TEST(trailing_zeros_negative, test_0001)
{
	u_int in1[] = {1, 0, 0, 0};
	u_int out[] = {1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	bi_trim(tau->num1);

	CHECK(
		tau->num1->len == tau->expected.len, "length should be %zu",
		tau->expected.len);
	CHECK(
		tau->num1->is_negative == tau->expected.is_negative,
		"negative flag should be %u", tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1->num, tau->expected.num,
		tau->expected.len * sizeof(*tau->expected.num), "");
}

TEST(trailing_zeros_negative, test_0100)
{
	u_int in1[] = {0, 0, 1, 0};
	u_int out[] = {0, 0, 1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	bi_trim(tau->num1);

	CHECK(
		tau->num1->len == tau->expected.len, "length should be %zu",
		tau->expected.len);
	CHECK(
		tau->num1->is_negative == tau->expected.is_negative,
		"negative flag should be %u", tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1->num, tau->expected.num,
		tau->expected.len * sizeof(*tau->expected.num), "");
}

struct normal_input
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(normal_input)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(normal_input) {}

TEST(normal_input, test_1)
{
	u_int in1[] = {1};
	u_int out[] = {1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(tau->num1);

	CHECK(
		tau->num1->len == tau->expected.len, "length should be %zu",
		tau->expected.len);
	CHECK(
		tau->num1->is_negative == tau->expected.is_negative,
		"negative flag should be %u", tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1->num, tau->expected.num,
		tau->expected.len * sizeof(*tau->expected.num), "");
}

TEST(normal_input, test_neg1)
{
	u_int in1[] = {1};
	u_int out[] = {1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	bi_trim(tau->num1);

	CHECK(
		tau->num1->len == tau->expected.len, "length should be %zu",
		tau->expected.len);
	CHECK(
		tau->num1->is_negative == tau->expected.is_negative,
		"negative flag should be %u", tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1->num, tau->expected.num,
		tau->expected.len * sizeof(*tau->expected.num), "");
}

TEST(normal_input, test_100)
{
	u_int in1[] = {0, 0, 1};
	u_int out[] = {0, 0, 1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bi_trim(tau->num1);

	CHECK(
		tau->num1->len == tau->expected.len, "length should be %zu",
		tau->expected.len);
	CHECK(
		tau->num1->is_negative == tau->expected.is_negative,
		"negative flag should be %u", tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1->num, tau->expected.num,
		tau->expected.len * sizeof(*tau->expected.num), "");
}

TEST(normal_input, test_neg100)
{
	u_int in1[] = {0, 0, 1};
	u_int out[] = {0, 0, 1};

	num1 = (bigint){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	bi_trim(tau->num1);

	CHECK(
		tau->num1->len == tau->expected.len, "length should be %zu",
		tau->expected.len);
	CHECK(
		tau->num1->is_negative == tau->expected.is_negative,
		"negative flag should be %u", tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->num1->num, tau->expected.num,
		tau->expected.len * sizeof(*tau->expected.num), "");
}

/*###################################################################*/
/*###################################################################*/

/*###################################################################*/
/*###################################################################*/
