#include "infiX.h"

static u4b_bignum *multiply_negatives(u4b_bignum *n1, u4b_bignum *n2)
	ATTR_NONNULL;
static u4b_bignum *multiply(u4b_bignum *n1, u4b_bignum *n2) ATTR_NONNULL;

/**
 * infiX_multiplication - multiply arbitrary long numbers.
 * @n1: the first number.
 * @n2: the second number.
 *
 * This function does preliminary checks on the parameters.
 *
 * Return: pointer to result, NULL on failure
 */
u4b_bignum *infiX_multiplication(u4b_bignum *n1, u4b_bignum *n2)
{
	if (!n1 || !n2)
		return (NULL);

	trim_u4b_array(n1);
	trim_u4b_array(n2);
	if (n1->is_negative || n2->is_negative)
		return (multiply_negatives(n1, n2));

	return (multiply(n1, n2));
}

/**
 * multiply - multiply arbitrary long numbers.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to result, NULL on failure
 */
u4b_bignum *multiply(u4b_bignum *n1, u4b_bignum *n2)
{
	int64_t byt_mul = 0;
	size_t n1_i = 0, n2_i = 0;
	u4b_bignum *product = NULL, *current_mul = NULL, *increment = NULL;

	/*Multiplication by zero.*/
	if (!n1->len || !n2->len)
		return (alloc_u4b_array(0));
	else if ((n1->len == 1 && !n1->array[0]) || (n2->len == 1 && !n2->array[0]))
		return (alloc_u4b_array(1));

	increment = alloc_u4b_array(0);
	if (!increment)
		return (NULL);

	/*Iterate over every number in n2 and multiply with every number in n1.*/
	for (n2_i = 0; n2_i < n2->len; n2_i++)
	{
		/*Skip multiplication by zero*/
		if (n2->array[n2_i] == 0)
			continue;

		/*Length of current_mul = */
		/*length of n1 + (number of digits between n2[0] and n2[n2_i])*/
		current_mul = alloc_u4b_array(n1->len + n2_i + 1);
		if (!current_mul)
			return (free_u4b_array(product));

		byt_mul = 0;
		for (n1_i = 0; n1_i < n1->len; n1_i++)
		{
			byt_mul += (int64_t)n2->array[n2_i] * n1->array[n1_i];
			current_mul->array[n2_i + n1_i] = byt_mul % MAX_VAL_u4b;
			byt_mul /= MAX_VAL_u4b;
		}

		current_mul->array[n2_i + n1_i] = byt_mul;
		product = infiX_addition(increment, current_mul);
		current_mul = free_u4b_array(current_mul);
		increment = free_u4b_array(increment);
		if (!product)
			return (NULL);

		increment = product;
	}

	trim_u4b_array(product);
	return (product);
}

/**
 * multiply_negatives - multiplication of signed numbers (-ve numbers) or zero
 * @n1: number to be multiplied
 * @n2: number to multiply
 *
 * Return: pointer to the result, NULL on failure
 */
u4b_bignum *multiply_negatives(u4b_bignum *n1, u4b_bignum *n2)
{
	char neg1 = n1->is_negative, neg2 = n2->is_negative;
	u4b_bignum *result = NULL;

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
	trim_u4b_array(result);
	return (result);
}
