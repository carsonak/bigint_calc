#include "bignum_math.h"

static ATTR_NONNULL bignum *multiply(bignum *const n1, bignum *const n2);
static ATTR_NONNULL bignum *
multiply_negatives(bignum *const n1, bignum *const n2);

/**
 * multiply - multiply two bignums.
 * @n1: first number.
 * @n2: second number.
 *
 * Return: pointer to result, NULL on failure.
 */
static bignum *multiply(bignum *const n1, bignum *const n2)
{
	l_int byt_prod = 0;
	size_t n1_i = 0, n2_i = 0;
	bignum *product = NULL, *current_mul = NULL;

	/*Multiplication by zero.*/
	if (is_zero(n1) || is_zero(n2))
		return (bn_alloc(1));

	/*Length of product = length of n1 + length of n2*/
	product = bn_alloc(n1->len + n2->len);
	current_mul = bn_alloc(n1->len + n2->len);
	if (!product || !current_mul)
		goto clean_up;

	product->len = 1;
	/*For every "digit" in n2, multiply with every "digit" in n1.*/
	for (n2_i = 0; n2_i < n2->len; n2_i++)
	{
		/*Skip multiplication by zero*/
		if (n2->num[n2_i] == 0)
			continue;

		/*Length of current_mul = */
		/*length of n1 + (number of "digits" upto n2_i)*/
		current_mul->len = n1->len + (n2_i + 1);
		/*The least significant n2_i "digits" will be 0 for every iteration.*/
		memset(current_mul->num, 0, sizeof(*current_mul->num) * n2_i);
		byt_prod = 0;
		for (n1_i = 0; n1_i < n1->len; n1_i++)
		{
			byt_prod += (l_int)n2->num[n2_i] * n1->num[n1_i];
			current_mul->num[n2_i + n1_i] = byt_prod % BIGNUM_BASE;
			byt_prod /= BIGNUM_BASE;
		}

		current_mul->num[n2_i + n1_i] = byt_prod;
		if (!bn_iadd(product, current_mul))
			break;
	}

clean_up:
	current_mul = bn_free(current_mul);
	if (n2_i < n2->len)
		product = bn_free(product);

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
static bignum *multiply_negatives(bignum *const n1, bignum *const n2)
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
 * bn_multiply - handle multiplication of two bignums.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to result, NULL on failure.
 */
bignum *bn_multiply(bignum *const n1, bignum *const n2)
{
	if (!n1 || !n2)
		return (NULL);

	trim_bignum(n1);
	trim_bignum(n2);
	if (n1->is_negative || n2->is_negative)
		return (multiply_negatives(n1, n2));

	return (multiply(n1, n2));
}
