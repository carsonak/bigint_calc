#include "infix.h"

/**
 * infiX_add - adds integers stored in arrays
 * @n1_arr: the first integer array (base 10)
 * @n2_arr: the second integer array (base 10)
 *
 * Return: pointer to result, NULL on failure
 */
mid_int *infiX_add(mid_int *n1_arr, mid_int *n2_arr)
{
	ssize_t a_sz = -1, b_sz = -1, sum_sz = 0, g = 1, h = 1, k = 1;
	hi_int byt_sum = 0;
	mid_int *sum = NULL;

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
	 * Size of the sum array = (larger of a_sz or b_sz, +1 for a carry)
	 * Memalloc sum_array (+1 for storing size, +1 for end padding)
	 */
	sum_sz = ((a_sz > b_sz) ? a_sz : b_sz) + 1;
	if (sum_sz < 1)
	{
		sum = calloc(3, sizeof(*sum));
		if (sum)
			sum[0] = 1;
		else
			perror("Malloc fail");

		return (sum);
	}

	sum = calloc((sum_sz + 2), sizeof(*sum));
	if (!sum)
	{
		perror("Malloc fail");
		return (NULL);
	}

	sum[0] = sum_sz;
	/*Adding from the least significant digits going up*/
	while (g <= a_sz || h <= b_sz || byt_sum > 0)
	{
		if (g <= a_sz)
		{
			byt_sum += (hi_int)n1_arr[g];
			++g;
		}

		if (h <= b_sz)
		{
			byt_sum += (hi_int)n2_arr[h];
			++h;
		}

		sum[k] = byt_sum % (U32_ROLL);
		byt_sum /= (U32_ROLL);
		++k;
	}

	trim_intarr(&sum);
	return (sum);
}
