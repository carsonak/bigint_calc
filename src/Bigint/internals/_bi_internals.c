#include <string.h>  // memmove, memset, memcpy

#include "_bi_internals.h"
#include "compiler_attributes_macros.h"

/*!
 * @brief allocate memory for a `Bigint` of given length.
 * @protected @memberof Bigint
 *
 * The `Bigint` struct and its array of "digits" will be stored in one
 * continuos memory block.
 * Only the least significant digit of the returned `Bigint` will be
 * initialised to 0.
 *
 * @param[in] len length of the array. A length of 0 returns a `Bigint`
 * pointer with a NULLed "digit" array.
 *
 * @return a pointer to a `Bigint` struct, NULL on failure.
 */
Bigint *_bi_alloc(const len_ty len)
{
	if (len < 0)
		return (NULL);

	Bigint *restrict bi = NULL;
	const size_t alignof_udigit_ty = _alignof(udigit_ty);
	const size_t arr_size = sizeof(*bi->num) * len;

	// overflow error.
	if (len > 0 && arr_size / len != sizeof(*bi->num))
		return (NULL);

	bi = xmalloc(sizeof(*bi) + arr_size + alignof_udigit_ty);
	if (!bi)
		return (NULL);

	*bi = (Bigint){0};
	bi->len = len;
	if (len > 0)
	{
		char *const mem = (char *)(bi + 1);
		bi->num = (udigit_ty *)(mem + alignof_udigit_ty -
								((size_t)mem % alignof_udigit_ty));
		bi->num[0] = 0;
	}

	return (bi);
}

/*!
 * @brief resizes the capacity of a `Bigint`'s internal array.
 * @protected @memberof Bigint
 *
 * All added memory will be initialised to 0.
 *
 * If `bi` is NULL, a pointer to a new `Bigint` of size `len` is returned.
 * Otherwise, resize the `bi` to `len` "digits".
 * If `len` is 0, the `Bigint` will have its "digit" array freed and NULLed.
 *
 * If any of the resizing operations fail `bi` will be freed and NULL returned.
 *
 * @param[in] bi pointer to the `Bigint`.
 * @param[in] len length to resize the array to.
 *
 * @return pointer to the resized `Bigint`, NULL on failure.
 */
Bigint *_bi_resize(Bigint *restrict bi, const len_ty len)
{
	if (len < 0 || (bi && bi->len < 0))
		return (_bi_free(bi));

	const size_t alignof_udigit_ty = _alignof(udigit_ty);
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

	const len_ty old_len = bi->len;
	Bigint *const restrict resized =
		xrealloc(bi, sizeof(*bi) + arr_size + alignof_udigit_ty);

	if (!resized)
		return (_bi_free(bi));

	resized->len = len;
	if (len < 1)
		resized->num = NULL;
	else
	{
		char *const mem = (char *)(resized + 1);
		resized->num = (udigit_ty *)(mem + alignof_udigit_ty -
									 ((size_t)mem % alignof_udigit_ty));

		if (len > old_len)
			memset(
				&(resized->num[old_len]), 0,
				sizeof(*resized->num) * (len - old_len)
			);
	}

	return (resized);
}

/*!
 * @brief frees memory of a `Bigint`.
 * @protected @memberof Bigint
 *
 * @param[in] freeable_bigint pointer to the `Bigint` to free.
 *
 * @return NULL always.
 */
void *_bi_free(Bigint *const freeable_bigint)
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
 * @brief truncate length of a `Bigint` to ignore trailing zeros.
 * @protected @memberof Bigint
 *
 * If the value of the `Bigint` is 0 or the length is 0,
 * the is_negative flag will be set to false.
 *
 * @param[in] n pointer to a `Bigint`.
 *
 * @return pointer to the truncated `Bigint`.
 */
Bigint *_bi_trim(Bigint *const restrict n)
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
 * @brief duplicate a `Bigint` as is.
 * @protected @memberof Bigint
 *
 * @param[in] bi pointer to the `Bigint` to duplicate.
 *
 * @return a pointer to duplicated bi, NULL on failure.
 */
Bigint *_bi_dup(Bigint const *const restrict bi)
{
	if (!bi)
		return (NULL);

	Bigint *const restrict dup = _bi_alloc(bi->len);

	if (!dup)
		return (NULL);

	udigit_ty *const restrict dup_num = dup->num;
	*dup = *bi;
	dup->num = dup_num;
	if (bi->len > 0)
		memcpy(dup->num, bi->num, sizeof(*bi->num) * bi->len);

	return (dup);
}

/*!
 * @brief copy contents of a `Bigint` into another `Bigint`.
 * @protected @memberof Bigint
 *
 * All pointers in dest should reference memory areas large enough to hold
 * the corresponding data in src.
 * The move will fail if any pointers are NULL or the length of `src` is less
 * than 1.
 *
 * @param[out] dest where to copy to.
 * @param[in] src the `Bigint` to be copied.
 *
 * @return pointer to the destination, NULL on failure.
 */
Bigint *_bi_move(Bigint *const dest, Bigint const *const src)
{
	if ((!src || src->len < 1 || !src->num) || (!dest || !dest->num))
		return (NULL);

	dest->len = src->len;
	dest->is_negative = src->is_negative;
	if (src->num && dest->num)
		memmove(dest->num, src->num, sizeof(*src->num) * src->len);

	return (dest);
}
