#include "infix.h"

/**
 * mplug_low - plugs an int array to the beginnig of another array
 * @dest: a pointer to a mid_uint array to be modified
 * @src: a mid_uint array to add to *dest
 *
 * Description: This function will allocate memory for an array of the size
 * of both *dest and src. If any of the pointers is NULL the function will
 * assume an array of size 1, value 0. The function will copy contents of
 * src into the memory area first then contents of *dest.
 *
 * Index 0 of the arrays should have a value greater than 0 indicating the
 * size of the array.
 *
 * Return: pointer to the mem area with the result, NULL on failure
 */
mid_uint *mplug_low(mid_uint **dest, mid_uint *src)
{
	ssize_t len_d = 0, len_s = 0, d = 1, s = 1, f = 1;
	mid_uint *final = NULL;

	if (*dest)
		len_d = (*dest)[0];

	if (src)
		len_s = src[0];

	f = (len_d + len_s) ? (len_d + len_s) : 1;
	final = calloc(f + 2, sizeof(*final));
	if (final)
	{
		final[0] = f;
		for (f = 1, s = 1; s <= len_s; f++, s++)
			final[f] = src[s];

		for (d = 1; d <= len_d; f++, d++)
			final[f] = (*dest)[d];

		if (*dest)
			free(*dest);

		trim_intarr(&final);
		*dest = final;
	}
	else
		perror("Malloc Fail");

	return (final);
}

/**
 * mplug_num_low - plugs an int to the beginning an int array
 * @dest: a pointer to a mid_uint array to be modified
 * @src: a mid_uint
 *
 * Description: This function will allocate memory for an array of the size
 * of *dest + 1. If *dest is NULL the function will store the int in an array
 * of size 1. The function will copy src into index 0 first then fill the
 * rest of the memory with contents of *dest.
 *
 * Index 0 of the arrays should have a value greater than 0 indicating the
 * size of the array.
 *
 * Return: pointer to the mem area with the result, NULL on failure
 */
mid_uint *mplug_num_low(mid_uint **dest, mid_uint src)
{
	ssize_t len_d = 0, d = 1, f = 1;
	mid_uint *final = NULL;

	if (!dest)
		return (NULL);

	if (*dest)
		len_d = (*dest)[0];

	f = len_d ? (len_d + 1) : 1;
	final = calloc(f + 2, sizeof(*final));
	if (final)
	{
		final[0] = f;
		final[1] = src;
		for (f = 2, d = 1; d <= len_d; f++, d++)
			final[f] = (*dest)[d];

		trim_intarr(&final);
		if (*dest)
			free(*dest);

		*dest = final;
	}
	else
		perror("Malloc Fail");

	return (final);
}
