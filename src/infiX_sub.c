#include "infiX.h"

static u4b_array *subtract_negatives(u4b_array *n1, u4b_array *n2)
	__attribute__((nonnull));

/**
 * infiX_subtraction - subtract large numbers stored in arrays
 * @n1: number to be subtracted
 * @n2: number to subtract
 *
 * Return: pointer to the diff, NULL on failure
 */
u4b_array *infiX_subtraction(u4b_array *n1, u4b_array *n2)
{
	size_t n1_i = 0, n2_i = 0, diff_i = 0, result_len = 0;
	int64_t byt_diff = 0;
	u4b_array *diff = NULL;

	if (!n1 || !n2)
		return (NULL);

	if (n1->is_negative || n2->is_negative)
		return (subtract_negatives(n1, n2));

	trim_u4b_array(n1);
	trim_u4b_array(n2);
	/*result_len = len of longest length*/
	result_len = (n1->len > n2->len) ? n1->len : n2->len;
	/*If both arrays are of the same length;*/
	/*We can reduce result_len as long as digits in n1 are equal to digits*/
	/*in n2, starting from the most significant digits.*/
	if (n1->len == n2->len)
		while (result_len > 2 && n1->array[result_len - 1] == n2->array[result_len - 1])
			result_len--;

	diff = alloc_u4b_array(result_len);
	if (!diff)
		return (diff);

	while ((n1_i < n1->len || n2_i < n2->len) && diff_i < diff->len)
	{
		if (cmp_u4barray(n1, n2) > 0)
		{
			/*n1 - n2*/
			if (n2_i < n2->len)
				byt_diff += (int64_t)n1->array[n1_i] - n2->array[n2_i];
			else
				byt_diff += n1->array[n1_i];
		}
		else
		{
			/*n2 - n1*/
			if (n1_i < n1->len)
				byt_diff += (int64_t)n2->array[n2_i] - n1->array[n1_i];
			else
				byt_diff += n2->array[n2_i];
		}

		if (byt_diff < 0)
		{
			byt_diff += MAX_VAL_u4b;
			diff->array[diff_i] = byt_diff % MAX_VAL_u4b;
			byt_diff = -1;
		}
		else
		{
			diff->array[diff_i] = byt_diff % MAX_VAL_u4b;
			byt_diff = 0;
		}

		++n1_i;
		++n2_i;
		++diff_i;
	}

	if (byt_diff < 0)
	{
		diff_i = diff->len - 1;
		diff->array[diff_i] = MAX_VAL_u4b - diff->array[diff_i];
		diff->is_negative = 1;
	}

	trim_u4b_array(diff);
	return (diff);
}

/**
 * subtract_negatives - subtraction of only signed numbers (negative numbers)
 * @n1: number to be subtracted
 * @n2: number to subtract
 *
 * Return: 1 if action taken (error or processed results), 0 if no action taken
 */
u4b_array *subtract_negatives(u4b_array *n1, u4b_array *n2)
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
		/*-8 - -5 = -8 + 5 = 5-8*/
		n1->is_negative = 0;
		n2->is_negative = 0;
		result = infiX_subtraction(n2, n1);
		n1->is_negative = 1;
		n2->is_negative = 1;
	}
	else if (n1->is_negative)
	{
		/*-8 - 5 = -(8+5)*/
		n1->is_negative = 0;
		result = infiX_addition(n1, n2);
		n1->is_negative = 1;
		if (result)
			result->is_negative = 1;
	}
	else if (n2->is_negative)
	{
		/*8 - -5 = 8+5*/
		n2->is_negative = 0;
		result = infiX_addition(n1, n2);
		n2->is_negative = 1;
	}
	else
		result = infiX_subtraction(n1, n2);

	return (result);
}
