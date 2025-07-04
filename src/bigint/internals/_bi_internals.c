#include <string.h>  // memmove, memset, memcpy

#include "_bi_internals.h"
#include "attribute_macros.h"

/*!
 * @brief allocate memory for a `bigint` of given length.
 * @protected @memberof bigint
 *
 * The `bigint` struct and its array of "digits" will be stored in one
 * continuos memory block.
 * Only the least significant digit of the returned `bigint` will be
 * initialised to 0.
 *
 * @param[in] len length of the array. A length of 0 returns a `bigint`
 * pointer with a NULLed "digit" array.
 *
 * @return a pointer to a `bigint` struct, NULL on failure.
 */
bigint *_bi_alloc(const len_ty len)
{
	if (len < 0)
		return (NULL);

	bigint *restrict bi = NULL;
	const size_t alignof_digit_ty = _alignof(digit_ty);
	const size_t arr_size = sizeof(*bi->num) * len;

	// overflow error.
	if (len > 0 && arr_size / len != sizeof(*bi->num))
		return (NULL);

	bi = xmalloc(sizeof(*bi) + arr_size + alignof_digit_ty);
	if (!bi)
		return (NULL);

	*bi = (bigint){0};
	bi->len = len;
	if (len > 0)
	{
		char *const mem = (char *)(bi + 1);
		bi->num = (digit_ty *)(mem + alignof_digit_ty -
							   ((size_t)mem % alignof_digit_ty));
		bi->num[0] = 0;
	}

	return (bi);
}

/*!
 * @brief resizes the capacity of a `bigint`'s internal array.
 * @protected @memberof bigint
 *
 * All added memory will be initialised to 0.
 *
 * If `bi` is NULL, a pointer to a new `bigint` of size `len` is returned.
 * Otherwise, resize the `bi` to `len` "digits".
 * If `len` is 0, the `bigint` will have its "digit" array freed and NULLed.
 *
 * If any of the resizing operations fail `bi` will be freed and NULL returned.
 *
 * @param[in] bi pointer to the `bigint`.
 * @param[in] len length to resize the array to.
 *
 * @return pointer to the resized `bigint`, NULL on failure.
 */
bigint *_bi_resize(bigint *bi, const len_ty len)
{
	if (len < 0 || (bi && bi->len < 0))
		return (_bi_free(bi));

	const size_t alignof_digit_ty = _alignof(digit_ty);
	const size_t arr_size = sizeof(*bi->num) * len;

	// overflow error.
	if (len > 0 && arr_size / len != sizeof(*bi->num))
		return (_bi_free(bi));

	if (!bi)
	{
		bi = _bi_alloc(len);
		if (bi && bi->num)
			memset(bi->num, 0, sizeof(*bi->num) * len);

		return (bi);
	}

	bi = xrealloc_free_on_fail(bi, sizeof(*bi) + arr_size + alignof_digit_ty);
	if (!bi)
		return (NULL);

	if (len < 1)
		bi->num = NULL;
	else
	{
		char *const mem = (char *)(bi + 1);
		bi->num = (digit_ty *)(mem + alignof_digit_ty -
							   ((size_t)mem % alignof_digit_ty));

		if (len > bi->len)
			memset(&(bi->num[bi->len]), 0, sizeof(*bi->num) * (len - bi->len));
	}

	bi->len = len;
	return (bi);
}

/*!
 * @brief frees memory of a `bigint`.
 * @protected @memberof bigint
 *
 * @param[in] freeable_bigint pointer to the `bigint` to free.
 *
 * @return NULL always.
 */
void *_bi_free(bigint *const freeable_bigint)
{
	if (freeable_bigint)
	{
		freeable_bigint->len = 0;
		/* freeable_bigint->num = xfree(freeable_bigint->num); */
		freeable_bigint->num = NULL;
	}

	return (xfree(freeable_bigint));
}

/*!
 * @brief truncate length of a `bigint` to ignore trailing zeros.
 * @protected @memberof bigint
 *
 * If the value of the `bigint` is 0 or the length is 0,
 * the is_negative flag will be set to false.
 *
 * @param[in] n pointer to a `bigint`.
 *
 * @return pointer to the truncated `bigint`.
 */
bigint *_bi_trim(bigint *const restrict n)
{
	if (!n)
		return (NULL);

	if (!n->num)
		n->len = 0;

	while (n->len > 1 && n->num[n->len - 1] == 0)
		--n->len;

	if (!n->num || (n->len == 1 && n->num[0] == 0))
		n->is_negative = false;

	return (n);
}

/*!
 * @brief duplicate a `bigint` as is.
 * @protected @memberof bigint
 *
 * @param[in] bi pointer to the `bigint` to duplicate.
 *
 * @return a pointer to duplicated bi, NULL on failure.
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

/*!
 * @brief copy contents of a `bigint` into another `bigint`.
 * @protected @memberof bigint
 *
 * All pointers in dest should reference memory areas large enough to hold
 * the corresponding data in src.
 * The move will fail if any pointers are NULL or the length of `src` is less
 * than 1.
 *
 * @param[out] dest where to copy to.
 * @param[in] src the `bigint` to be copied.
 *
 * @return pointer to the destination, NULL on failure.
 */
bigint *_bi_move(bigint *const dest, bigint const *const src)
{
	if ((!src || src->len < 1 || !src->num) || (!dest || !dest->num))
		return (NULL);

	dest->len = src->len;
	dest->is_negative = src->is_negative;
	if (src->num && dest->num)
		memmove(dest->num, src->num, sizeof(*src->num) * src->len);

	return (dest);
}
