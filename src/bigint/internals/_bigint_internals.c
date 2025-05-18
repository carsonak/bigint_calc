#include "_bigint_internals.h"

/**
 * _bi_alloc - allocate memory for a `bigint` of given length.
 * @len: length of the array, length 0 returns the struct with a NULL array.
 *
 * Return: a pointer to a `bigint` struct, NULL on failure.
 */
bigint *_bi_alloc(const len_type len)
{
	if (len < 0)
		return (NULL);

	bigint *bn = NULL;
	const size_t size = sizeof(*bn->num) * len;

	if (len > 0 && size / len != sizeof(*bn->num))
		return (NULL);

	bn = xmalloc(sizeof(*bn) + size);
	if (!bn)
		return (NULL);

	*bn = (bigint){0};
	bn->len = len;
	if (len > 0)
	{
		bn->num = (u_int *)(bn + 1);
		bn->num[0] = 0;
		bn->num[len - 1] = 0;
	}

	return (bn);
}

/**
 * _bi_resize - resizes memory of a `bigint` array.
 * @bi: pointer to the `bigint`.
 * @len: length to resize the array to.
 *
 * If `bi` is NULL, returns pointer to a new `bigint`, otherwise
 * resize to len. If len is 0 the resized `bigint` will not be able
 * to store any numbers.
 * On failure `bi` is freed.
 *
 * Return: pointer to the resized `bigint`, NULL on failure.
 */
bigint *_bi_resize(bigint *bi, const len_type len)
{
	if (len < 0)
		return (_bi_free(bi));

	const size_t new_size = sizeof(*bi->num) * len;

	if (len > 0 && new_size / len != sizeof(*bi->num))
		return (_bi_free(bi));

	if (bi)
		bi = xrealloc_free_on_fail(bi, sizeof(*bi) + new_size);
	else
		bi = _bi_alloc(len);

	if (!bi)
		return (NULL);

	if (len > 0)
		bi->num = (u_int *)(bi + 1);
	else
		bi->num = NULL;

	/* If the `bigint` is being enlarged, set the added memory to 0. */
	if (len > bi->len)
		memset(&(bi->num[bi->len]), 0, sizeof(*bi->num) * (len - bi->len));

	bi->len = len;
	return (bi);
}

/**
 * _bi_free - frees memory of a `bigint`.
 * @freeable_ptr: pointer to the `bigint` to free.
 *
 * Return: NULL always.
 */
void *_bi_free(bigint *const freeable_ptr)
{
	if (freeable_ptr)
	{
		freeable_ptr->len = 0;
		/* freeable_ptr->num = xfree(freeable_ptr->num); */
		freeable_ptr->num = NULL;
	}

	return (xfree(freeable_ptr));
}

/**
 * _bi_trim - truncate length of a `bigint` to ignore trailing zeros.
 * @n: pointer to a `bigint` struct.
 *
 * If the value of the `bigint` is 0 or the length is 0,
 * the is_negative flag will be set to false.
 */
void _bi_trim(bigint *const n)
{
	if (!n)
		return;

	if (!n->num)
		n->len = 0;

	while (n->len > 1 && !n->num[n->len - 1])
		--n->len;

	if (!n->num || (n->len == 1 && !n->num[0]))
		n->is_negative = false;
}

/**
 * _bi_dup - duplicate a `bigint` as is.
 * @bi: pointer to the `bigint` to duplicate.
 *
 * Return: a pointer to duplicated bi, NULL on failure.
 */
bigint *_bi_dup(bigint const *const restrict bi)
{
	if (!bi)
		return (NULL);

	bigint *const restrict dup = _bi_alloc(bi->len);

	if (!dup)
		return (NULL);

	dup->is_negative = bi->is_negative;
	if (bi->len > 0)
		memcpy(dup->num, bi->num, sizeof(*bi->num) * bi->len);

	return (dup);
}

/**
 * _bi_move - copies contents of a `bigint` to another `bigint`.
 * @dest: where to copy to.
 * @src: the `bigint` to be copied.
 *
 * All pointers in dest should reference memory areas large enough to hold
 * the corresponding data in src.
 * If pointers in src are NULL, pointers in dest will not be affected.
 *
 * Return: pointer to the destination, NULL on failure.
 */
bigint *_bi_move(bigint *const dest, bigint const *const src)
{
	if (!src || !dest || (src->len > 0 && src->num && !dest->num))
		return (NULL);

	dest->len = src->len;
	dest->is_negative = src->is_negative;
	if (src->num && dest->num)
		memmove(dest->num, src->num, sizeof(*src->num) * src->len);

	return (dest);
}
