/*!
 * @file
 * @brief bigint division methods.
 */

#include <stdio.h>   // fprintf
#include <string.h>  // memcpy

#include "_bi_internals.h"
#include "bigint.h"

static bool check_division_by_0(const bigint *const n2) ATTR_NONNULL;
static bool quotient_is_less_than_1(
	const bigint *const restrict n1, const bigint *const restrict n2
) ATTR_NONNULL;
static bigint *get_remainder(
	const bigint *const restrict n1, const bigint *const restrict n2,
	bigint *const restrict quotient
) ATTR_NONNULL;
static ldigit_ty get_current_quotient(
	bigint *const restrict slice, const bigint *const restrict n2,
	bigint *restrict *const restrict rem
);
static len_ty drop_next(
	bigint *const restrict slice, const bigint *const restrict rem,
	const bigint *const restrict n1, len_ty n1_i,
	const bigint *const restrict n2
) ATTR_NONNULL_POS(1, 3, 5);
static bi_divmod_res divide(
	const bigint *const restrict n1, const bigint *const restrict n2
) ATTR_NONNULL;
static bi_divmod_res divide_negatives(
	bigint *const restrict n1, bigint *const restrict n2
) ATTR_NONNULL;

/*!
 * @brief check if the denominator is zero.
 * @private @memberof bigint
 *
 * @param[in] n2 denominator.
 *
 * @return 1 if n2 is zero, else 0.
 */
static bool check_division_by_0(const bigint *const n2)
{
	if (n2->len < 1 || (n2->len == 1 && !n2->num[0]))
	{
		fprintf(stderr, "Error: division by zero.\n");
		return (true);
	}

	return (false);
}

/*!
 * @brief check if numerator < denominator.
 * @private @memberof bigint
 *
 * @param[in] n1 numerator/divisor.
 * @param[in] n2 denominator/dividend.
 *
 * @return true if numerator < denominator, false if not.
 */
static bool quotient_is_less_than_1(
	const bigint *const restrict n1, const bigint *const restrict n2
)
{
	return (_bi_compare_const(n1, n2) < 0);
}

/*!
 * @brief calculate remainder.
 * @private @memberof bigint
 *
 * ```C
 * remainder = dividend - (divisor * quotient)
 * ```
 *
 * @param[in] n1 the dividend.
 * @param[in] n2 the divisor.
 * @param[in] quotient current quotient.
 *
 * @return pointer to the remainder on success, NULL on failure.
 */
static bigint *get_remainder(
	const bigint *const restrict n1, const bigint *const restrict n2,
	bigint *const restrict quotient
)
{
	bigint num1 = *n1, num2 = *n2;
	bigint *const restrict multiple = bi_multiply(&num2, quotient);
	bigint *restrict rem = NULL;

	if (multiple)
		rem = bi_subtract(&num1, multiple);

	_bi_free(multiple);
	return (rem);
}

/*!
 * @brief calculate a single "digit" of the quotient.
 * @private @memberof bigint
 *
 * @param[in] slice the current "slice" of the bigint being divided.
 * @param[in] n2 the denominator.
 * @param[out] rem address of a `bigint` pointer to store the remainder.
 *
 * @return an int representing a "digit" of the quotient, -1 on error.
 */
static ldigit_ty get_current_quotient(
	bigint *const restrict slice, const bigint *const restrict n2,
	bigint *restrict *const restrict rem
)
{
	bigint q_estimate = {
		.len = 1, .is_negative = false, .num = (digit_ty[2]){0}
	};
	bool rem_gteq_divisor = 0;
	ldigit_ty msd_slice = 0;

	*rem = _bi_free(*rem);
	msd_slice = slice->num[slice->len - 1];
	if (slice->len > n2->len)
		msd_slice = (msd_slice * BIGINT_BASE) + slice->num[slice->len - 2];

	/* quotient ≈ most significant "digit" of slice / m.s.d of denominator. */
	q_estimate.num[0] = msd_slice / n2->num[n2->len - 1];
	*rem = get_remainder(slice, n2, &q_estimate);
	if (!(*rem))
		return (-1);

	/* 0 <= (slice - (q_estimate * denominator)) < denominator */
	rem_gteq_divisor = _bi_compare_const(*rem, n2) >= 0;
	while ((*rem)->is_negative || rem_gteq_divisor)
	{
		if ((*rem)->is_negative)
		{
			/* q_estimate was too big. */
			/* over_shoot = ceil(m.s.d remainder / m.s.d denominator) */
			/* CAUTION: Possible case => overshoot.len > n2.len. */
			digit_ty over_shoot =
				(*rem)->num[(*rem)->len - 1] / n2->num[n2->len - 1];

			if ((*rem)->num[(*rem)->len - 1] % n2->num[n2->len - 1])
				over_shoot++;

			bi_isubtract_int(&q_estimate, over_shoot);
		}
		else
		{
			/* q_estimate was too small. */
			/* under_shoot = floor(m.s.d remainder / m.s.d denominator) */
			digit_ty under_shoot =
				(*rem)->num[(*rem)->len - 1] / n2->num[n2->len - 1];

			bi_iadd_int(&q_estimate, under_shoot);
		}

		*rem = _bi_free(*rem);
		*rem = get_remainder(slice, n2, &q_estimate);
		if (!(*rem))
			return (-1);

		rem_gteq_divisor = _bi_compare_const(*rem, n2) >= 0;
	}

	return (q_estimate.num[0]);
}

/*!
 * @brief drop in the next "digits" from numerator into slice.
 * @private @memberof bigint
 *
 * @param[out] slice holder for "digits" to be dropped.
 * @param[in] rem remainder from previous division step.
 * @param[in] n1 numerator/dividend.
 * @param[in] n1_i index of the next "digit" to be dropped.
 * @param[in] n2 denominator/divisor.
 *
 * @return number of "digits" dropped from n1.
 */
static len_ty drop_next(
	bigint *const restrict slice, const bigint *const restrict rem,
	const bigint *const restrict n1, len_ty n1_i,
	const bigint *const restrict n2
)
{
	len_ty due = n2->len, offset = 1;

	if (rem)  // Move "digits" from remainder into slice.
	{
		memcpy(
			&slice->num[slice->len - rem->len], rem->num,
			sizeof(*rem->num) * rem->len
		);
		due = n2->len - rem->len;
	}
	/* If !remainder then; n2.len "digits" will be dropped from n1. */

	/* Drop in as many "digits" as possible from n1. */
	/* n1_i should not wrap. */
	/* calling function must also detect this condition. */
	if (due > (n1_i + 1))
	{
		offset += due - (n1_i + 1);
		due = n1_i + 1;
		n1_i = 0;
		memcpy(&slice->num[offset], &n1->num[n1_i], sizeof(*n1->num) * due);
		return (due);
	}

	/* Drop in "digits" from n1 until due == 0. */
	if (due)
	{
		n1_i -= due - 1;  // n1_i is already included.
		memcpy(&slice->num[offset], &n1->num[n1_i], sizeof(*n1->num) * due);
		/* n1_i should point to the index of the next "digit" to drop. */
		n1_i--;
	}

	if (_cmp_rev_uint_arr(&slice->num[offset], n2->num, n2->len) < 0)
	{  // If slice < n2 then; drop an extra "digit".
		slice->num[0] = n1->num[n1_i];
		due++;
	}

	return (due);
}

/*!
 * @brief divide two bigints.
 * @private @memberof bigint
 *
 * @param n1 numerator/divisor.
 * @param n2 denominator/dividend.
 *
 * @return a struct with pointers to the results,
 * struct with NULL pointers on error.
 */
static bi_divmod_res
divide(const bigint *const restrict n1, const bigint *const restrict n2)
{
	bigint *current_slice = NULL;
	len_ty slice_offset = 1, q_i = 0, n1_i = 0, dropped = 0;
	ldigit_ty current_q = 0;
	bi_divmod_res res = {0};

	/* Since division is reverse of multiplication then; */
	/* quotient "digits" = numerator "digits" - denominator "digits" + */
	/* (0 or 1). */
	if (n1->num[n1->len - 1] < n2->num[n2->len - 1])
		/* If m.s.d of numerator < m.s.d denominator. */
		res.quotient = _bi_alloc((n1->len - n2->len ? n1->len - n2->len : 1));
	else
		res.quotient = _bi_alloc(n1->len - n2->len + 1);

	/* len_slice = len of n2, +1 for an extra dropdown. */
	current_slice = _bi_alloc(n2->len + 1);
	if (!current_slice || !res.quotient)
		goto error_cleanup;

	n1_i = n1->len - 1;
	dropped = drop_next(current_slice, NULL, n1, n1_i, n2);
	if (n1_i < dropped)
		n1_i = 0;
	else  // n1_i should point to the index of the next "digit" to drop.
		n1_i -= dropped;

	slice_offset = current_slice->len - dropped;
	q_i = res.quotient->len;
	while (q_i > 0)
	{
		q_i--;
		current_q = get_current_quotient(
			&(bigint){.is_negative = current_slice->is_negative,
					  .len = current_slice->len - slice_offset,
					  .num = current_slice->num + slice_offset},
			n2, &(res.remainder)
		);
		if (current_q < 0)
			goto error_cleanup;

		res.quotient->num[q_i] = current_q;
		dropped = drop_next(current_slice, res.remainder, n1, n1_i, n2);
		if (n1_i < dropped)
			n1_i = 0;
		else
			n1_i -= dropped;

		if (dropped > 0)
		{
			slice_offset = current_slice->len - (dropped + res.remainder->len);
			/* One drop is discounted after every division. */
			q_i -= dropped - 1;
			/* For every index "i" dropped into current_slice */
			/* set quotient[i] to 0. */
			memset(
				&(res.quotient->num[q_i]), 0,
				sizeof(*(res.quotient->num)) * (dropped - 1)
			);
		}
	}

	if (0)
	{
error_cleanup:
		res.quotient = _bi_free(res.quotient);
		res.remainder = _bi_free(res.remainder);
	}

	current_slice = _bi_free(current_slice);
	_bi_trim(res.quotient);
	_bi_trim(res.remainder);
	return (res);
}

/*!
 * @brief handle division of two signed bigints.
 * @private @memberof bigint
 *
 * @param[in] n1 numerator/divisor.
 * @param[in] n2 denominator/dividend.
 *
 * @return a struct with pointers to the results,
 * struct with NULL pointers on error.
 */
static bi_divmod_res
divide_negatives(bigint *const restrict n1, bigint *const restrict n2)
{
	const bool neg1 = n1->is_negative, neg2 = n2->is_negative;
	bi_divmod_res res = {0};

	n1->is_negative = false;
	n2->is_negative = false;
	res = bi_divmod(n1, n2);
	if (!res.quotient || !res.remainder)
		goto cleanup;

	if (neg1 && neg2)  // -8 / -5 = 8 / 5
		goto cleanup;

	if (neg1 || neg2)
	{
		// -8 / 5 = -((8 / 5) + 1)
		// 8 / -5 = -((8 / 5) + 1)
		bigint one = {.len = 1, .is_negative = false, .num = (digit_ty[1]){1}};

		res.quotient = _bi_resize(res.quotient, res.quotient->len + 1);
		if (!res.quotient)
		{
			res.remainder = _bi_free(res.remainder);
			goto cleanup;
		}

		bi_iadd(res.quotient, &one);
		res.quotient->is_negative = true;
	}

cleanup:
	n1->is_negative = neg1;
	n2->is_negative = neg2;
	_bi_trim(res.quotient);
	_bi_trim(res.remainder);
	return (res);
}

/*!
 * @brief handle division of two bigints, returning both quotient and
 * remainder.
 * @public @memberof bigint
 *
 * @param[in] n1 numerator/divisor.
 * @param[in] n2 denominator/dividend.
 *
 * @return a struct with pointers to the quotient and remainder,
 * a struct with NULL pointers on error.
 */
bi_divmod_res bi_divmod(bigint *const restrict n1, bigint *const restrict n2)
{
	bi_divmod_res res = {0};

	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (res);

	if (bi_isNaN(_bi_trim(n1)) || bi_isNaN(_bi_trim(n2)))
	{
		res.quotient = _bi_alloc(0);
		res.remainder = _bi_alloc(0);
		return (res);
	}

	if (check_division_by_0(n2))
		return (res);

	if (n1->is_negative || n2->is_negative)
		res = divide_negatives(n1, n2);
	else
	{
		if (bi_iszero(n1))  // then quotient and remainder == 0
		{
			res.quotient = _bi_alloc(1);
			res.remainder = _bi_alloc(1);
			if (!res.quotient || !res.remainder)
				goto error_cleanup;

			return (res);
		}
		else if (quotient_is_less_than_1(n1, n2))  // then remainder == n1
		{
			res.quotient = _bi_alloc(1);
			res.remainder = _bi_alloc(n1->len);
			if (!res.quotient || !res.remainder)
				goto error_cleanup;

			memcpy(res.remainder->num, n1->num, sizeof(*n1->num) * n1->len);
			return (res);
		}

		res = divide(n1, n2);
	}

	if (!res.quotient || !res.remainder)
	{
error_cleanup:
		res.quotient = _bi_free(res.quotient);
		res.remainder = _bi_free(res.remainder);
		return (res);
	}

	if (res.quotient->is_negative)
	{
		// for case: -7 / 4 == -2 or 7 / -4 == -2 then;
		// 	-7 % 4 = 1 and 7 % -4 = -1
		const bool neg2 = n2->is_negative;
		bigint *const restrict old_rem = res.remainder;

		n2->is_negative = false;
		res.remainder = bi_subtract(n2, old_rem);
		n2->is_negative = neg2;
		_bi_free(old_rem);
	}

	res.remainder->is_negative = n2->is_negative;
	_bi_trim(res.quotient);
	_bi_trim(res.remainder);
	return (res);
}

/*!
 * @brief handle division of two bigints.
 * @public @memberof bigint
 *
 * @param[in] n1 numerator.
 * @param[in] n2 denominator.
 *
 * @return pointer to the result, NULL on failure.
 */
bigint *bi_divide(bigint *const restrict n1, bigint *const restrict n2)
{
	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (NULL);

	bi_divmod_res result = bi_divmod(n1, n2);

	result.remainder = _bi_free(result.remainder);
	_bi_trim(result.quotient);
	return (result.quotient);
}

/*!
 * @brief handle modulo of two bigints.
 * @public @memberof bigint
 *
 * @param[in] n1 numerator.
 * @param[in] n2 denominator.
 *
 * @return pointer to the result, NULL on failure.
 */
bigint *bi_modulo(bigint *const restrict n1, bigint *const restrict n2)
{
	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (NULL);

	bi_divmod_res result = bi_divmod(n1, n2);

	result.quotient = _bi_free(result.quotient);
	_bi_trim(result.remainder);
	return (result.remainder);
}
