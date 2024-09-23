#include "bignum_math.h"

static ATTR_NONNULL void subtract(bignum *n1, bignum *n2);
static ATTR_NONNULL bool subtract_negatives(bignum *n1, bignum *n2);

/**
 * subtract - subtract two bignums inplace.
 * @n1: first number.
 * @n2: second numbers.
 *
 * Return: pointer to the result, NULL on failure.
 */
static void subtract(bignum *n1, bignum *n2)
{
	size_t n1_i = 0, n2_i = 0, tmp_len = 0, final_len = 0;
	lint n1_is_bigger = 0, byt_diff = 0;

	/*result_len = max(n1->len, n2->len)*/
	tmp_len = (n1->len > n2->len) ? n1->len : n2->len;
	/*If both arrays are of the same length then;*/
	/*result_len = n1->len - */
	/*(length of continuous matches in n1 and n2 from msd down to lsd)*/
	if (n1->len == n2->len)
		while (tmp_len > 1 && n1->num[tmp_len - 1] == n2->num[tmp_len - 1])
			tmp_len--;

	n1_is_bigger = cmp_bignum(n1, n2);
	while (n1_i < tmp_len && (n1_i < n1->len || n2_i < n2->len))
	{
		if (n1_is_bigger > 0) /*then; n1 - n2*/
		{
			if (n2_i < n2->len)
				byt_diff += (lint)n1->num[n1_i] - n2->num[n2_i];
			else
				byt_diff += n1->num[n1_i];
		}
		else /*n2 - n1*/
		{
			if (n1_i < n1->len)
				byt_diff += (lint)n2->num[n2_i] - n1->num[n1_i];
			else
				byt_diff += n2->num[n2_i];
		}

		if (byt_diff < 0) /*borrow 1 from next.*/
		{
			byt_diff += BIGNUM_UINT_MAX;
			n1->num[n1_i] = byt_diff % BIGNUM_UINT_MAX;
			byt_diff = -1;
			final_len++;
		}
		else
		{
			/*For the case: 1,000,000,000 - 5 = 999,999,995*/
			/*A borrow needs to be performed but may cause an out of bounds*/
			/*n1 access, as n1 needs only be large enough to hold the answer.*/
			if (byt_diff || (n2_i + 1 < n2->len || n1_i < n1->len))
			{
				n1->num[n1_i] = byt_diff % BIGNUM_UINT_MAX;
				final_len++;
			}

			byt_diff = 0;
		}

		++n1_i;
		++n2_i;
	}

	if (n1_is_bigger <= 0)
		n1->is_negative = true;

	n1->len = final_len;
	trim_bignum(n1);
}

/**
 * subtract_negatives - handle subtraction of two signed bignums.
 * @n1: first number.
 * @n2: second number.
 *
 * Return: pointer to the result, NULL on failure.
 */
static bool subtract_negatives(bignum *n1, bignum *n2)
{
	bool neg1 = n1->is_negative, neg2 = n2->is_negative;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2) /*-8 - -5 = -(8-5)*/
	{
		subtract(n1, n2);
		n1->is_negative = !n1->is_negative;
	}
	else if (neg2) /*8 - -5 = 8+5*/
	{
		if (!bn_add_inplace(n1, n2))
			return (false);
	}
	else if (neg1)
	{
		/*-8 - 5 = -(8+5)*/
		if (!bn_add_inplace(n1, n2))
			return (false);

		n1->is_negative = !n1->is_negative;
	}

	n2->is_negative = neg2;
	trim_bignum(n1);
	return (true);
}

/**
 * bn_sub_inplace - handles subtraction of two bignums inplace.
 * @n1: first number, must have enough space to store the result.
 * @n2: second number.
 *
 * The results of the subtraction will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * Return: pointer to the result, NULL on failure.
 */
bool bn_sub_inplace(bignum *n1, bignum *n2)
{
	if (!n1 || !n1->num)
		return (false);

	trim_bignum(n1);
	if (!n2 || !n2->num)
	{
		n1->is_negative = !n1->is_negative;
		return (true);
	}

	trim_bignum(n2);
	if (n1->is_negative || n2->is_negative)
		return (subtract_negatives(n1, n2));

	subtract(n1, n2);
	return (true);
}
