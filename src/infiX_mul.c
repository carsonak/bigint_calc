#include "infiX.h"

static uint32_t *multiply_negatives(uint32_t *n1_arr, uint32_t *n2_arr);

/**
 * infiX_multiplication - multiplies numbers stored in arrays.
 * @n1_arr: the first integer array (base 10)
 * @n2_arr: the second integer array (base 10)
 *
 * Return: pointer to result, NULL on failure
 */
uint32_t *infiX_multiplication(uint32_t *n1_arr, uint32_t *n2_arr)
{
	uint64_t byt_mul = 0;
	ssize_t top = -1, botm = -1, c_mulsz = 0, t = 1, b = 1;
	uint32_t *c_mul = NULL, *prod = NULL, *total = NULL;

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
		c_mul = check_calloc((c_mulsz + 1), sizeof(*c_mul));
		if (!c_mul)
			return (NULL);

		c_mul[0] = c_mulsz;
		byt_mul = 0;
		for (t = 1; t <= top; t++)
		{
			byt_mul += (uint64_t)n1_arr[t] * (uint64_t)n2_arr[b];
			/*Offset to current index of botm*/
			c_mul[(t - 1) + b] = byt_mul % MAX_VAL_u4b;
			byt_mul /= MAX_VAL_u4b;
		}

		c_mul[(t - 1) + b] = byt_mul;
		prod = infiX_addition(total, c_mul);
		free_n_null(c_mul);
		free_n_null(total);
		if (!prod)
			return (NULL);

		total = prod;
	}

	trim_u4b_array(prod);
	return (prod);
}

/**
 * nultiply_negatives - nultiplication of signed numbers (-ve numbers) or zero
 * @n1_arr: number to be nultiplied
 * @n2_arr: number to nultiply
 *
 * Return: pointer to the result, NULL on failure
 */
uint32_t *multiply_negatives(uint32_t *n1_arr, uint32_t *n2_arr)
{
	uint32_t *result = NULL, a_msd = 0, b_msd = 0;

	trim_u4b_array(n1_arr);
	trim_u4b_array(n2_arr);
	if (n1_arr)
		a_msd = n1_arr[n1_arr[0]];

	if (n2_arr)
		b_msd = n2_arr[n2_arr[0]];

	if ((a_msd & NEGBIT_u4b) && (b_msd & NEGBIT_u4b))
	{ /* -8 * -7 = 8*7 */
		n1_arr[n1_arr[0]] ^= NEGBIT_u4b;
		if (n1_arr != n2_arr)
			n2_arr[n2_arr[0]] ^= NEGBIT_u4b;

		result = infiX_multiplication(n1_arr, n2_arr);
	}
	else if (a_msd & NEGBIT_u4b)
	{ /* -8 * 7 = -(8*7) */
		n1_arr[n1_arr[0]] ^= NEGBIT_u4b;
		result = infiX_multiplication(n1_arr, n2_arr);
		result[result[0]] |= NEGBIT_u4b;
	}
	else if (b_msd & NEGBIT_u4b)
	{ /* 8 * -7 = -(8*7) */
		n2_arr[n2_arr[0]] ^= NEGBIT_u4b;
		result = infiX_multiplication(n1_arr, n2_arr);
		result[result[0]] |= NEGBIT_u4b;
	}
	else if (!n1_arr || !n2_arr || (n1_arr[0] == 1 && a_msd == 0) ||
			 (n2_arr[0] == 1 && b_msd == 0))
		/*Multiplication by zero or NULL*/
		result = check_calloc(2, sizeof(*result));

	return (result);
}
