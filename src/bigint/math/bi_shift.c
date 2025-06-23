/*!
 * @file
 * @brief methods for shifting "digits" of bigints.
 */

#include <string.h>  // memmove

#include "_bi_internals.h"
#include "bigint.h"

static bigint *_bi_shift(bigint *const restrict n, const intmax_t d) _nonnull;
static bigint *_bi_ishift(bigint *const restrict n, const intmax_t d) _nonnull;

/*!
 * @brief shift "digits" of a `bigint`.
 * @private @memberof bigint
 *
 * @param[in] n pointer to the bigint.
 * @param[in] d how much to shift by: if negative, shift to the left;
 * else, shift to the right;
 *
 * @return pointer to the shifted `bigint`, NULL on failure.
 */
static bigint *_bi_shift(bigint *const restrict n, const intmax_t d)
{
	if (n->len < 0)
		return (NULL);

	if (bi_isNaN(_bi_trim(n)))
		return (_bi_alloc(0));

	if (bi_iszero(n))
		return (_bi_alloc(1));

	bigint *restrict res = NULL;

	if (d < 0)  // left shift
	{
		const intmax_t digits = -d;
		const uintmax_t final_len = (uintmax_t)n->len + digits;

		res = _bi_alloc(final_len);
		if (res)
		{
			memset(res->num, 0, sizeof(*n->num) * digits);
			memcpy(&(res->num[digits]), n->num, sizeof(*n->num) * n->len);
		}
	}
	else  // right shift
	{
		const uintmax_t final_len = d < n->len ? (uintmax_t)n->len - d : 1;

		res = _bi_alloc(final_len);
		if (d >= n->len)  // result == 0
			return (res);

		if (res)
			memcpy(res->num, &(n->num[d]), sizeof(*n->num) * final_len);
	}

	if (res)
		res->is_negative = n->is_negative;

	return (res);
}

/*!
 * @brief right shift a `bigint` i.e: `n / BIGINT_BASE^d`.
 * @public @memberof bigint
 *
 * @param[in] n pointer to the bigint.
 * @param[in] d how much to shift by.
 *
 * @return pointer to the shifted `bigint`, NULL on failure.
 */
bigint *bi_shift_r(bigint *const restrict n, const intmax_t d)
{
	if (!n || n->len < 0 || d < 0)
		return (NULL);

	return (_bi_shift(n, d));
}

/*!
 * @brief left shift a `bigint` i.e: `n * BIGINT_BASE^d`.
 * @public @memberof bigint
 *
 * @param[in] n pointer to the bigint.
 * @param[in] d how much to shift by.
 *
 * @return pointer to the shifted `bigint`, NULL on failure.
 */
bigint *bi_shift_l(bigint *const restrict n, const intmax_t d)
{
	if (!n || n->len < 0 || d < 0)
		return (NULL);

	return (_bi_shift(n, -d));
}

/*!
 * @brief shift digits of a `bigint` inplace.
 * @private @memberof bigint
 *
 * `n` must be large enough to hold the final result, as there will be
 * no memory allocations via *alloc family functions.
 *
 * @param[in out] n pointer to the bigint.
 * @param[in] d how much to shift by: if negative shift to the left,
 * if positive shift to the right.
 *
 * @return pointer to the bigint on success, NULL on failure.
 */
static bigint *_bi_ishift(bigint *const restrict n, const intmax_t d)
{
	if (n->len < 0)
		return (NULL);

	if (bi_isNaN(_bi_trim(n)))
		return (NULL);

	if (d == 0 || bi_iszero(n))
		return (n);

	uintmax_t final_len = 0;

	if (d < 0)  // left shift
	{
		const intmax_t digits = -d;

		final_len = (uintmax_t)n->len + digits;
		memmove(&(n->num[digits]), n->num, sizeof(*n->num) * n->len);
		memset(n->num, 0, sizeof(*n->num) * digits);
	}
	else  // right shift
	{

		final_len = n->len > d ? (uintmax_t)n->len - d : 1;
		if (n->len > d)
			memmove(n->num, &(n->num[d]), sizeof(*n->num) * final_len);
		else
			n->num[final_len - 1] = 0;

		n->num[final_len] = 0;
	}

	n->len = final_len;
	return (n);
}

/*!
 * @brief shift a `bigint` to the right inplace, i.e: `n / BIGINT_BASE^d`.
 * @public @memberof bigint
 *
 * No memory allocations via *alloc family functions will occur, therefore
 * `n` must be large enough to hold the final result.
 *
 * @param[in out] n pointer to the bigint.
 * @param[in] d how much to shift by.
 *
 * @return pointer to the `bigint` on success, NULL on failure.
 */
bigint *bi_ishift_r(bigint *const restrict n, const intmax_t d)
{
	if (!n || n->len < 0 || d < 0)
		return (NULL);

	return (_bi_ishift(n, d));
}

/*!
 * @brief shift a `bigint` to the left inplace, i.e: `n * BIGINT_BASE^d`.
 * @public @memberof bigint
 *
 * No memory allocations via *alloc family functions will occur, therefore
 * `n` must be large enough to hold the final result.
 *
 * @param[in out] n pointer to the bigint.
 * @param[in] d how much to shift by.
 *
 * @return pointer to the `bigint` on success, NULL on failure.
 */
bigint *bi_ishift_l(bigint *const restrict n, const intmax_t d)
{
	if (!n || n->len < 0 || d < 0)
		return (NULL);

	return (_bi_ishift(n, -d));
}
