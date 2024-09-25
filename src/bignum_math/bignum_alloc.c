#include "bignum_math.h"

/**
 * bignum_alloc - allocate memory for a bignum of given length.
 * @len: length of the array, length 0 returns the struct with a NULL array.
 *
 * Return: a pointer to a bignum struct, NULL on failure.
 */
bignum *bignum_alloc(size_t len)
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
 * bignum_realloc - resizes memory of a bignum array.
 * @bn: pointer to the bignum.
 * @len: size in bytes to resize to.
 *
 * Return: true on success, false on failure.
 */
bool bignum_realloc(bignum *bn, size_t len)
{
	if (!bn)
		return (false);

	bn->num = xrealloc(bn->num, sizeof(*bn->num) * len);
	if (len && !bn->num)
		return (false);

	if (len > bn->len)
		memset(&(bn->num[bn->len]), 0, sizeof(*bn->num) * (len - bn->len));

	bn->len = len;
	return (true);
}

/**
 * bignum_dup - duplicate a bignum.
 * @bn: a pointer to a bignum.
 *
 * Return: a pointer to a copy of bn.
 */
bignum *bignum_dup(bignum *bn)
{
	bignum *dup = NULL;

	if (!bn)
		return (NULL);

	dup = bignum_alloc(0);
	if (!bn->len || !dup)
		return (dup);

	dup->num = xmalloc(bn->len * sizeof(*dup->num));
	if (!dup->num)
		return (bignum_free(dup));

	dup->is_negative = bn->is_negative;
	dup->len = bn->len;
	memcpy(dup->num, bn->num, sizeof(*bn->num) * bn->len);
	return (dup);
}

/**
 * bignum_free - free a bignum, return NULL.
 * @freeable_ptr: a pointer to a bignum.
 *
 * Return: NULL always.
 */
void *bignum_free(bignum *freeable_ptr)
{
	if (freeable_ptr)
		free_n_null(freeable_ptr->num);

	return (free_n_null(freeable_ptr));
}
