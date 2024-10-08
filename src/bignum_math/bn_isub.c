#include "bignum_math.h"

static ATTR_NONNULL void subtract(bignum * const n1, bignum * const n2);
static ATTR_NONNULL bool
subtract_negatives(bignum * const n1, bignum * const n2);

/**
 * subtract - subtract two bignums inplace.
 * @n1: first number.
 * @n2: second numbers.
 *
 * Return: pointer to the result, NULL on failure.
 */
static void subtract(bignum * const n1, bignum * const n2)
{
	size_t n1_i = 0, n2_i = 0, tmp_len = 0, final_len = 0;
	l_int n1_is_bigger = 0, byt_diff = 0;

	/*result_len = max(n1->len, n2->len)*/
	tmp_len = (n1->len > n2->len) ? n1->len : n2->len;
	/*If both arrays are of the same length then;*/
	/*result_len = n1->len - */
	/*(length of continuous matches in n1 and n2 from msd down to lsd)*/
	if (n1->len == n2->len)
		while (tmp_len > 1 && n1->num[tmp_len - 1] == n2->num[tmp_len - 1])
			tmp_len--;

	n1_is_bigger = bn_compare(n1, n2);
	while (n1_i < tmp_len && (n1_i < n1->len || n2_i < n2->len))
	{
		if (n1_is_bigger > 0) /*then; n1 - n2*/
		{
			if (n2_i < n2->len)
				byt_diff += (l_int)n1->num[n1_i] - n2->num[n2_i];
			else
				byt_diff += n1->num[n1_i];
		}
		else /*n2 - n1*/
		{
			if (n1_i < n1->len)
				byt_diff += (l_int)n2->num[n2_i] - n1->num[n1_i];
			else
				byt_diff += n2->num[n2_i];
		}

		if (byt_diff < 0) /*borrow 1 from next.*/
		{
			byt_diff += BIGNUM_BASE;
			n1->num[n1_i] = byt_diff % BIGNUM_BASE;
			byt_diff = -1;
			final_len++;
		}
		else
		{
			/*For cases such as: 1,000,000,000 - 5 = 999,999,995*/
			/*n1 is reduced in length from the original number, therefore */
			/*don't update n1 as the borrow reduces the next "digit" to 0.*/
			if (byt_diff || (n2_i + 1 < n2->len || n1_i < n1->len))
			{
				n1->num[n1_i] = byt_diff % BIGNUM_BASE;
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
static bool subtract_negatives(bignum * const n1, bignum * const n2)
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
		if (!bn_iadd(n1, n2))
			return (false);
	}
	else if (neg1)
	{
		/*-8 - 5 = -(8+5)*/
		if (!bn_iadd(n1, n2))
			return (false);

		n1->is_negative = !n1->is_negative;
	}

	n2->is_negative = neg2;
	trim_bignum(n1);
	return (true);
}

/**
 * bn_isubtract - handles subtraction of two bignums inplace.
 * @n1: first number, must have enough space to store the result.
 * @n2: second number.
 *
 * The results of the subtraction will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * Return: 1 on success, 0 on failure (if n1 is NULL).
 */
bool bn_isubtract(bignum * const n1, bignum * const n2)
{
	/*n1.num cannot be NULL as this function does not allocate any memory.*/
	if (!n1 || !n1->num)
		return (false);

	trim_bignum(n1);
	if (!n2) /*This case is treated as -(n1).*/
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
