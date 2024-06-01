#include "infiX.h"

u4b_array *subtract_negatives(u4b_array *n1, u4b_array *n2);

/**
 * infiX_subtraction - subtract large numbers stored in arrays
 * @n1: number to be subtracted
 * @n2: number to subtract
 *
 * Return: pointer to the diff, NULL on failure
 */
u4b_array *infiX_subtraction(u4b_array *n1, u4b_array *n2)
{
	size_t n1_i = 0, n2_i = 0, diff_i = 0;
	int64_t byt_diff = 0;
	u4b_array *diff = NULL;

	if (!n1 || !n2 || !n1->array || !n2->array)
		return (NULL);

	if (n1->is_negative || n2->is_negative)
		return (subtract_negatives(n1, n2));

	trim_u4b_array(n1);
	trim_u4b_array(n2);
	diff = check_calloc(1, sizeof(*diff));
	if (!diff)
		return (NULL);

	/*diff->len = len of longest index*/
	diff->len = (n1->len > n2->len) ? n1->len : n2->len;
	/*If both are the same size;*/
	/*reduce diff->len by length of consecutive matching ints in n1 and n2*/
	/*from their highest index.*/
	if (n1->len == n2->len)
		while (diff->len > 1 && n1->array[diff->len] == n2->array[diff->len])
			diff->len--;

	diff->array = check_calloc((diff->len), sizeof(*diff->array));
	if (!diff->array)
	{
		free_n_null(diff);
		return (NULL);
	}

	while ((n1_i < n1->len || n2_i < n2->len) && diff_i < diff->len)
	{
		/*If n1 is greater than n2*/
		if (n1->len > n2->len ||
			(n1->len == n2->len && n1->array[diff->len - 1] > n2->array[diff->len - 1]))
		{
			if (n2_i < n2->len) /*n1 - n2*/
				byt_diff += (int64_t)n1->array[n1_i] - n2->array[n2_i];
			else
				byt_diff += n1->array[n1_i];
		}
		else
		{
			if (n1_i < n1->len) /*n2 - n1*/
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
	u4b_array *difference = NULL;

	if (!n1 || !n2 || !n1->array || !n2->array)
		return (NULL);

	trim_u4b_array(n1);
	trim_u4b_array(n2);
	if (n1->is_negative && n2->is_negative)
	{
		/*-8 - -5 = -8 + 5 = 5-8*/
		n2->is_negative = 0;
		difference = infiX_subtraction(n2, n1);
	}
	else if (n1->is_negative)
	{
		/*-8 - 5 = -(8+5)*/
		n1->is_negative = 0;
		difference = infiX_addition(n1, n2);
		difference->is_negative = 1;
	}
	else if (n2->is_negative)
	{
		/*8 - -5 = 8+5*/
		n2->is_negative = 0;
		difference = infiX_addition(n1, n2);
	}
	else
		difference = infiX_subtraction(n1, n2);

	return (difference);
}
