#include "bigint_utils.h"

/**
 * bi_alloc - allocate memory for a bigint of given length.
 * @len: length of the array, length 0 returns the struct with a NULL array.
 *
 * Return: a pointer to a bigint struct, NULL on failure.
 */
bigint *bi_alloc(const size_t len)
{
	bigint *arr = xmalloc(sizeof(*arr) + (sizeof(*arr->num) * len));

	if (!arr)
		return (NULL);

	*arr = (bigint){0};
	arr->len = len;
	if (len > 0)
	{
		arr->num = (u_int *)(arr + 1);
		arr->num[0] = 0;
		arr->num[len - 1] = 0;
	}

	return (arr);
}

/**
 * bi_resize - resizes memory of a bigint array.
 * @bi: pointer to the bigint.
 * @len: size in bytes to resize to.
 *
 * If `bi` is NULL, returns pointer to a new bigint, otherwise
 * resize to len. If len is 0 the resized bigint will not be able
 * to store any numbers.
 * On failure `bi` is left unchanged.
 *
 * Return: pointer to the resized bigint, NULL on failure.
 */
bigint *bi_resize(bigint *bi, const size_t len)
{
	const size_t actual_size = sizeof(*bi) + (sizeof(*bi->num) * len);

	bi = xrealloc(bi, actual_size);
	if (!bi)
		return (NULL);

	if (len)
		bi->num = (u_int *)(bi + 1);
	else
		bi->num = NULL;

	/*Initialise memory to 0 only when expanding the bigint.*/
	if (len > bi->len)
		memset(&(bi->num[bi->len]), 0, sizeof(*bi->num) * (len - bi->len));

	bi->len = len;
	return (bi);
}

/**
 * bi_dup - duplicate a bigint.
 * @bi: a pointer to a bigint.
 *
 * Return: a pointer to duplicated bi, NULL on failure.
 */
bigint *bi_dup(bigint const *const bi)
{
	if (!bi)
		return (NULL);

	bigint *const dup = bi_alloc(bi->len);

	if (!dup)
		return (NULL);

	dup->is_negative = bi->is_negative;
	if (bi->len)
		memcpy(dup->num, bi->num, sizeof(*bi->num) * bi->len);

	return (dup);
}

/**
 * bi_delete - free a bigint, return NULL.
 * @freeable_ptr: a pointer to a bigint.
 *
 * Return: NULL always.
 */
void *bi_delete(bigint *const freeable_ptr)
{
	if (freeable_ptr)
	{
		freeable_ptr->len = 0;
		// freeable_ptr->num = xfree(freeable_ptr->num);
		freeable_ptr->num = NULL;
	}

	return (xfree(freeable_ptr));
}
