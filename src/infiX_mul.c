#include "infiX.h"

static mid_uint *multiply_negatives(mid_uint *n1_arr, mid_uint *n2_arr);

/**
 * infiX_multiplication - multiplies numbers stored in arrays.
 * @n1_arr: the first integer array (base 10)
 * @n2_arr: the second integer array (base 10)
 *
 * Return: pointer to result, NULL on failure
 */
mid_uint *infiX_multiplication(mid_uint *n1_arr, mid_uint *n2_arr)
{
	hi_uint byt_mul = 0;
	ssize_t top = -1, botm = -1, c_mulsz = 0, t = 1, b = 1;
	mid_uint *c_mul = NULL, *prod = NULL, *total = NULL;

	prod = multiply_negatives(n1_arr, n2_arr);
	if (prod || errno)
		return (prod);

	if (n1_arr)
		top = n1_arr[0];

	if (n2_arr)
		botm = n2_arr[0];

	for (b = 1; b <= botm; b++)
	{
		/*Skip multiplication by zero*/
		if (n2_arr[b] == 0)
			continue;

		/*Size of c_mul = Array size of top + Current index of botm (b)*/
		c_mulsz = top + b;
		/*Memalloc c_mul (+1 for storing size of the array)*/
		c_mul = calloc((c_mulsz + 1), sizeof(*c_mul));
		if (!c_mul)
		{
			perror("Malloc fail");
			return (NULL);
		}

		c_mul[0] = c_mulsz;
		byt_mul = 0;
		for (t = 1; t <= top; t++)
		{
			byt_mul += (hi_uint)n1_arr[t] * (hi_uint)n2_arr[b];
			/*Offset to current index of botm*/
			c_mul[(t - 1) + b] = byt_mul % MID_MAX_VAL;
			byt_mul /= MID_MAX_VAL;
		}

		c_mul[(t - 1) + b] = byt_mul;
		prod = infiX_addition(total, c_mul);
		free(c_mul);
		free(total);
		if (!prod)
			return (NULL);

		total = prod;
	}

	trim_intarr(prod);
	return (prod);
}

/**
 * nultiply_negatives - nultiplication of signed numbers (-ve numbers) or zero
 * @n1_arr: number to be nultiplied
 * @n2_arr: number to nultiply
 *
 * Return: pointer to the result, NULL on failure
 */
mid_uint *multiply_negatives(mid_uint *n1_arr, mid_uint *n2_arr)
{
	mid_uint *result = NULL, a_msd = 0, b_msd = 0;

	trim_intarr(n1_arr);
	trim_intarr(n2_arr);
	if (n1_arr)
		a_msd = n1_arr[n1_arr[0]];

	if (n2_arr)
		b_msd = n2_arr[n2_arr[0]];

	if ((a_msd & NEGBIT_MIDUINT) && (b_msd & NEGBIT_MIDUINT))
	{ /* -8 * -7 = 8*7 */
		n1_arr[n1_arr[0]] ^= NEGBIT_MIDUINT;
		n2_arr[n2_arr[0]] ^= NEGBIT_MIDUINT;
		result = infiX_multiplication(n1_arr, n2_arr);
	}
	else if (a_msd & NEGBIT_MIDUINT)
	{ /* -8 * 7 = -(8*7) */
		n1_arr[n1_arr[0]] ^= NEGBIT_MIDUINT;
		result = infiX_multiplication(n1_arr, n2_arr);
		result[result[0]] |= NEGBIT_MIDUINT;
	}
	else if (b_msd & NEGBIT_MIDUINT)
	{ /* 8 * -7 = -(8*7) */
		n2_arr[n2_arr[0]] ^= NEGBIT_MIDUINT;
		result = infiX_multiplication(n1_arr, n2_arr);
		result[result[0]] |= NEGBIT_MIDUINT;
	}
	else if (!n1_arr || !n2_arr || (n1_arr[0] == 1 && a_msd == 0) ||
			 (n2_arr[0] == 1 && b_msd == 0))
	{ /*Multiplication by zero or NULL*/
		result = calloc(2, sizeof(*result));
		if (!result)
			perror("Malloc fail");
	}

	return (result);
}
