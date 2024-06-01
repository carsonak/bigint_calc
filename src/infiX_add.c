#include "infiX.h"

u4b_array *add_negatives(u4b_array *n1, u4b_array *n2);

/**
 * infiX_addition - adds integers stored in arrays
 * @n1: the first integer array (base 10)
 * @n2: the second integer array (base 10)
 *
 * Return: pointer to result, NULL on failure
 */
u4b_array *infiX_addition(u4b_array *n1, u4b_array *n2)
{
	size_t n1_i = 0, n2_i = 0, sum_i = 0;
	int64_t byt_sum = 0;
	u4b_array *sum = NULL;

	if (!n1 || !n2 || !n1->array || !n2->array)
		return (NULL);

	if (n1->is_negative || n2->is_negative)
		return (add_negatives(n1, n2));

	trim_u4b_array(n1);
	trim_u4b_array(n2);
	sum = check_calloc(1, sizeof(*sum));
	if (!sum)
		return (NULL);

	/*Size of the sum array = (larger of n1->len or n2->len, +1 for a carry)*/
	sum->len = ((n1->len > n2->len) ? n1->len : n2->len) + 1;
	if (sum->len <= 1)
	{
		sum->len = 0;
		return (sum);
	}

	sum->array = check_calloc(sum->len, sizeof(*sum->array));
	if (!sum->array)
	{
		free_n_null(sum);
		return (NULL);
	}

	while (n1_i < n1->len || n2_i < n2->len || byt_sum > 0)
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

	if (!n1 || !n2 || !n1->array || !n2->array)
		return (NULL);

	trim_u4b_array(n1);
	trim_u4b_array(n2);
	if (n1->is_negative && n2->is_negative)
	{
		/*-8 + -7 = -(8+7)*/
		n1->is_negative = 0;
		n2->is_negative = 0;
		result = infiX_addition(n1, n2);
		result->is_negative = 1;
	}
	else if (n1->is_negative)
	{
		/*-8 + 7 = 7-8*/
		n1->is_negative = 0;
		result = infiX_subtraction(n2, n1);
	}
	else if (n2->is_negative)
	{
		/*8 + -7 = 8-7*/
		n2->is_negative = 0;
		result = infiX_subtraction(n1, n2);
	}
	else
		result = infiX_addition(n1, n2);

	return (result);
}
