#include "bignum_math.h"

static ATTR_NONNULL bignum *multiply(bignum *n1, bignum *n2);
static ATTR_NONNULL bignum *multiply_negatives(bignum *n1, bignum *n2);

/**
 * multiply - multiply two bignums.
 * @n1: first number.
 * @n2: second number.
 *
 * Return: pointer to result, NULL on failure.
 */
static bignum *multiply(bignum *n1, bignum *n2)
{
	lint byt_prod = 0;
	size_t n1_i = 0, n2_i = 0;
	bignum *product = NULL, *current_mul = NULL, *increment = NULL;

	/*Multiplication by zero.*/
	if (!n1->len || !n2->len)
		return (alloc_bignum(0));
	else if ((n1->len == 1 && !n1->num[0]) || (n2->len == 1 && !n2->num[0]))
		return (alloc_bignum(1));

	increment = alloc_bignum(0);
	if (!increment)
		return (NULL);

	/*Iterate over every number in n2 and multiply with every number in n1.*/
	for (n2_i = 0; n2_i < n2->len; n2_i++)
	{
		/*Skip multiplication by zero*/
		if (n2->num[n2_i] == 0)
			continue;

		/*Length of current_mul = */
		/*length of n1 + (number of digits between n2[0] and n2[n2_i])*/
		current_mul = alloc_bignum(n1->len + n2_i + 1);
		if (!current_mul)
			return (free_bignum(product));

		memset(current_mul->num, 0, sizeof(*current_mul->num) * (n2_i + 1));
		byt_prod = 0;
		for (n1_i = 0; n1_i < n1->len; n1_i++)
		{
			byt_prod += (lint)n2->num[n2_i] * n1->num[n1_i];
			current_mul->num[n2_i + n1_i] = byt_prod % BIGNUM_UINT_MAX;
			byt_prod /= BIGNUM_UINT_MAX;
		}

		current_mul->num[n2_i + n1_i] = byt_prod;
		product = bn_addition(increment, current_mul);
		current_mul = free_bignum(current_mul);
		increment = free_bignum(increment);
		if (!product)
			return (NULL);

		increment = product;
	}

	trim_bignum(product);
	return (product);
}

/**
 * multiply_negatives - handle multiplication of signed bignums.
 * @n1: first number.
 * @n2: second number.
 *
 * Return: pointer to the result, NULL on failure.
 */
static bignum *multiply_negatives(bignum *n1, bignum *n2)
{
	bool neg1 = n1->is_negative, neg2 = n2->is_negative;
	bignum *result = NULL;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2) /* -8 * -7 = 8*7 */
		result = multiply(n1, n2);
	else if (neg1 || neg2)
	{
		/* -8 * 7 = -(8*7) */
		/* 8 * -7 = -(8*7) */
		result = multiply(n1, n2);
		if (result)
			result->is_negative = true;
	}

	n1->is_negative = neg1;
	n2->is_negative = neg2;
	trim_bignum(result);
	return (result);
}

/**
 * bn_multiplication - handle multiplication of two bignums.
 * @n1: the first number.
 * @n2: the second number.
 *
 * This function does preliminary checks on the parameters.
 *
 * Return: pointer to result, NULL on failure.
 */
bignum *bn_multiplication(bignum *n1, bignum *n2)
{
	if (!n1 || !n2)
		return (NULL);

	trim_bignum(n1);
	trim_bignum(n2);
	if (n1->is_negative || n2->is_negative)
		return (multiply_negatives(n1, n2));

	return (multiply(n1, n2));
}
