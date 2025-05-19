#include "_bigint_internals.h"
#include "bigint.h"

static ATTR_NONNULL bigint *
add(const bigint *const restrict n1, const bigint *const restrict n2);
static ATTR_NONNULL bigint *
add_negatives(bigint *const restrict n1, bigint *const restrict n2);

/**
 * add - add two bigints.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to result, NULL on failure.
 */
static bigint *
add(const bigint *const restrict n1, const bigint *const restrict n2)
{
	len_type n1_i = 0, n2_i = 0, sum_i = 0;
	/* sum->len = (larger of n1->len or n2->len, +1 for a carry) */
	const len_type result_len = ((n1->len > n2->len) ? n1->len : n2->len) + 1;
	l_int byt_sum = 0;
	bigint *sum = NULL;

	if (result_len <= 1)
		return (_bi_alloc(0));

	sum = _bi_alloc(result_len);
	if (!sum)
		return (NULL);

	while (n1_i < n1->len || n2_i < n2->len || byt_sum > 0)
	{
		if (n1_i < n1->len)
		{
			byt_sum += n1->num[n1_i];
			++n1_i;
		}

		if (n2_i < n2->len)
		{
			byt_sum += n2->num[n2_i];
			++n2_i;
		}

		sum->num[sum_i] = byt_sum % BIGINT_BASE;
		byt_sum /= BIGINT_BASE;
		++sum_i;
	}

	if (sum_i < sum->len)
		memset(&sum->num[sum_i], 0, sizeof(*sum->num) * (sum->len - sum_i));

	_bi_trim(sum);
	return (sum);
}

/**
 * add_negatives - handle addition of signed bigints.
 * @n1: first number.
 * @n2: second number.
 *
 * Return: pointer to the result, NULL on failure.
 */
static bigint *
add_negatives(bigint *const restrict n1, bigint *const restrict n2)
{
	bool neg1 = n1->is_negative, neg2 = n2->is_negative;
	bigint *result = NULL;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2)
	{
		/* -8 + -7 = -(8+7) */
		result = add(n1, n2);
		if (result)
			result->is_negative = !result->is_negative;
	}
	else if (neg1) /* -8 + 7 = 7-8 */
		result = bi_subtract(n2, n1);
	else if (neg2) /* 8 + -7 = 8-7 */
		result = bi_subtract(n1, n2);

	n1->is_negative = neg1;
	n2->is_negative = neg2;
	_bi_trim(result);
	return (result);
}

/**
 * bi_add - handle addition of two bigints.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to result, NULL on failure.
 */
bigint *bi_add(bigint *const restrict n1, bigint *const restrict n2)
{
	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (NULL);

	_bi_trim(n1);
	_bi_trim(n2);
	if (bi_isNaN(n1) || bi_isNaN(n2))
		return (_bi_alloc(0));

	if (n1->is_negative || n2->is_negative)
		return (add_negatives(n1, n2));

	return (add(n1, n2));
}

/**
 * bi_add_int - add an int to a `bigint`.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to the answer on success, NULL on failure.
 */
bigint *bi_add_int(bigint *const n1, const intmax_t n2)
{
	if (!n1 || n1->len < 0)
		return (NULL);

	bigint num2 = {.len = 4, .is_negative = 0, .num = (u_int[4]){0}};

	return (bi_add(n1, int_to_bi(&num2, n2)));
}
