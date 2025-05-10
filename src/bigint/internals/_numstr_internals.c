#include "_numstr_internals.h"

/**
 * _numstr_alloc - allocate memory for a numstr of given length.
 * @len: length of the array, length 0 returns the struct with a NULL array.
 *
 * Return: a pointer to a numstr struct, NULL on failure.
 */
numstr *_numstr_alloc(len_type len)
{
	if (len < 0)
		return (NULL);

	numstr *arr = xcalloc(1, sizeof(*arr));

	if (!arr)
		return (NULL);

	arr->len = len;
	if (len > 0)
	{
		arr->str = xmalloc(len * sizeof(*arr->str));
		if (!arr->str)
			arr = xfree(arr);
	}

	return (arr);
}

/**
 * _numstr_free - free a numstr, return NULL.
 * @freeable_ptr: a pointer to a numstr.
 *
 * Return: NULL always.
 */
void *_numstr_free(numstr *freeable_ptr)
{
	freeable_ptr->len = 0;
	freeable_ptr->str = xfree(freeable_ptr->str);

	return (xfree(freeable_ptr));
}
