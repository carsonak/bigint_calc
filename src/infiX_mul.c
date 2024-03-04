#include "infix.h"

/**
 * infiX_mul - multiplies numbers stored in arrays.
 * @n1_arr: the first integer array (base 10)
 * @n2_arr: the second integer array (base 10)
 *
 * Return: pointer to result, NULL on failure
 */
mid_uint *infiX_mul(mid_uint *n1_arr, mid_uint *n2_arr)
{
	hi_uint byt_mul = 0;
	ssize_t top = -1, botm = -1, c_mulsz = 0, t = 1, b = 1;
	mid_uint *c_mul = NULL, *prod = NULL, *total = NULL;

	/*Get and adjust size of the arrays. Stored at index 0.*/
	if (n1_arr)
	{
		trim_intarr(&n1_arr);
		top = n1_arr[0];
	}

	if (n2_arr)
	{
		trim_intarr(&n2_arr);
		botm = n2_arr[0];
	}

	/*Multiplication by zero or NULL*/
	if (!n1_arr || !n2_arr || (top == 1 && n1_arr[1] == 0) || (botm == 1 && n2_arr[1] == 0))
	{
		prod = calloc(3, sizeof(*prod));
		if (!prod)
			perror("Malloc fail");
		else
			prod[0] = 1;
	}
	else
	{
		for (b = 1; b <= botm; b++)
		{
			/*Skip multiplication by zero*/
			if (n2_arr[b] == 0)
				continue;

			/*Size of c_mul = Array size of top + Current index of botm (b)*/
			/*Memalloc c_mul (+1 for storing size, +1 for end padding)*/
			c_mulsz = top + b;
			c_mul = calloc((c_mulsz + 2), sizeof(*c_mul));
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

			c_mul[(t - 1) + b] = byt_mul % MID_MAX_VAL;
			prod = infiX_add(total, c_mul);
			free(c_mul);
			if (total)
				free(total);

			if (!prod)
				return (NULL);

			total = prod;
		}
	}

	trim_intarr(&prod);
	return (prod);
}
