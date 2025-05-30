/*!
 * @file
 * @brief methods for subtracting bigint types.
 */

#include <string.h> /* memset */

#include "_bi_internals.h"
#include "bigint.h"

static bigint *subtract(
	const bigint *const restrict n1, const bigint *const restrict n2
) ATTR_NONNULL;
static bigint *subtract_negatives(
	bigint *const restrict n1, bigint *const restrict n2
) ATTR_NONNULL;

/*!
 * @brief subtract two `bigint`s.
 * @private @memberof bigint
 *
 * @param[in] n1 first number.
 * @param[in] n2 second number.
 *
 * @return pointer to the result, NULL on failure.
 */
static bigint *
subtract(const bigint *const restrict n1, const bigint *const restrict n2)
{
	len_type n1_i = 0, n2_i = 0, diff_i = 0, result_len = 0;
	bool n1_is_bigger = 0;
	l_int byt_diff = 0;
	bigint *diff = NULL;

	/* result_len = max(n1->len, n2->len) */
	result_len = (n1->len > n2->len) ? n1->len : n2->len;
	/* If both arrays are of the same length then; */
	/* result_len = n1->len - */
	/* (length of longest continuos matches of m.s.ds in n1 and n2). */
	if (n1->len == n2->len)
		while (result_len > 1 &&
		       n1->num[result_len - 1] == n2->num[result_len - 1])
			result_len--;

	diff = _bi_alloc(result_len);
	if (!diff)
		return (NULL);

	n1_is_bigger = _bi_compare_const(n1, n2) > 0;
	if (n1_is_bigger == false)
		diff->is_negative = true;

	while ((n1_i < n1->len || n2_i < n2->len) && diff_i < diff->len)
	{
		if (n1_is_bigger == true) /* then; n1 - n2 */
		{
			if (n2_i < n2->len)
				byt_diff += (l_int)n1->num[n1_i] - n2->num[n2_i];
			else
				byt_diff += n1->num[n1_i];
		}
		else /* n2 - n1 */
		{
			if (n1_i < n1->len)
				byt_diff += (l_int)n2->num[n2_i] - n1->num[n1_i];
			else
				byt_diff += n2->num[n2_i];
		}

		if (byt_diff < 0) /* borrow 1 from next. */
		{
			byt_diff += BIGINT_BASE;
			diff->num[diff_i] = byt_diff % BIGINT_BASE;
			byt_diff = -1;
		}
		else
		{
			diff->num[diff_i] = byt_diff % BIGINT_BASE;
			byt_diff = 0;
		}

		++n1_i;
		++n2_i;
		++diff_i;
	}

	if (diff_i < diff->len)
		memset(
			&diff->num[diff_i], 0, sizeof(*diff->num) * (diff->len - diff_i));

	return (_bi_trim(diff));
}

/*!
 * @brief handle subtraction of two signed `bigint`s.
 * @private @memberof bigint
 *
 * @param[in] n1 first number.
 * @param[in] n2 second number.
 *
 * @return pointer to the result, NULL on failure.
 */
static bigint *
subtract_negatives(bigint *const restrict n1, bigint *const restrict n2)
{
	const bool neg1 = n1->is_negative, neg2 = n2->is_negative;
	bigint *result = NULL;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2) /* -8 - -5 = -(8-5) */
	{
		result = subtract(n1, n2);
		if (result)
			result->is_negative = !result->is_negative;
	}
	else if (neg2) /* 8 - -5 = 8+5 */
		result = bi_add(n1, n2);
	else if (neg1)
	{
		/* -8 - 5 = -(8+5) */
		result = bi_add(n1, n2);
		if (result)
			result->is_negative = !result->is_negative;
	}

	n1->is_negative = neg1;
	n2->is_negative = neg2;
	return (_bi_trim(result));
}

/*!
 * @brief handle subtraction of two `bigint`s.
 * @public @memberof bigint
 *
 * @param[in] n1 first number.
 * @param[in] n2 second number.
 *
 * @return pointer to the result, NULL on failure.
 */
bigint *bi_subtract(bigint *const restrict n1, bigint *const restrict n2)
{
	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (NULL);

	if (bi_isNaN(_bi_trim(n1)) || bi_isNaN(_bi_trim(n2)))
		return (_bi_alloc(0));

	if (n1->is_negative || n2->is_negative)
		return (subtract_negatives(n1, n2));

	return (subtract(n1, n2));
}

/*!
 * @brief subtract an int from a `bigint`.
 * @public @memberof bigint
 *
 * @param[in] n1 the first number.
 * @param[in] n2 the second number.
 *
 * @return pointer to the answer on success, NULL on failure.
 */
bigint *bi_subtract_int(bigint *const n1, const intmax_t n2)
{
	bigint num2 = {.len = 4, .is_negative = 0, .num = (u_int[4]){0}};

	if (!n1 || n1->len < 0)
		return (NULL);

	return (bi_subtract(n1, int_to_bi(&num2, n2)));
}
