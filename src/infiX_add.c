#include "infiX.h"

static u4b_array *add_negatives(u4b_array *n1, u4b_array *n2)
	ATTR_NONNULL;
static u4b_array *add(u4b_array *n1, u4b_array *n2) ATTR_NONNULL;

/**
 * infiX_addition - add two arbitrary long numbers.
 * @n1: the first number
 * @n2: the second number
 *
 * This function does preliminary checks on the parameters.
 *
 * Return: pointer to result, NULL on failure
 */
u4b_array *infiX_addition(u4b_array *n1, u4b_array *n2)
{
	if (!n1 || !n2)
		return (NULL);

	trim_u4b_array(n1);
	trim_u4b_array(n2);
	if (n1->is_negative || n2->is_negative)
		return (add_negatives(n1, n2));

	return (add(n1, n2));
}

/**
 * add_negatives - handle addition of signed numbers.
 * @n1: first number
 * @n2: second number
 *
 * Return: return results of operation
 */
u4b_array *add_negatives(u4b_array *n1, u4b_array *n2)
{
	char neg1 = n1->is_negative, neg2 = n2->is_negative;
	u4b_array *result = NULL;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2)
	{
		/*-8 + -7 = -(8+7)*/
		result = add(n1, n2);
		if (result)
			result->is_negative = true;
	}
	else if (neg1) /*-8 + 7 = 7-8*/
		result = infiX_subtraction(n2, n1);
	else if (neg2) /*8 + -7 = 8-7*/
		result = infiX_subtraction(n1, n2);

	n1->is_negative = neg1;
	n2->is_negative = neg2;
	trim_u4b_array(result);
	return (result);
}

/**
 * add - add two numbers stored in arrays.
 * @n1: the first number
 * @n2: the second number
 *
 * Return: pointer to result, NULL on failure
 */
u4b_array *add(u4b_array *n1, u4b_array *n2)
{
	size_t n1_i = 0, n2_i = 0, sum_i = 0, result_len = 0;
	int64_t byt_sum = 0;
	u4b_array *sum = NULL;

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
