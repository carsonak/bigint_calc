/*!
 * @file
 * @brief methods for subtracting bigint types.
 */

#include <string.h>  // memset

#include "_bi_internals.h"
#include "bigint.h"

static bigint *isubtract(
	bigint *const restrict n1, const bigint *const restrict n2
) ATTR_NONNULL;
static bigint *isubtract_negatives(
	bigint *const restrict n1, bigint *const restrict n2
) ATTR_NONNULL;

/*!
 * @brief subtract two `bigint`s inplace.
 * @private @memberof bigint
 *
 * This function subtracts only unsigned numbers.
 *
 * @param[in out] n1 first number.
 * @param[in] n2 second number.
 *
 * @return pointer to `n1`.
 */
static bigint *
isubtract(bigint *const restrict n1, const bigint *const restrict n2)
{
	len_ty n_i = 0;
	bool const n1_is_greater = _bi_compare_const(n1, n2) > 0;

	// Subtraction without checking for borrows.
	if (n1_is_greater)  // then; n1 - n2
	{
		for (; n_i < n2->len; n_i++)
			n1->num[n_i] -= n2->num[n_i];
	}
	else  // n2 - n1
	{
		n1->is_negative = true;
		/**
		 * The result of a subtraction usually has as many or one less digits
		 * as the "longest" operand. Therefore, care should be taken to stay
		 * within the of bounds of the size of the result, `n1` in this case.
		 *
		 * For example, suppose `n1` is a 2 digit number 55 and `n2` a
		 * 5 digit number 10025. The result of the subtraction `n1 - n2`
		 * is a 4 digit number 9970. The caller may have anticipated
		 * this and only allocated memory enough for 4 digits.
		 *
		 * The conditions for when the result has one less digit than the
		 * longest operand are:
		 * 	1. The longest operand has atleast two digits.
		 * 	2. The value of its last digit is 1.
		 * 	3. A borrow from previous subtractions reduces that last digit's
		 * 	value to 0.
		 *
		 * Out of bounds access is only relevant when `n2` > `n1`.
		 */
		for (; n_i < (n2->len - (n2->len > n1->len)); n_i++)
			n1->num[n_i] = n2->num[n_i] - n1->num[n_i];
	}

	if (n_i > n1->len)
		n1->len = n_i;

	// Handle the pending borrows.
	unsigned int borrow = 0;
	for (n_i = 0; n_i < n1->len; n_i++)
	{
		n1->num[n_i] -= borrow;
		borrow = 0;
		if (n1->num[n_i] < 0)
		{
			n1->num[n_i] += BIGINT_BASE;
			borrow = 1;
		}
	}

	// Handle the special case.
	if (n_i < n2->len && (n2->num[n_i] - borrow) > 0)
	{
		n1->num[n_i] = n2->num[n_i] - borrow;
		n1->len++;
	}

	return (n1);
}

/*!
 * @brief handle subtraction of two signed `bigint`s.
 * @private @memberof bigint
 *
 * @param[in out] n1 first number.
 * @param[in] n2 second number.
 *
 * @return pointer to `n1`.
 */
static bigint *
isubtract_negatives(bigint *const restrict n1, bigint *const restrict n2)
{
	const bool neg1 = n1->is_negative, neg2 = n2->is_negative;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2)  // -8 - -5 = -(8-5)
	{
		isubtract(n1, n2);
		n1->is_negative = !n1->is_negative;
	}
	else if (neg2)        // 8 - -5 = 8+5
		bi_iadd(n1, n2);  // Inplace addition should not fail.
	else if (neg1)
	{                     // -8 - 5 = -(8+5)
		bi_iadd(n1, n2);  // Inplace addition should not fail.
		n1->is_negative = true;
	}

	n2->is_negative = neg2;
	return (n1);
}

/*!
 * @brief handle subtraction of two `bigint`s inplace.
 * @public @memberof bigint
 *
 * The results of the subtraction will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * @param[in out] n1 first number, must have enough space to store the result.
 * @param[in] n2 second number.
 *
 * @return pointer to `n1` on success, NULL on failure.
 */
bigint *bi_isubtract(bigint *const restrict n1, bigint *const restrict n2)
{
	if (!n1 || n1->len < 0)
		return (NULL);

	_bi_trim(n1);
	if (!n2)  // then n1 = -n1.
	{
		n1->is_negative = !n1->is_negative;
		return (_bi_trim(n1));
	}

	if (bi_isNaN(n1) || bi_isNaN(_bi_trim(n2)))
		return (NULL);

	if (n1->is_negative || n2->is_negative)
		isubtract_negatives(n1, n2);
	else
		isubtract(n1, n2);

	return (_bi_trim(n1));
}

/*!
 * @brief subtract an int from a `bigint` inplace.
 * @public @memberof bigint
 *
 * The results of the subtraction will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * @param[in out] n1 the first number, must have enough memory allocated to hold the answer.
 * @param[in] n2 the second number.
 *
 * @return pointer to `n1` on success, NULL on failure.
 */
bigint *bi_isubtract_int(bigint *const restrict n1, const intmax_t n2)
{
	if (!n1 || n1->len < 0)
		return (NULL);

	bigint num2 = {.len = 4, .is_negative = 0, .num = (digit_ty[6]){0}};

	return (bi_isubtract(n1, int_to_bi(&num2, n2)));
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

	/* Allocate for max possible size rather than a perfect fit. */
	const len_ty result_len = n1->len > n2->len ? n1->len : n2->len;
	bigint *const restrict diff = _bi_alloc(result_len);

	if (!diff)
		return (NULL);

	memset(&(diff->num[n1->len]), 0, sizeof(*n1->num) * (diff->len - n1->len));
	_bi_move(diff, n1);
	if (n1->is_negative || n2->is_negative)
		isubtract_negatives(diff, n2);
	else
		isubtract(diff, n2);

	return (_bi_trim(diff));
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
bigint *bi_subtract_int(bigint *const restrict n1, const intmax_t n2)
{
	bigint num2 = {.len = 4, .is_negative = 0, .num = (digit_ty[6]){0}};

	if (!n1 || n1->len < 0)
		return (NULL);

	return (bi_subtract(n1, int_to_bi(&num2, n2)));
}
