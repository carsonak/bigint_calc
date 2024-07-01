#include "infiX.h"

static BigNum *add_negatives(BigNum *n1, BigNum *n2) ATTR_NONNULL;
static BigNum *add(BigNum *n1, BigNum *n2) ATTR_NONNULL;

/**
 * infiX_addition - handle addition of two bignums.
 * @n1: the first number.
 * @n2: the second number.
 *
 * This function does preliminary checks on the parameters.
 *
 * Return: pointer to result, NULL on failure.
 */
BigNum *infiX_addition(BigNum *n1, BigNum *n2)
{
	if (!n1 || !n2)
		return (NULL);

	trim_bignum(n1);
	trim_bignum(n2);
	if (n1->is_negative || n2->is_negative)
		return (add_negatives(n1, n2));

	return (add(n1, n2));
}

/**
 * add_negatives - handle addition of signed bignums.
 * @n1: first number.
 * @n2: second number.
 *
 * Return: pointer to the result, NULL on failure.
 */
BigNum *add_negatives(BigNum *n1, BigNum *n2)
{
	char neg1 = n1->is_negative, neg2 = n2->is_negative;
	BigNum *result = NULL;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2)
	{
		/*-8 + -7 = -(8+7)*/
		result = add(n1, n2);
		if (result)
			result->is_negative = true;
	}
	else if (neg1) /*-8 + 7 = 7-8*/
		result = infiX_subtraction(n2, n1);
	else if (neg2) /*8 + -7 = 8-7*/
		result = infiX_subtraction(n1, n2);

	n1->is_negative = neg1;
	n2->is_negative = neg2;
	trim_bignum(result);
	return (result);
}

/**
 * add - add two bignums.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to result, NULL on failure.
 */
BigNum *add(BigNum *n1, BigNum *n2)
{
	unsigned long int n1_i = 0, n2_i = 0, sum_i = 0, result_len = 0;
	long int byt_sum = 0;
	BigNum *sum = NULL;

	/*sum->len = (larger of n1->len or n2->len, +1 for a carry)*/
	result_len = ((n1->len > n2->len) ? n1->len : n2->len) + 1;
	if (result_len <= 1)
		return (alloc_bignum(0));

	sum = alloc_bignum(result_len);
	while (sum && (n1_i < n1->len || n2_i < n2->len || byt_sum > 0))
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

		sum->num[sum_i] = byt_sum % (BIGNUM_UINT_MAX);
		byt_sum /= (BIGNUM_UINT_MAX);
		++sum_i;
	}

	trim_bignum(sum);
	return (sum);
}
