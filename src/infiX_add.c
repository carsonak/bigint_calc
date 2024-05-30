#include "infiX.h"

#ifdef __cplusplus
extern "C"
{
#endif
	static int
	add_negatives(uint32_t *n1_arr, uint32_t *n2_arr, uint32_t **result);
#ifdef __cplusplus
}
#endif

/**
 * infiX_addition - adds integers stored in arrays
 * @n1_arr: the first integer array (base 10)
 * @n2_arr: the second integer array (base 10)
 *
 * Return: pointer to result, NULL on failure
 */
uint32_t *infiX_addition(uint32_t *n1_arr, uint32_t *n2_arr)
{
	ssize_t a_sz = -1, b_sz = -1, sum_sz = 0, g = 1, h = 1, k = 1;
	int64_t byt_sum = 0;
	uint32_t *sum = NULL;

	if (add_negatives(n1_arr, n2_arr, &sum))
		return (sum);

	if (n1_arr)
		a_sz = n1_arr[0];

	if (n2_arr)
		b_sz = n2_arr[0];

	/*Size of the sum array = (larger of a_sz or b_sz, +1 for a carry)*/
	sum_sz = ((a_sz > b_sz) ? a_sz : b_sz) + 1;
	if (sum_sz < 1)
	{
		sum = calloc_check(2, sizeof(*sum));
		if (sum)
			sum[0] = 1;

		return (sum);
	}

	/*Memalloc sum_array (+1 for storing size)*/
	sum = calloc_check((sum_sz + 1), sizeof(*sum));
	if (!sum)
		return (NULL);

	sum[0] = sum_sz;
	while (g <= a_sz || h <= b_sz || byt_sum > 0)
	{
		if (g <= a_sz)
		{
			byt_sum += (int64_t)n1_arr[g];
			++g;
		}

		if (h <= b_sz)
		{
			byt_sum += (int64_t)n2_arr[h];
			++h;
		}

		sum[k] = byt_sum % (MID_MAX_VAL);
		byt_sum /= (MID_MAX_VAL);
		++k;
	}

	trim_intarr(sum);
	return (sum);
}

/**
 * add_negatives - addition of only signed numbers (negative numbers)
 * @n1_arr: number to be subtracted
 * @n2_arr: number to subtract
 * @result: address of a pointer to store the result
 *
 * Return: 1 if action taken (error or processed results), 0 if no action taken
 */
int add_negatives(uint32_t *n1_arr, uint32_t *n2_arr, uint32_t **result)
{
	uint32_t a_msd = 0, b_msd = 0;

	if (!result)
	{
		fprintf(stderr, "add_negatives: Invalid arguments\n");
		return (1);
	}

	trim_intarr(n1_arr);
	trim_intarr(n2_arr);
	if (n1_arr)
		a_msd = n1_arr[n1_arr[0]];

	if (n2_arr)
		b_msd = n2_arr[n2_arr[0]];

	if ((a_msd & NEGBIT_UI32) && (b_msd & NEGBIT_UI32))
	{ /*-8 + -7 = -(8+7)*/
		n1_arr[n1_arr[0]] ^= NEGBIT_UI32;
		if (n1_arr != n2_arr)
			n2_arr[n2_arr[0]] ^= NEGBIT_UI32;

		(*result) = infiX_addition(n1_arr, n2_arr);
		(*result)[(*result)[0]] |= NEGBIT_UI32;
	}
	else if (a_msd & NEGBIT_UI32)
	{ /*-8 + 7 = 7-8*/
		n1_arr[n1_arr[0]] ^= NEGBIT_UI32;
		(*result) = infiX_subtraction(n2_arr, n1_arr);
	}
	else if (b_msd & NEGBIT_UI32)
	{ /*8 + -7 = 8-7*/
		n2_arr[n2_arr[0]] ^= NEGBIT_UI32;
		(*result) = infiX_subtraction(n1_arr, n2_arr);
	}

	if (*result || errno)
		return (1);

	return (0);
}
