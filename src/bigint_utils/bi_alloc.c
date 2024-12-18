#include "bigint_utils.h"

/**
 * bni_alloc - allocate memory for a bigint of given length.
 * @len: length of the array, length 0 returns the struct with a NULL array.
 *
 * Return: a pointer to a bigint struct, NULL on failure.
 */
bigint *bi_alloc(size_t len)
{
	bigint *arr = xcalloc(1, sizeof(*arr));

	if (!arr)
		return (NULL);

	arr->len = len;
	if (len > 0)
	{
		arr->num = xmalloc(len * sizeof(*arr->num));
		if (!arr->num)
			return (free_n_null(arr));

		arr->num[0] = 0;
		arr->num[len - 1] = 0;
	}

	return (arr);
}

/**
 * bi_realloc - resizes memory of a bigint array.
 * @bn: pointer to the bigint.
 * @len: size in bytes to resize to.
 *
 * Return: true on success, false on failure.
 */
bool bi_realloc(bigint *bn, size_t len)
{
	uint *new_arr = NULL;

	if (!bn) /*Cannot resize a NULL pointer.*/
		return (false);

	new_arr = xrealloc(bn->num, sizeof(*bn->num) * len);
	if (len && !new_arr)
		return (false);

	bn->num = new_arr;
	/*Initialise memory to 0 only when expanding the bigint.*/
	if (len > bn->len)
		memset(&(bn->num[bn->len]), 0, sizeof(*bn->num) * (len - bn->len));

	bn->len = len;
	return (true);
}

/**
 * bi_dup - duplicate a bigint.
 * @bn: a pointer to a bigint.
 *
 * Return: a pointer to a copy of bn.
 */
bigint *bi_dup(bigint *bn)
{
	bigint *dup = NULL;

	if (!bn)
		return (NULL);

	dup = bi_alloc(bn->len);
	if (!bn->len || !dup)
		return (dup);

	dup->is_negative = bn->is_negative;
	memcpy(dup->num, bn->num, sizeof(*bn->num) * bn->len);
	return (dup);
}

/**
 * bni_free - free a bigint, return NULL.
 * @freeable_ptr: a pointer to a bigint.
 *
 * Return: NULL always.
 */
void *bi_free(bigint *freeable_ptr)
{
	if (freeable_ptr)
	{
		freeable_ptr->len = 0;
		freeable_ptr->num = free_n_null(freeable_ptr->num);
	}

	return (free_n_null(freeable_ptr));
}
