#include "_bi_internals.h"
#include "bigint.h"

static bigint *
_bi_shift(bigint *const restrict n, const len_type d) ATTR_NONNULL;
static bigint *
_bi_ishift(bigint *const restrict n, const len_type d) ATTR_NONNULL;

/**
 * _bi_shift - shift digits of a `bigint`.
 * @n: pointer to the bigint.
 * @d: how much to shift by: if negative shift to the left,
 * if positive shift to the right.
 *
 * Return: pointer to the shifted `bigint`, NULL on failure.
 */
static bigint *_bi_shift(bigint *const restrict n, const len_type d)
{
	if (n->len < 0)
		return (NULL);

	_bi_trim(n);
	if (bi_isNaN(n))
		return (_bi_alloc(0));

	if (bi_iszero(n))
		return (_bi_alloc(1));

	bigint *restrict res = NULL;

	if (d < 0) /* left shift */
	{
		const len_type digits = -d;
		const uintmax_t final_len = (uintmax_t)n->len + digits;

		res = _bi_alloc(final_len);
		if (res)
		{
			memset(res->num, 0, sizeof(*n->num) * digits);
			memcpy(&(res->num[digits]), n->num, sizeof(*n->num) * n->len);
		}
	}
	else /* right shift */
	{
		const uintmax_t final_len = n->len > d ? (uintmax_t)n->len - d : 1;

		res = _bi_alloc(final_len);
		if (d >= n->len)
			return (res);

		if (res)
			memcpy(res->num, &(n->num[d]), sizeof(*n->num) * final_len);
	}

	return (res);
}

/**
 * bi_shift_r - right shift a `bigint` i.e: n / BIGINT_BASE^d.
 * @n: pointer to the bigint.
 * @d: how much to shift by.
 *
 * Return: pointer to the shifted `bigint`, NULL on failure.
 */
bigint *bi_shift_r(bigint *const restrict n, const len_type d)
{
	if (!n || n->len < 0 || d < 0)
		return (NULL);

	return (_bi_shift(n, d));
}

/**
 * bi_shift_l - left shift a `bigint` i.e: n * BIGINT_BASE^d.
 * @n: pointer to the bigint.
 * @d: how much to shift by.
 *
 * Return: pointer to the shifted `bigint`, NULL on failure.
 */
bigint *bi_shift_l(bigint *const restrict n, const len_type d)
{
	if (!n || n->len < 0 || d < 0)
		return (NULL);

	return (_bi_shift(n, -d));
}

/**
 * _bi_ishift - shift digits of a `bigint` inplace.
 * @n: pointer to the bigint.
 * @d: how much to shift by: if negative shift to the left,
 * if positive shift to the right.
 *
 * `n` must be large enough to hold the final result, as there will be
 * no memory allocations via *alloc family functions.
 *
 * Return: pointer to the bigint on success, NULL on failure.
 */
static bigint *_bi_ishift(bigint *const restrict n, const len_type d)
{
	if (n->len < 0)
		return (NULL);

	_bi_trim(n);
	if (bi_isNaN(n))
		return (NULL);

	if (d == 0 || bi_iszero(n))
		return (n);

	uintmax_t final_len = 0;

	if (d < 0) /* left shift */
	{
		const len_type digits = -d;

		final_len = (uintmax_t)n->len + digits;
		memmove(&(n->num[digits]), n->num, sizeof(*n->num) * n->len);
		memset(n->num, 0, sizeof(*n->num) * digits);
	}
	else /* right shift */
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

/**
 * bi_ishift_r - shift a `bigint` to the right inplace, i.e: n / BIGINT_BASE^d.
 * @n: pointer to the bigint.
 * @d: how much to shift by.
 *
 * No memory allocations via *alloc family functions will occur.
 *
 * Return: pointer to the `bigint` on success, NULL on failure.
 */
bigint *bi_ishift_r(bigint *const restrict n, const len_type d)
{
	if (!n || n->len < 0 || d < 0)
		return (NULL);

	return (_bi_ishift(n, d));
}

/**
 * bi_ishift_l - shift a `bigint` to the left inplace, i.e: n * BIGINT_BASE^d.
 * @n: pointer to the bigint.
 * @d: how much to shift by.
 *
 * No memory allocations via *alloc family functions will occur and therefore
 * `n` must be large enough to hold the final result.
 *
 * Return: pointer to the `bigint` on success, NULL on failure.
 */
bigint *bi_ishift_l(bigint *const restrict n, const len_type d)
{
	if (!n || n->len < 0 || d < 0)
		return (NULL);

	return (_bi_ishift(n, -d));
}
