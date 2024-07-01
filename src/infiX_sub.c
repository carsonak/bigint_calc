#include "infiX.h"

ATTR_NONNULL
static BigNum *subtract_negatives(BigNum *n1, BigNum *n2);
static BigNum *subtract(BigNum *n1, BigNum *n2) ATTR_NONNULL;

/**
 * infiX_subtraction - handle subtraction of two bignums.
 * @n1: first number.
 * @n2: second number.
 *
 * This function does preliminary checks on the parameters.
 *
 * Return: pointer to the result, NULL on failure.
 */
BigNum *infiX_subtraction(BigNum *n1, BigNum *n2)
{
	if (!n1 || !n2)
		return (NULL);

	trim_bignum(n1);
	trim_bignum(n2);
	if (n1->is_negative || n2->is_negative)
		return (subtract_negatives(n1, n2));

	return (subtract(n1, n2));
}

/**
 * subtract_negatives - handle subtraction of two signed bignums.
 * @n1: first number.
 * @n2: second number.
 *
 * Return: pointer to the result, NULL on failure.
 */
BigNum *subtract_negatives(BigNum *n1, BigNum *n2)
{
	char neg1 = n1->is_negative, neg2 = n2->is_negative;
	BigNum *result = NULL;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2) /*-8 - -5 = -8 + 5 = 5-8*/
		result = subtract(n2, n1);
	else if (neg2) /*8 - -5 = 8+5*/
		result = infiX_addition(n1, n2);
	else if (neg1)
	{
		/*-8 - 5 = -(8+5)*/
		result = infiX_addition(n1, n2);
		if (result)
			result->is_negative = true;
	}

	n1->is_negative = neg1;
	n2->is_negative = neg2;
	trim_bignum(result);
	return (result);
}

/**
 * subtract - subtract two bignums.
 * @n1: first number.
 * @n2: second numbers.
 *
 * Return: pointer to the result, NULL on failure.
 */
BigNum *subtract(BigNum *n1, BigNum *n2)
{
	unsigned long int n1_i = 0, n2_i = 0, diff_i = 0, result_len = 0;
	long int n1_is_bigger = 0;
	long int byt_diff = 0;
	BigNum *diff = NULL;

	/*result_len = max(n1->len, n2->len)*/
	result_len = (n1->len > n2->len) ? n1->len : n2->len;
	/*If both arrays are of the same length then;*/
	/*result_len = n1->len - */
	/*(length of continuous matches in n1 and n2 from msd down to lsd).*/
	if (n1->len == n2->len)
		while (result_len > 2 && n1->num[result_len - 1] == n2->num[result_len - 1])
			result_len--;

	diff = alloc_bignum(result_len);
	if (!diff)
		return (NULL);

	n1_is_bigger = cmp_bignum(n1, n2);
	if (n1_is_bigger <= 0)
		diff->is_negative = true;

	while ((n1_i < n1->len || n2_i < n2->len) && diff_i < diff->len)
	{
		if (n1_is_bigger > 0) /*then; n1 - n2*/
		{
			if (n2_i < n2->len)
				byt_diff += (long int)n1->num[n1_i] - n2->num[n2_i];
			else
				byt_diff += n1->num[n1_i];
		}
		else /*n2 - n1*/
		{
			if (n1_i < n1->len)
				byt_diff += (long int)n2->num[n2_i] - n1->num[n1_i];
			else
				byt_diff += n2->num[n2_i];
		}

		if (byt_diff < 0) /*borrow 1 from next.*/
		{
			byt_diff += BIGNUM_UINT_MAX;
			diff->num[diff_i] = byt_diff % BIGNUM_UINT_MAX;
			byt_diff = -1;
		}
		else
		{
			diff->num[diff_i] = byt_diff % BIGNUM_UINT_MAX;
			byt_diff = 0;
		}

		++n1_i;
		++n2_i;
		++diff_i;
	}

	trim_bignum(diff);
	return (diff);
}
