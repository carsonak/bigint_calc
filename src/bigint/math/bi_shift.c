#include "_bigint_internals.h"
#include "bigint.h"

/**
 * bi_shift - digit shift a `bigint`.
 * @n: pointer to the bigint.
 * @c: how much to shift by; if negative, left shift; if positive, right shift.
 *
 * Return: pointer to the shifted `bigint`, NULL on failure.
 */
static bigint *bi_shift(bigint *const restrict n, len_type c)
{
	if (!n || n->len < 0)
		return (NULL);

	_bi_trim(n);
	if (bi_isNaN(n))
		return (_bi_alloc(0));

	if (bi_iszero(n))
		return (_bi_alloc(1));

	bigint *restrict res = NULL;

	if (c < 0) /* left shift */
	{
		c = -c;
		const uintmax_t final_len = (uintmax_t)n->len + c;

		res = _bi_alloc(final_len);
		if (res)
		{
			memset(res->num, 0, sizeof(*n->num) * c);
			memcpy(&(res->num[c]), n->num, sizeof(*n->num) * n->len);
		}
	}
	else /* right shift */
	{
		const uintmax_t final_len = n->len > c ? (uintmax_t)n->len - c : 1;

		res = _bi_alloc(final_len);
		if (c >= n->len)
			return (res);

		if (res)
			memcpy(res->num, &(n->num[c]), sizeof(*n->num) * final_len);
	}

	return (res);
}

/**
 * bi_shift_r - right shift a `bigint` i.e: n / BIGINT_BASE^c.
 * @n: pointer to the bigint.
 * @c: how much to shift by.
 *
 * Return: pointer to the shifted `bigint`, NULL on failure.
 */
bigint *bi_shift_r(bigint *const restrict n, const len_type c)
{
	if (!n || n->len < 0 || c < 0)
		return (NULL);

	return (bi_shift(n, c));
}

/**
 * bi_shift_l - left shift a `bigint` i.e: n * BIGINT_BASE^c.
 * @n: pointer to the bigint.
 * @c: how much to shift by.
 *
 * Return: pointer to the shifted `bigint`, NULL on failure.
 */
bigint *bi_shift_l(bigint *const restrict n, const len_type c)
{
	if (!n || n->len < 0 || c < 0)
		return (NULL);

	return (bi_shift(n, -c));
}

/**
 * bi_ishift - digit shift a `bigint` inplace.
 * @n: pointer to the bigint.
 * @c: how much to shift by; if negative, left shift; if positive, right shift.
 *
 * No memory allocations via *alloc family functions will occur and therefore
 * `n` must be large enough to hold the final result.
 *
 * Return: true on success, false on failure.
 */
static bool bi_ishift(bigint *const restrict n, len_type c)
{
	if (!n || n->len < 0)
		return (false);

	_bi_trim(n);
	if (bi_isNaN(n))
		return (false);

	if (c == 0 || bi_iszero(n))
		return (true);

	uintmax_t final_len = 0;

	if (c < 0) /* left shift */
	{
		c = -c;
		final_len = (uintmax_t)n->len + c;
		memmove(&(n->num[c]), n->num, sizeof(*n->num) * n->len);
		memset(n->num, 0, sizeof(*n->num) * c);
	}
	else /* right shift */
	{

		final_len = n->len > c ? (uintmax_t)n->len - c : 1;
		if (n->len > c)
			memmove(n->num, &(n->num[c]), sizeof(*n->num) * final_len);
		else
			n->num[final_len - 1] = 0;

		n->num[final_len] = 0;
	}

	n->len = final_len;
	return (true);
}

/**
 * bi_ishift_r - shift a `bigint` to the right inplace, i.e: n / BIGINT_BASE^c.
 * @n: pointer to the bigint.
 * @c: how much to shift by.
 *
 * No memory allocations via *alloc family functions will occur.
 *
 * Return: true on success, false on failure.
 */
bool bi_ishift_r(bigint *const restrict n, const len_type c)
{
	if (!n || n->len < 0 || c < 0)
		return (false);

	return (bi_ishift(n, c));
}

/**
 * bi_ishift_l - shift a `bigint` to the left inplace, i.e: n * BIGINT_BASE^c.
 * @n: pointer to the bigint.
 * @c: how much to shift by.
 *
 * No memory allocations via *alloc family functions will occur and therefore
 * `n` must be large enough to hold the final result.
 *
 * Return: true on success, false on failure.
 */
bool bi_ishift_l(bigint *const restrict n, const len_type c)
{
	if (!n || n->len < 0 || c < 0)
		return (false);

	return (bi_ishift(n, -c));
}
