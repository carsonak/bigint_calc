#include "_bigint_internals.h"
#include "bigint.h"

static ATTR_NONNULL void
iadd(bigint *const restrict n1, bigint const *const restrict n2);
static ATTR_NONNULL bool
iadd_negatives(bigint *const restrict n1, bigint *const restrict n2);

/**
 * iadd - add two bigints inplace.
 * @n1: the first number, should be large enough to store the results.
 * @n2: the second number.
 */
static void iadd(bigint *const restrict n1, bigint const *const restrict n2)
{
	len_typ n1_i = 0, n2_i = 0, res_len = 0;
	l_int byt_sum = 0;

	while (n1_i < n1->len || n2_i < n2->len || byt_sum > 0)
	{
		if (n1_i < n1->len)
			byt_sum += n1->num[n1_i];

		if (n2_i < n2->len)
		{
			byt_sum += n2->num[n2_i];
			++n2_i;
		}

		n1->num[n1_i] = byt_sum % (BIGINT_BASE);
		byt_sum /= (BIGINT_BASE);
		++n1_i;
		++res_len;
	}

	n1->len = res_len;
	_bi_trim(n1);
}

/**
 * iadd_negatives - handle inplace addition of signed bigints.
 * @n1: first number.
 * @n2: second number.
 *
 * Return: true on success, false on failure.
 */
static bool
iadd_negatives(bigint *const restrict n1, bigint *const restrict n2)
{
	bool neg1 = n1->is_negative, neg2 = n2->is_negative;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2) /* -8 + -7 = -(8+7) */
	{
		iadd(n1, n2);
		n1->is_negative = !n1->is_negative;
		n2->is_negative = true;
	}
	else if (neg1) /* -8 + 7 = -(8-7) */
	{
		if (!bi_isubtract(n1, n2))
			return (false);

		n1->is_negative = !n1->is_negative;
	}
	else if (neg2) /* 8 + -7 = 8-7 */
	{
		if (!bi_isubtract(n1, n2))
			return (false);

		n2->is_negative = true;
	}

	_bi_trim(n1);
	return (true);
}

/**
 * bi_iadd - add two bigints and store results in first number.
 * @n1: the first number, should have enough space to store the result.
 * @n2: the second number.
 *
 * The results of the addittion will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * Return: 1 on success, 0 on failure.
 */
bool bi_iadd(bigint *const restrict n1, bigint *const restrict n2)
{
	/* n1.num cannot be NULL as this function does not allocate any memory. */
	if (!n1 || !n1->num || n1->len < 0)
		return (false);

	_bi_trim(n1);
	if (!n2) /* This case is treated as: n1 = n1 * 1. */
		return (true);

	_bi_trim(n2);
	if (bi_isNaN(n1) || bi_isNaN(n2))
		return (false);

	if (n1->is_negative || n2->is_negative)
		return (iadd_negatives(n1, n2));

	iadd(n1, n2);
	return (true);
}

/**
 * bi_iadd_int - add an int to a `bigint` inplace.
 * @n1: the first number, must have enough memory allocated to hold the answer.
 * @n2: the second number.
 *
 * The results of the addittion will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * Return: true on success, false on failure.
 */
bool bi_iadd_int(bigint *const n1, const intmax_t n2)
{
	bigint num2 = {.len = 4, .is_negative = 0, .num = (u_int[3]){0}};

	if (!n1 || n1->len < 0)
		return (false);

	int_to_bi(&num2, n2);
	return (bi_iadd(n1, &num2));
}
