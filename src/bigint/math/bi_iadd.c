/*!
 * @file
 * @brief methods for adding bigint types inplace.
 */

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
 * @param[in out] n1 the first number, should be large enough to store the results.
 * @param[in] n2 the second number.
 *
 * @return pointer to `n1`.
 */
static bigint *iadd(bigint *const restrict n1, bigint const *const restrict n2)
{
	len_type n1_i = 0, n2_i = 0, res_len = 0;
	l_int byt_sum = 0;

	while (n1_i < n1->len || n2_i < n2->len || byt_sum > 0)
	{
		if (n1_i < n1->len)
			byt_sum += n1->num[n1_i];

		if (n2_i < n2->len)
		{
			byt_sum += n2->num[n2_i];
			++n2_i;
		}

		n1->num[n1_i] = byt_sum % BIGINT_BASE;
		byt_sum /= BIGINT_BASE;
		++n1_i;
		++res_len;
	}

	n1->len = res_len;
	return (n1);
}

/*!
 * @brief handle inplace addition of two signed `bigint`s.
 * @private @memberof bigint
 *
 * @param[in out] n1 first number.
 * @param[in] n2 second number.
 *
 * @return pointer to `n1` on success, NULL on failure.
 */
static bigint *
iadd_negatives(bigint *const restrict n1, bigint *const restrict n2)
{
	bool neg1 = n1->is_negative, neg2 = n2->is_negative;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2) /* -8 + -7 = -(8+7) */
	{
		iadd(n1, n2);
		n1->is_negative = !n1->is_negative;
		n2->is_negative = true;
	}
	else if (neg1) /* -8 + 7 = -(8-7) */
	{
		if (!bi_isubtract(n1, n2))
			return (NULL);

		n1->is_negative = !n1->is_negative;
	}
	else if (neg2) /* 8 + -7 = 8-7 */
	{
		if (!bi_isubtract(n1, n2))
			return (NULL);

		n2->is_negative = true;
	}

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
 * @return 1 on success, 0 on failure.
 */
bigint *bi_iadd(bigint *const restrict n1, bigint *const restrict n2)
{
	/* n1.num should not be NULL as this function does not */
	/* allocate any memory. */
	if (!n1 || !n1->num || n1->len < 0)
		return (NULL);

	_bi_trim(n1);
	if (!n2) /* This case is treated as: n1 = +n1. */
		return (n1);

	if (bi_isNaN(n1) || bi_isNaN(_bi_trim(n2)))
		return (NULL);

	if (n1->is_negative || n2->is_negative)
	{
		if (!iadd_negatives(n1, n2))
			return (NULL);
	}
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
	bigint num2 = {.len = 4, .is_negative = 0, .num = (u_int[3]){0}};

	if (!n1 || n1->len < 0)
		return (false);

	return (bi_iadd(n1, int_to_bi(&num2, n2)));
}
