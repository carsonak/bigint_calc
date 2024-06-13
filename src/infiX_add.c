#include "infiX.h"

static u4b_array *add_negatives(u4b_array *n1, u4b_array *n2)
	__attribute__((nonnull));

/**
 * infiX_addition - adds integers stored in arrays
 * @n1: the first integer array (base 10)
 * @n2: the second integer array (base 10)
 *
 * Return: pointer to result, NULL on failure
 */
u4b_array *infiX_addition(u4b_array *n1, u4b_array *n2)
{
	size_t n1_i = 0, n2_i = 0, sum_i = 0, result_len = 0;
	int64_t byt_sum = 0;
	u4b_array *sum = NULL;

	if (!n1 || !n2)
		return (NULL);

	if (n1->is_negative || n2->is_negative)
		return (add_negatives(n1, n2));

	trim_u4b_array(n1);
	trim_u4b_array(n2);
	/*sum->len = (larger of n1->len or n2->len, +1 for a carry)*/
	result_len = ((n1->len > n2->len) ? n1->len : n2->len) + 1;
	if (result_len <= 1)
		return (alloc_u4b_array(0));

	sum = alloc_u4b_array(result_len);
	while (sum && (n1_i < n1->len || n2_i < n2->len || byt_sum > 0))
	{
		if (n1_i < n1->len)
		{
			byt_sum += n1->array[n1_i];
			++n1_i;
		}

		if (n2_i < n2->len)
		{
			byt_sum += n2->array[n2_i];
			++n2_i;
		}

		sum->array[sum_i] = byt_sum % (MAX_VAL_u4b);
		byt_sum /= (MAX_VAL_u4b);
		++sum_i;
	}

	trim_u4b_array(sum);
	return (sum);
}

/**
 * add_negatives - handle addition of signed/negative numbers
 * @n1: first number
 * @n2: second number
 *
 * Return: return results of operation
 */
u4b_array *add_negatives(u4b_array *n1, u4b_array *n2)
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
		/*-8 + -7 = -(8+7)*/
		n1->is_negative = 0;
		n2->is_negative = 0;
		result = infiX_addition(n1, n2);
		n1->is_negative = 1;
		n2->is_negative = 1;
		if (result)
			result->is_negative = 1;
	}
	else if (n1->is_negative)
	{
		/*-8 + 7 = 7-8*/
		n1->is_negative = 0;
		result = infiX_subtraction(n2, n1);
		n1->is_negative = 1;
	}
	else if (n2->is_negative)
	{
		/*8 + -7 = 8-7*/
		n2->is_negative = 0;
		result = infiX_subtraction(n1, n2);
		n2->is_negative = 1;
	}
	else
		result = infiX_addition(n1, n2);

	return (result);
}
