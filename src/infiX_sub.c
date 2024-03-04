#include "infix.h"

/**
 * infiX_sub - subtracts numbers in an array
 * @n1_arr: number to be subtracted
 * @n2_arr: number to subtract
 *
 * Return: pointer to the result, NULL on failure
 */
mid_uint *infiX_sub(mid_uint *n1_arr, mid_uint *n2_arr)
{
	ssize_t a_sz = -1, b_sz = -1, res_sz = -1, g = 1, h = 1, k = 1, diff = 0;
	hi_uint byt_res = 0;
	mid_uint *res = NULL;

	/*Get and adjust size of the arrays. Stored at index 0.*/
	if (n1_arr)
	{
		trim_intarr(&n1_arr);
		a_sz = n1_arr[0];
	}

	if (n2_arr)
	{
		trim_intarr(&n2_arr);
		b_sz = n2_arr[0];
	}

	/**
	 * Calculate and adjust size of the result
	 * res_sz = size of the larger number
	 * if both are the same size:
	 *		number of digits in result will be equal to the number of
	 *		digits left once the digits don't match anymore starting
	 *		from the most significant digit.
	 */
	res_sz = (a_sz > b_sz) ? a_sz : b_sz;
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

	diff = res_sz;
	res = calloc((res_sz + 2), sizeof(*res));
	if (!res)
	{
		perror("Malloc fail");
		return (NULL);
	}

	res[0] = res_sz;
	while ((g <= a_sz || h <= b_sz) && k <= res_sz)
	{
		/*If n1 is greater than n2*/
		if (a_sz > b_sz || (a_sz == b_sz && n1_arr[diff] > n2_arr[diff]))
		{
			if (h <= b_sz)
				byt_res += (hi_uint)n1_arr[g] - (hi_uint)n2_arr[h];
			else
				byt_res += (hi_uint)n1_arr[g];
		}
		else
		{
			if (g <= a_sz)
				byt_res += (hi_uint)n2_arr[h] - (hi_uint)n1_arr[g];
			else
				byt_res += (hi_uint)n2_arr[h];
		}

		/*If subtraction results in a -ve number borrow from the next digits*/
		if (byt_res < 0 && k < res_sz)
		{
			byt_res += U32_ROLL;
			res[k] = byt_res % U32_ROLL;
			byt_res = -1;
		}
		else
		{
			res[k] = byt_res % U32_ROLL;
			byt_res = 0;
		}

		++g;
		++h;
		++k;
	}

	/*Setting the -ve indicating bit if n2 is greater than n1*/
	if (a_sz < b_sz || (a_sz == b_sz && n1_arr[diff] < n2_arr[diff]))
		res[res_sz] |= U32_NEGBIT;

	trim_intarr(&res);
	return (res);
}
