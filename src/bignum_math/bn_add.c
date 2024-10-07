#include "bignum_math.h"

static ATTR_NONNULL bignum *add(bignum *const n1, bignum *const n2);
static ATTR_NONNULL bignum *add_negatives(bignum *const n1, bignum *const n2);

/**
 * add - add two bignums.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to result, NULL on failure.
 */
static bignum *add(bignum *const n1, bignum *const n2)
{
	size_t n1_i = 0, n2_i = 0, sum_i = 0, result_len = 0;
	lint byt_sum = 0;
	bignum *sum = NULL;

	/*sum->len = (larger of n1->len or n2->len, +1 for a carry)*/
	result_len = ((n1->len > n2->len) ? n1->len : n2->len) + 1;
	if (result_len <= 1)
		return (bn_alloc(0));

	sum = bn_alloc(result_len);
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

		sum->num[sum_i] = byt_sum % (BIGNUM_BASE);
		byt_sum /= (BIGNUM_BASE);
		++sum_i;
	}

	if (sum_i < sum->len)
		memset(&sum->num[sum_i], 0, sizeof(*sum->num) * (sum->len - sum_i));

	trim_bignum(sum);
	return (sum);
}

/**
 * add_negatives - handle addition of signed bignums.
 * @n1: first number.
 * @n2: second number.
 *
 * Return: pointer to the result, NULL on failure.
 */
static bignum *add_negatives(bignum *const n1, bignum *const n2)
{
	bool neg1 = n1->is_negative, neg2 = n2->is_negative;
	bignum *result = NULL;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2)
	{
		/*-8 + -7 = -(8+7)*/
		result = add(n1, n2);
		if (result)
			result->is_negative = !result->is_negative;
	}
	else if (neg1) /*-8 + 7 = 7-8*/
		result = bn_subtract(n2, n1);
	else if (neg2) /*8 + -7 = 8-7*/
		result = bn_subtract(n1, n2);

	n1->is_negative = neg1;
	n2->is_negative = neg2;
	trim_bignum(result);
	return (result);
}

/**
 * bn_add - handle addition of two bignums.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to result, NULL on failure.
 */
bignum *bn_add(bignum *const n1, bignum *const n2)
{
	if (!n1 || !n2)
		return (NULL);

	trim_bignum(n1);
	trim_bignum(n2);
	if (n1->is_negative || n2->is_negative)
		return (add_negatives(n1, n2));

	return (add(n1, n2));
}
