#include "_bi_internals.h"
#include "bigint.h"
#include <stdio.h> /* fprintf */

static bool check_division_by_0(const bigint *const n2) ATTR_NONNULL;
static bool quotient_is_less_than_1(
	const bigint *const restrict n1, const bigint *const restrict n2
) ATTR_NONNULL;
static bigint *get_remainder(
	bigint *const restrict n1, bigint *const restrict n2,
	bigint *const restrict quotient
) ATTR_NONNULL;
static l_int get_current_quotient(
	bigint *const restrict slice, bigint *const restrict n2,
	bigint *restrict *const restrict remainder
);
static len_type drop_next(
	bigint *const restrict slice, const bigint *const restrict remainder,
	const bigint *const restrict n1, len_type n1_i,
	const bigint *const restrict n2
) ATTR_NONNULL_IDX(1, 3, 5);
static bi_div_res
divide(bigint *const restrict n1, bigint *const restrict n2) ATTR_NONNULL;
static bi_div_res divide_negatives(
	bigint *const restrict n1, bigint *const restrict n2
) ATTR_NONNULL;

/**
 * check_division_by_0 - checks if the denominator is zero.
 * @n2: denominator.
 *
 * Return: 1 if n2 is zero, else 0.
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

/**
 * quotient_is_less_than_1 - checks if numerator < denominator.
 * @n1: numerator/divisor.
 * @n2: denominator/dividend.
 *
 * Return: true if numerator < denominator, false if not.
 */
static bool quotient_is_less_than_1(
	const bigint *const restrict n1, const bigint *const restrict n2
)
{
	return (_bi_compare_const(n1, n2) < 0);
}

/**
 * get_remainder - calculate remainder.
 * @n1: the dividend.
 * @n2: the divisor.
 * @quotient: current quotient.
 *
 * remainder = dividend - (divisor * quotient)
 *
 * Return: pointer to the remainder on success, NULL on failure.
 */
static bigint *get_remainder(
	bigint *const restrict n1, bigint *const restrict n2,
	bigint *const restrict quotient
)
{
	bigint *const restrict multiple = bi_multiply(n2, quotient);
	bigint *restrict rem = NULL;

	if (multiple)
		rem = bi_subtract(n1, multiple);

	_bi_free(multiple);
	return (rem);
}

/**
 * get_current_quotient - calculate the current quotient.
 * @slice: the current number being divided.
 * @n2: the denominator.
 * @remainder: Address of a `bigint` pointer to store the remainder.
 *
 * Return: an int representing current quotient, -1 on error.
 */
static l_int get_current_quotient(
	bigint *const restrict slice, bigint *const restrict n2,
	bigint *restrict *const restrict remainder
)
{
	bigint q_estimate = {.len = 1, .is_negative = false, .num = (u_int[3]){0}};
	len_type excess = 0;
	l_int msd_slice = 0, bigger_than_divisor = 0;

	*remainder = _bi_free(*remainder);
	msd_slice = slice->num[slice->len - 1];
	if (slice->len > n2->len)
		msd_slice = (msd_slice * BIGINT_BASE) + slice->num[slice->len - 2];

	/* quotient ≈ most significant "digit" of slice / m.s.d of denominator. */
	q_estimate.num[0] = msd_slice / n2->num[n2->len - 1];
	*remainder = get_remainder(slice, n2, &q_estimate);
	if (!(*remainder))
		return (-1);

	/* 0 <= (slice - (q_estimate * denominator)) < denominator */
	bigger_than_divisor = bi_compare(*remainder, n2);
	while ((*remainder)->is_negative || bigger_than_divisor >= 0)
	{
		if ((*remainder)->is_negative)
		{
			/* q_estimate was too big. */
			/* over_shoot = ceil(m.s.d remainder / m.s.d denominator) */
			/* CAUTION: Possible case => overshoot.len > n2.len. */
			excess =
				(*remainder)->num[(*remainder)->len - 1] / n2->num[n2->len - 1];
			if ((*remainder)->num[(*remainder)->len - 1] % n2->num[n2->len - 1])
				++excess;

			bi_isubtract_int(&q_estimate, excess);
		}
		else
		{
			/* q_estimate was too small. */
			/* under_shoot = floor(m.s.d remainder / m.s.d denominator) */
			excess =
				(*remainder)->num[(*remainder)->len - 1] / n2->num[n2->len - 1];
			bi_iadd_int(&q_estimate, excess);
		}

		*remainder = _bi_free(*remainder);
		*remainder = get_remainder(slice, n2, &q_estimate);
		if (!(*remainder))
			return (-1);

		bigger_than_divisor = bi_compare(*remainder, n2);
	}

	return (q_estimate.num[0]);
}

/**
 * drop_next - drops in the next "digits" from numerator.
 * @slice: holder for "digits" to be dropped.
 * @remainder: remainder from previous division step.
 * @n1: numerator/dividend.
 * @n1_i: index of the next "digit" to be dropped.
 * @n2: denominator/divisor.
 *
 * Return: number of "digits" dropped from n1.
 */
static len_type drop_next(
	bigint *const restrict slice, const bigint *const restrict remainder,
	const bigint *const restrict n1, len_type n1_i,
	const bigint *const restrict n2
)
{
	len_type due = n2->len, offset = 1;

	if (remainder) /* Move "digits" from remainder into slice. */
	{
		memmove(
			&slice->num[slice->len - remainder->len], remainder->num,
			sizeof(*remainder->num) * remainder->len);
		due = n2->len - remainder->len;
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
		memmove(&slice->num[offset], &n1->num[n1_i], sizeof(*n1->num) * due);
		return (due);
	}

	/* Drop in "digits" from n1 until due == 0. */
	if (due)
	{
		n1_i -= due - 1; /* n1_i is already included. */
		memmove(&slice->num[offset], &n1->num[n1_i], sizeof(*n1->num) * due);
		/* n1_i should point to the index of the next "digit" to drop. */
		n1_i--;
	}

	if (_cmp_rev_uint_arr(&slice->num[offset], n2->num, n2->len) < 0)
	{ /* If slice < n2 then; drop an extra "digit". */
		slice->num[0] = n1->num[n1_i];
		++due;
	}

	return (due);
}

/**
 * divide - divides two bigints.
 * @n1: numerator/divisor.
 * @n2: denominator/dividend.
 *
 * Return: a struct with pointers to the results,
 * struct with NULL pointers on error.
 */
static bi_div_res divide(bigint *const restrict n1, bigint *const restrict n2)
{
	bigint *current_slice = NULL;
	len_type slice_offset = 1, q_i = 0, n1_i = 0, dropped = 0;
	l_int current_q = 0;
	bi_div_res res = {0};

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
	else /* n1_i should point to the index of the next "digit" to drop. */
		n1_i -= dropped;

	slice_offset = current_slice->len - dropped;
	q_i = res.quotient->len;
	while (q_i > 0)
	{
		q_i--;
		current_slice->num += slice_offset;
		current_slice->len -= slice_offset;
		current_q = get_current_quotient(current_slice, n2, &(res.remainder));
		current_slice->num -= slice_offset;
		current_slice->len += slice_offset;
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

/**
 * divide_negatives - handle division of signed bigints.
 * @n1: numerator/divisor.
 * @n2: denominator/dividend.
 *
 * Return: a struct with pointers to the results,
 * struct with NULL pointers on error.
 */
static bi_div_res
divide_negatives(bigint *const restrict n1, bigint *const restrict n2)
{
	const bool neg1 = n1->is_negative, neg2 = n2->is_negative;
	bi_div_res res = {0};

	n1->is_negative = false;
	n2->is_negative = false;
	res = bi_divmod(n1, n2);
	if (!res.quotient || !res.remainder)
		goto cleanup;

	if (neg1 && neg2) /* -8 / -5 = 8 / 5 */
		goto cleanup;

	if (neg1 || neg2)
	{
		/* -8 / 5 = -((8 / 5) + 1) */
		/* 8 / -5 = -((8 / 5) + 1) */
		bigint one = {.len = 1, .is_negative = false, .num = (u_int[1]){1}};

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

/**
 * bi_divmod - handle division of two bigints,
 * return quotient and reminder.
 * @n1: numerator/divisor.
 * @n2: denominator/dividend.
 *
 * Return: a struct with pointers to the quotient and remainder,
 * struct with NULL pointers on error.
 */
bi_div_res bi_divmod(bigint *const restrict n1, bigint *const restrict n2)
{
	bi_div_res res = {0};

	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (res);

	_bi_trim(n1);
	_bi_trim(n2);
	if (bi_isNaN(n1) || bi_isNaN(n2))
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
		if (bi_iszero(n1)) /* then quotient and remainder == 0 */
		{
			res.quotient = _bi_alloc(1);
			res.remainder = _bi_alloc(1);
			if (!res.quotient || !res.remainder)
				goto error_cleanup;

			return (res);
		}
		else if (quotient_is_less_than_1(n1, n2)) /* then remainder == n1 */
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
		/* for case: -7 // 4 == -2 or 7 // -4 == -2 then; */
		/* -7 % 4 = 1 and 7 % -4 = -1 */
		bool neg2 = n2->is_negative;
		bigint *old_rem = res.remainder;

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

/**
 * bi_divide - handle division of two bigints.
 * @n1: numerator.
 * @n2: denominator.
 *
 * Return: pointer to the result, NULL on failure.
 */
bigint *bi_divide(bigint *const restrict n1, bigint *const restrict n2)
{
	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (NULL);

	bi_div_res result = bi_divmod(n1, n2);

	result.remainder = _bi_free(result.remainder);
	_bi_trim(result.quotient);
	return (result.quotient);
}

/**
 * bi_modulo - handle modulo of two bigints.
 * @n1: numerator.
 * @n2: denominator.
 *
 * Return: pointer to the result, NULL on failure.
 */
bigint *bi_modulo(bigint *const restrict n1, bigint *const restrict n2)
{
	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (NULL);

	bi_div_res result = bi_divmod(n1, n2);

	result.quotient = _bi_free(result.quotient);
	_bi_trim(result.remainder);
	return (result.remainder);
}
