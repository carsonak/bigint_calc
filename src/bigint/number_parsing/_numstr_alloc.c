#include "_numstr_alloc.h"

/*!
 * @brief allocate memory for a `numstr`.
 * @protected @memberof numstr
 *
 * @param[in] len length of the array,
 * length 0 returns the struct with a NULL array.
 *
 * @return a pointer to a `numstr`, NULL on failure.
 */
numstr *_numstr_alloc(len_ty len)
{
	if (len < 0)
		return (NULL);

	numstr *restrict arr = xcalloc(1, sizeof(*arr));

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

/*!
 * @brief free a `numstr`.
 * @protected @memberof numstr
 *
 * @param[in] freeable_ptr a pointer to a numstr.
 *
 * @return NULL always.
 */
void *_numstr_free(numstr *freeable_ptr)
{
	if (freeable_ptr)
	{
		freeable_ptr->len = 0;
		freeable_ptr->str = xfree(freeable_ptr->str);
	}

	return (xfree(freeable_ptr));
}
