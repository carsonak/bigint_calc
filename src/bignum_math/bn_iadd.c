#include "bignum_math.h"

static ATTR_NONNULL void add(bignum * const n1, bignum * const n2);
static ATTR_NONNULL bool add_negatives(bignum * const n1, bignum * const n2);

/**
 * add - add two bignums inplace.
 * @n1: the first number, should be large enough to store the results.
 * @n2: the second number.
 */
static void add(bignum * const n1, bignum * const n2)
{
	size_t n1_i = 0, n2_i = 0, res_len = 0;
	lint byt_sum = 0;

	while (n1_i < n1->len || n2_i < n2->len || byt_sum > 0)
	{
		if (n1_i < n1->len)
			byt_sum += n1->num[n1_i];

		if (n2_i < n2->len)
		{
			byt_sum += n2->num[n2_i];
			++n2_i;
		}

		n1->num[n1_i] = byt_sum % (BIGNUM_BASE);
		byt_sum /= (BIGNUM_BASE);
		++n1_i;
		++res_len;
	}

	n1->len = res_len;
	trim_bignum(n1);
}

/**
 * add_negatives - handle inplace addition of signed bignums.
 * @n1: first number.
 * @n2: second number.
 *
 * Return: true on success, false on failure.
 */
static bool add_negatives(bignum * const n1, bignum * const n2)
{
	bool neg1 = n1->is_negative, neg2 = n2->is_negative;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2) /*-8 + -7 = -(8+7)*/
	{
		add(n1, n2);
		n1->is_negative = !n1->is_negative;
		n2->is_negative = true;
	}
	else if (neg1) /*-8 + 7 = -(8-7)*/
	{
		if (!bn_sub_inplace(n1, n2))
			return (false);

		n1->is_negative = !n1->is_negative;
	}
	else if (neg2) /*8 + -7 = 8-7*/
	{
		if (!bn_sub_inplace(n1, n2))
			return (false);

		n2->is_negative = true;
	}

	trim_bignum(n1);
	return (true);
}

/**
 * bn_add_inplace - add two bignums and store results in first number.
 * @n1: the first number, should have enough space to store the result.
 * @n2: the second number.
 *
 * The results of the addittion will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * Return: 1 on success, 0 on failure (if n1 is NULL).
 */
bool bn_add_inplace(bignum * const n1, bignum * const n2)
{
	/*n1.num cannot be NULL as this function does not allocate any memory.*/
	if (!n1 || !n1->num)
		return (false);

	trim_bignum(n1);
	if (!n2) /*This case is treated as +(n1).*/
		return (true);

	trim_bignum(n2);
	if (n1->is_negative || n2->is_negative)
		return (add_negatives(n1, n2));

	add(n1, n2);
	return (true);
}
