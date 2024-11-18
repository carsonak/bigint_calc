#include "bignum_math.h"

static ATTR_NONNULL bignum *subtract(bignum *const n1, bignum *const n2);
static ATTR_NONNULL bignum *
subtract_negatives(bignum *const n1, bignum *const n2);

/**
 * subtract - subtract two bignums.
 * @n1: first number.
 * @n2: second numbers.
 *
 * Return: pointer to the result, NULL on failure.
 */
static bignum *subtract(bignum *const n1, bignum *const n2)
{
	size_t n1_i = 0, n2_i = 0, diff_i = 0, result_len = 0;
	l_int n1_is_bigger = 0, byt_diff = 0;
	bignum *diff = NULL;

	/*result_len = max(n1->len, n2->len)*/
	result_len = (n1->len > n2->len) ? n1->len : n2->len;
	/*If both arrays are of the same length then;*/
	/*result_len = n1->len - */
	/*(length of continuous matches in n1 and n2 from msd down to lsd).*/
	if (n1->len == n2->len)
		while (result_len > 1 && n1->num[result_len - 1] == n2->num[result_len - 1])
			result_len--;

	diff = bn_alloc(result_len);
	if (!diff)
		return (NULL);

	n1_is_bigger = bn_compare(n1, n2);
	if (n1_is_bigger <= 0)
		diff->is_negative = true;

	while ((n1_i < n1->len || n2_i < n2->len) && diff_i < diff->len)
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
			diff->num[diff_i] = byt_diff % BIGNUM_BASE;
			byt_diff = -1;
		}
		else
		{
			diff->num[diff_i] = byt_diff % BIGNUM_BASE;
			byt_diff = 0;
		}

		++n1_i;
		++n2_i;
		++diff_i;
	}

	if (diff_i < diff->len)
		memset(&diff->num[diff_i], 0, sizeof(*diff->num) * (diff->len - diff_i));

	trim_bignum(diff);
	return (diff);
}

/**
 * subtract_negatives - handle subtraction of two signed bignums.
 * @n1: first number.
 * @n2: second number.
 *
 * Return: pointer to the result, NULL on failure.
 */
static bignum *subtract_negatives(bignum *const n1, bignum *const n2)
{
	bool neg1 = n1->is_negative, neg2 = n2->is_negative;
	bignum *result = NULL;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2) /*-8 - -5 = -(8-5)*/
	{
		result = subtract(n1, n2);
		if (result)
			result->is_negative = !result->is_negative;
	}
	else if (neg2) /*8 - -5 = 8+5*/
		result = bn_add(n1, n2);
	else if (neg1)
	{
		/*-8 - 5 = -(8+5)*/
		result = bn_add(n1, n2);
		if (result)
			result->is_negative = !result->is_negative;
	}

	n1->is_negative = neg1;
	n2->is_negative = neg2;
	trim_bignum(result);
	return (result);
}

/**
 * bn_subtract - handle subtraction of two bignums.
 * @n1: first number.
 * @n2: second number.
 *
 * Return: pointer to the result, NULL on failure.
 */
bignum *bn_subtract(bignum *const n1, bignum *const n2)
{
	if (!n1 || !n2)
		return (NULL);

	trim_bignum(n1);
	trim_bignum(n2);
	if (n1->is_negative || n2->is_negative)
		return (subtract_negatives(n1, n2));

	return (subtract(n1, n2));
}
