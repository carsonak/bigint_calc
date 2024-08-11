#include "parsing.h"

/**
 * alloc_numstr - allocate memory for a numstr of given length.
 * @len: length of the array, length 0 returns the struct with a NULL array.
 *
 * Return: a pointer to a numstr struct, NULL on failure.
 */
numstr *alloc_numstr(size_t len)
{
	numstr *arr = xcalloc(1, sizeof(*arr));

	if (!arr)
		return (NULL);

	arr->len = len;
	if (len > 0)
	{
		arr->str = xmalloc(len * sizeof(*arr->str));
		if (!arr->str)
			arr = free_n_null(arr);
	}

	return (arr);
}

/**
 * free_numstr - free a numstr, return NULL.
 * @freeable_ptr: a pointer to a numstr.
 *
 * Return: NULL always.
 */
void *free_numstr(numstr *freeable_ptr)
{
	if (freeable_ptr)
		free_n_null(freeable_ptr->str);

	return (free_n_null(freeable_ptr));
}
