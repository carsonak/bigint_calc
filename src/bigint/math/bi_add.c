/*!
 * @file
 * @brief bigint addition methods.
 */

#include <string.h>  // memcpy

#include "_bi_internals.h"
#include "bigint.h"

static bigint *
iadd(bigint *const restrict n1, bigint const *const restrict n2) ATTR_NONNULL;
static bigint *iadd_negatives(
	bigint *const restrict n1, bigint *const restrict n2
) ATTR_NONNULL;

/*!
 * @brief add two `bigint`s inplace.
 * @private @memberof bigint
 *
 * @invariant
 * Numbers to be added are never negative.
 *
 * @invariant
 * `n1` is always large enough to hold the results of the addition.
 *
 * @param[in out] n1 the first number.
 * @param[in] n2 the second number.
 *
 * @return pointer to `n1`.
 */
static bigint *iadd(bigint *const restrict n1, bigint const *const restrict n2)
{
	len_ty n_i = 0;

	if (n2->len > n1->len)
		n1->len = n2->len;

	// Addition without checking for carry.
	for (; n_i < n2->len; n_i++)
		n1->num[n_i] += n2->num[n_i];

	n_i = 0;
	// Handling the pending carries.
	for (unsigned int carry = 0; n_i < n1->len || carry > 0; n_i++)
	{
		n1->num[n_i] += carry;
		carry = n1->num[n_i] >= BIGINT_BASE;
		n1->num[n_i] %= BIGINT_BASE;
	}

	n1->len = n_i;
	return (n1);
}

/*!
 * @brief handle inplace addition of two signed `bigint`s.
 * @private @memberof bigint
 *
 * @param[in out] n1 first number.
 * @param[in] n2 second number.
 *
 * @return pointer to `n1`.
 */
static bigint *
iadd_negatives(bigint *const restrict n1, bigint *const restrict n2)
{
	const bool neg1 = n1->is_negative, neg2 = n2->is_negative;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2)  // -8 + -7 = -(8+7)
	{
		iadd(n1, n2);
		n1->is_negative = true;
	}
	else if (neg1)  // -8 + 7 = -(8-7)
	{
		bi_isubtract(n1, n2);  // inplace subtraction should not fail.
		n1->is_negative = !n1->is_negative;
	}
	else if (neg2)             // 8 + -7 = 8-7
		bi_isubtract(n1, n2);  // inplace subtraction should not fail.

	n2->is_negative = neg2;
	return (n1);
}

/*!
 * @brief add two `bigint`s and store results in first number.
 * @public @memberof bigint
 *
 * The results of the addittion will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * @param[in out] n1 the first number, should have enough space to store the result.
 * @param[in] n2 the second number.
 *
 * @return pointer to `n1` on success, NULL on failure.
 */
bigint *bi_iadd(bigint *const restrict n1, bigint *const restrict n2)
{
	/* n1.num should not be NULL as this function does not */
	/* allocate any memory. */
	if (!n1 || !n1->num || n1->len < 0)
		return (NULL);

	_bi_trim(n1);
	if (!n2)  // This case is treated as: n1 = +n1.
		return (n1);

	if (bi_isNaN(n1) || bi_isNaN(_bi_trim(n2)))
		return (NULL);

	if (n1->is_negative || n2->is_negative)
		iadd_negatives(n1, n2);
	else
		iadd(n1, n2);

	return (_bi_trim(n1));
}

/*!
 * @brief add an int to a `bigint` inplace.
 * @public @memberof bigint
 *
 * The results of the addittion will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * @param[in out] n1 the first number, must have enough memory allocated to hold the answer.
 * @param[in] n2 the second number.
 *
 * @return pointer to `n1` on success, NULL on failure.
 */
bigint *bi_iadd_int(bigint *const restrict n1, const intmax_t n2)
{
	if (!n1 || n1->len < 0)
		return (false);

	bigint num2 = {.len = 4, .is_negative = 0, .num = (digit_ty[6]){0}};

	return (bi_iadd(n1, int_to_bi(&num2, n2)));
}

/*!
 * @brief handle addition of two bigints.
 * @public @memberof bigint
 *
 * @param[in] n1 the first number.
 * @param[in] n2 the second number.
 *
 * @return pointer to the result on success, NULL on failure.
 */
bigint *bi_add(bigint *const restrict n1, bigint *const restrict n2)
{
	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (NULL);

	if (bi_isNaN(_bi_trim(n1)) || bi_isNaN(_bi_trim(n2)))
		return (_bi_alloc(0));

	/* Allocating for max possible size rather than a perfect fit. */
	const len_ty result_len = ((n1->len > n2->len) ? n1->len : n2->len) + 1;
	bigint *const restrict sum = _bi_alloc(result_len);

	if (!sum)
		return (NULL);

	memset(&(sum->num[n1->len]), 0, sizeof(*n1->num) * (sum->len - n1->len));
	_bi_move(sum, n1);
	if (n1->is_negative || n2->is_negative)
		iadd_negatives(sum, n2);
	else
		iadd(sum, n2);

	return (_bi_trim(sum));
}

/*!
 * @brief add an int to a `bigint`.
 * @public @memberof bigint
 *
 * @param n1 the first number.
 * @param n2 the second number.
 *
 * @return pointer to the result on success, NULL on failure.
 */
bigint *bi_add_int(bigint *const restrict n1, const intmax_t n2)
{
	if (!n1 || n1->len < 0)
		return (NULL);

	bigint num2 = {.len = 4, .is_negative = 0, .num = (digit_ty[6]){0}};

	return (bi_add(n1, int_to_bi(&num2, n2)));
}
