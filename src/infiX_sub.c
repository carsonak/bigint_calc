#include "infiX.h"

/**
 * infiX_sub - subtract large numbers stored in arrays
 * @n1_arr: number to be subtracted
 * @n2_arr: number to subtract
 *
 * Return: pointer to the result, NULL on failure
 */
mid_uint *infiX_sub(mid_uint *n1_arr, mid_uint *n2_arr)
{
	ssize_t a_sz = -1, b_sz = -1, res_sz = -1, n1_i = 1, n2_i = 1, k = 1;
	int64_t byt_res = 0;
	mid_uint *res = NULL;

	/*Get and adjust size of the arrays. Stored at index 0.*/
	if (n1_arr)
	{
		trim_intarr(n1_arr);
		a_sz = n1_arr[0];
	}

	if (n2_arr)
	{
		trim_intarr(n2_arr);
		b_sz = n2_arr[0];
	}

	/*Calculating and adjusting size of the result*/
	/*res_sz = size of the larger number*/
	res_sz = (a_sz > b_sz) ? a_sz : b_sz;
	/*If both are the same size: res_sz = length of longest unmatching digits*/
	if (a_sz > 0 && b_sz > 0 && a_sz == b_sz)
		for (; n1_arr[res_sz] && n2_arr[res_sz] && res_sz > 0; res_sz--)
			if (n1_arr[res_sz] != n2_arr[res_sz])
				break;

	/*Subtracting NULLs or matching numbers*/
	if (res_sz < 1)
	{
		res = calloc(3, sizeof(*res));
		if (!res)
			perror("Malloc fail");
		else
			res[0] = 1;

		return (res);
	}

	res = calloc((res_sz + 2), sizeof(*res));
	if (!res)
	{
		perror("Malloc fail");
		return (NULL);
	}

	res[0] = res_sz;
	while ((n1_i <= a_sz || n2_i <= b_sz) && k <= res_sz)
	{
		/*If n1 is greater than n2*/
		if (a_sz > b_sz || (a_sz == b_sz && n1_arr[res_sz] > n2_arr[res_sz]))
		{
			if (n2_i <= b_sz) /*Subtract: n1 - n2*/
				byt_res += (int64_t)n1_arr[n1_i] - (int64_t)n2_arr[n2_i];
			else
				byt_res += (int64_t)n1_arr[n1_i];
		}
		else
		{
			if (n1_i <= a_sz) /*Subtract: n2 - n1*/
				byt_res += (int64_t)n2_arr[n2_i] - (int64_t)n1_arr[n1_i];
			else
				byt_res += (int64_t)n2_arr[n2_i];
		}

		if (byt_res < 0 && k < res_sz)
		{ /*If subtraction results in a -ve number borrow from the next block*/
			byt_res += MID_MAX_VAL;
			res[k] = byt_res % MID_MAX_VAL;
			byt_res = -1;
		}
		else
		{
			res[k] = byt_res % MID_MAX_VAL;
			byt_res = 0;
		}

		++n1_i;
		++n2_i;
		++k;
	}

	/*Setting the -ve indicating bit if n2 is greater than n1*/
	if (a_sz < b_sz || (a_sz == b_sz && n1_arr[res_sz] < n2_arr[res_sz]))
		res[res_sz] |= MID_NEGBIT;

	trim_intarr(res);
	return (res);
}
