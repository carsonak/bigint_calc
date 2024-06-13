#include "infiX.h"

static u4b_array *multiply_negatives(u4b_array *n1, u4b_array *n2)
	__attribute__((nonnull));

/**
 * infiX_multiplication - multiplies numbers stored in arrays.
 * @n1: the first integer array (base 10)
 * @n2: the second integer array (base 10)
 *
 * Return: pointer to result, NULL on failure
 */
u4b_array *infiX_multiplication(u4b_array *n1, u4b_array *n2)
{
	int64_t byt_mul = 0;
	size_t n1_i = 0, n2_i = 0;
	u4b_array *product = NULL, *cur_mul = NULL, *sum = NULL;

	if (!n1 || !n2)
		return (NULL);

	if (n1->is_negative || n2->is_negative)
		return (multiply_negatives(n1, n2));

	trim_u4b_array(n1);
	trim_u4b_array(n2);
	/*Multiplication by zero.*/
	if (!n1->len || !n2->len)
		return (alloc_u4b_array(0));
	else if ((n1->len == 1 && !n1->array[0]) || (n2->len == 1 && !n2->array[0]))
		return (alloc_u4b_array(1));

	/*product->len = n1->len + n2->len - (0 or 1).*/
	product = alloc_u4b_array(n1->len + n2->len);
	if (!product)
		return (NULL);

	product->len = 1;
	/*Iterate over every number in n2 and multiply with every number in n1.*/
	for (n2_i = 0; n2_i <= n2->len; n2_i++)
	{
		/*Skip multiplication by zero*/
		if (n2->array[n2_i] == 0)
			continue;

		/*Length of cur_mul = length of n1 + current index of n2*/
		cur_mul = alloc_u4b_array(n1->len + n2_i);
		if (!cur_mul)
			return (free_u4b_array(product));

		byt_mul = 0;
		for (n1_i = 0; n1_i < n1->len; n1_i++)
		{
			byt_mul += (int64_t)n2->array[n2_i] * n1->array[n1_i];
			cur_mul->array[n2_i + n1_i] = byt_mul % MAX_VAL_u4b;
			byt_mul /= MAX_VAL_u4b;
		}

		cur_mul->array[n2_i + n1_i] = byt_mul;
		sum = infiX_addition(product, cur_mul);
		if (sum)
		{
			product->len = sum->len;
			/*Skip over the first n2_i indices as they will be unchanged.*/
			memmove(product->array, sum->array + n2_i, sum->len - n2_i);
		}

		cur_mul = free_u4b_array(cur_mul);
		if (!sum)
		{
			product = free_u4b_array(product);
			return (NULL);
		}

		sum = free_u4b_array(sum);
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
u4b_array *multiply_negatives(u4b_array *n1, u4b_array *n2)
{
	u4b_array *result = NULL;

	if (!n1 || !n2)
		return (NULL);

	if (!n1->array)
		n1->is_negative = 0;

	if (!n2->array)
		n2->is_negative = 0;

	if (n1->is_negative && n2->is_negative)
	{
		/* -8 * -7 = 8*7 */
		n1->is_negative = 0;
		n2->is_negative = 0;
		result = infiX_multiplication(n1, n2);
		n1->is_negative = 1;
		n2->is_negative = 1;
	}
	else if (n1->is_negative)
	{
		/* -8 * 7 = -(8*7) */
		n1->is_negative = 0;
		result = infiX_multiplication(n1, n2);
		n1->is_negative = 1;
		if (result)
			result->is_negative = 1;
	}
	else if (n2->is_negative)
	{
		/* 8 * -7 = -(8*7) */
		n2->is_negative = 0;
		result = infiX_multiplication(n1, n2);
		n2->is_negative = 1;
		if (result)
			result->is_negative = 1;
	}
	else
		result = infiX_multiplication(n1, n2);

	return (result);
}
