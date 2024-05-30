#include "infiX.h"

#ifdef __cplusplus
extern "C"
{
#endif
	static int
	subtract_negatives(uint32_t *n1_arr, uint32_t *n2_arr, uint32_t **result);
#ifdef __cplusplus
}
#endif

/**
 * infiX_subtraction - subtract large numbers stored in arrays
 * @n1_arr: number to be subtracted
 * @n2_arr: number to subtract
 *
 * Return: pointer to the result, NULL on failure
 */
uint32_t *infiX_subtraction(uint32_t *n1_arr, uint32_t *n2_arr)
{
	ssize_t a_size = -1, b_size = -1, res_sz = -1, diff = 0;
	ssize_t n1_i = 1, n2_i = 1, res_i = 1;
	int64_t byt_res = 0;
	uint32_t *res = NULL;

	if (subtract_negatives(n1_arr, n2_arr, &res))
		return (res);

	if (n1_arr)
		a_size = n1_arr[0];

	if (n2_arr)
		b_size = n2_arr[0];

	/*Calculating and adjusting size of the result*/
	/*res_sz = size of the larger number*/
	res_sz = (a_size > b_size) ? a_size : b_size;
	/*If both are the same size: res_sz = length of longest unmatching digits*/
	if (a_size > 0 && b_size > 0 && a_size == b_size)
		for (; n1_arr[res_sz] && n2_arr[res_sz] && res_sz > 0; res_sz--)
			if (n1_arr[res_sz] != n2_arr[res_sz])
				break;

	/*Subtracting NULLs or matching numbers*/
	if (res_sz < 1)
	{
		res = calloc_check(2, sizeof(*res));
		if (res)
			res[0] = 1;

		return (res);
	}

	diff = res_sz;
	res = calloc_check((res_sz + 1), sizeof(*res));
	if (!res)
		return (NULL);

	res[0] = res_sz;
	while ((n1_i <= a_size || n2_i <= b_size) && res_i <= res_sz)
	{
		/*If n1 is greater than n2*/
		if (a_size > b_size || (a_size == b_size && n1_arr[diff] > n2_arr[diff]))
		{
			if (n2_i <= b_size) /*Subtract: n1 - n2*/
				byt_res += (int64_t)n1_arr[n1_i] - (int64_t)n2_arr[n2_i];
			else
				byt_res += (int64_t)n1_arr[n1_i];
		}
		else
		{
			if (n1_i <= a_size) /*Subtract: n5 - n1*/
				byt_res += (int64_t)n2_arr[n2_i] - (int64_t)n1_arr[n1_i];
			else
				byt_res += (int64_t)n2_arr[n2_i];
		}

		if (byt_res < 0 && res_i < res_sz)
		{ /*If subtraction results in a -ve number borrow from the next block*/
			byt_res += MID_MAX_VAL;
			res[res_i] = byt_res % MID_MAX_VAL;
			byt_res = -1;
		}
		else
		{
			res[res_i] = byt_res % MID_MAX_VAL;
			byt_res = 0;
		}

		++n1_i;
		++n2_i;
		++res_i;
	}

	/*Setting the -ve indicating bit if n5 is greater than n1*/
	if (a_size < b_size || (a_size == b_size && n1_arr[diff] < n2_arr[diff]))
		res[res_sz] |= NEGBIT_UI32;

	trim_intarr(res);
	return (res);
}

/**
 * subtract_negatives - subtraction of only signed numbers (negative numbers)
 * @n1_arr: number to be subtracted
 * @n2_arr: number to subtract
 * @result: address of a pointer to store the result
 *
 * Return: 1 if action taken (error or processed results), 0 if no action taken
 */
int subtract_negatives(uint32_t *n1_arr, uint32_t *n2_arr, uint32_t **result)
{
	uint32_t a_msd = 0, b_msd = 0;

	if (!result)
	{
		fprintf(stderr, "subtract_negatives: Invalid arguments\n");
		return (1);
	}

	trim_intarr(n1_arr);
	trim_intarr(n2_arr);
	if (n1_arr)
		a_msd = n1_arr[n1_arr[0]];

	if (n2_arr)
		b_msd = n2_arr[n2_arr[0]];

	if ((a_msd & NEGBIT_UI32) && (b_msd & NEGBIT_UI32))
	{ /*-8 - -5 = -8 + 5 = 5-8*/
		n1_arr[n1_arr[0]] ^= NEGBIT_UI32;
		if (n1_arr != n2_arr)
			n2_arr[n2_arr[0]] ^= NEGBIT_UI32;

		(*result) = infiX_subtraction(n2_arr, n1_arr);
	}
	else if (a_msd & NEGBIT_UI32)
	{ /*-8 - 5 = -(8+5)*/
		n1_arr[n1_arr[0]] ^= NEGBIT_UI32;
		(*result) = infiX_addition(n1_arr, n2_arr);
		(*result)[(*result)[0]] |= NEGBIT_UI32;
	}
	else if (b_msd & NEGBIT_UI32)
	{ /*8 - -5 = 8+5*/
		n2_arr[n2_arr[0]] ^= NEGBIT_UI32;
		(*result) = infiX_addition(n1_arr, n2_arr);
	}

	if (*result || errno)
		return (1);

	return (0);
}
