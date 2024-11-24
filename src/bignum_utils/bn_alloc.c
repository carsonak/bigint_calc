#include "bignum_utils.h"

/**
 * bn_alloc - allocate memory for a bignum of given length.
 * @len: length of the array, length 0 returns the struct with a NULL array.
 *
 * Return: a pointer to a bignum struct, NULL on failure.
 */
bignum *bn_alloc(size_t len)
{
	bignum *arr = xcalloc(1, sizeof(*arr));

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
 * bn_realloc - resizes memory of a bignum array.
 * @bn: pointer to the bignum.
 * @len: size in bytes to resize to.
 *
 * The
 *
 * Return: true on success, false on failure.
 */
bool bn_realloc(bignum *bn, size_t len)
{
	uint *new_arr = NULL;

	if (!bn) /*Cannot resize a NULL pointer.*/
		return (false);

	new_arr = xrealloc(bn->num, sizeof(*bn->num) * len);
	if (len && !new_arr)
		return (false);

	bn->num = new_arr;
	/*Initialise memory to 0 only when expanding the bignum.*/
	if (len > bn->len)
		memset(&(bn->num[bn->len]), 0, sizeof(*bn->num) * (len - bn->len));

	bn->len = len;
	return (true);
}

/**
 * bn_dup - duplicate a bignum.
 * @bn: a pointer to a bignum.
 *
 * Return: a pointer to a copy of bn.
 */
bignum *bn_dup(bignum *bn)
{
	bignum *dup = NULL;

	if (!bn)
		return (NULL);

	dup = bn_alloc(bn->len);
	if (!bn->len || !dup)
		return (dup);

	dup->is_negative = bn->is_negative;
	memcpy(dup->num, bn->num, sizeof(*bn->num) * bn->len);
	return (dup);
}

/**
 * bn_free - free a bignum, return NULL.
 * @freeable_ptr: a pointer to a bignum.
 *
 * Return: NULL always.
 */
void *bn_free(bignum *freeable_ptr)
{
	if (freeable_ptr)
	{
		freeable_ptr->len = 0;
		freeable_ptr->num = free_n_null(freeable_ptr->num);
	}

	return (free_n_null(freeable_ptr));
}
