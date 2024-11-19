#include "bignum_math.h"
#include <stdio.h> /* fprintf */

static ATTR_NONNULL bool check_division_by_0(bignum * const n2);
static ATTR_NONNULL bool check_0_result(bignum * const n1, bignum * const n2);
static ATTR_NONNULL bignum *
get_remainder(bignum *const n1, bignum *const n2, bignum *quotient);
static ATTR_NONNULL l_int
get_current_quotient(bignum *slice, bignum *const n2, bignum **remainder);
static ATTR_NONNULL bignum *
divide(bignum *const n1, bignum *const n2, bignum **remainder);
static ATTR_NONNULL bignum *
divide_negatives(bignum *const n1, bignum *const n2, bignum **remainder);

/**
 * check_division_by_0 - checks if the denominator is zero.
 * @n2: denominator.
 *
 * Return: 1 if n2 is zero, else 0.
 */
static bool check_division_by_0(bignum *const n2)
{
	if (!n2->len || (n2->len == 1 && !n2->num[0]))
	{
		fprintf(stderr, "Division by zero error.\n");
		return (true);
	}

	return (false);
}

/**
 * check_0_result - checks if numerator < denominator.
 * @n1: numerator/divisor.
 * @n2: denominator/dividend.
 *
 * Return: true if numerator < denominator, false if not.
 */
static bool check_0_result(bignum *const n1, bignum *const n2)
{
	if (bn_compare(n1, n2) < 0)
		return (true);

	return (false);
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
static bignum *
get_remainder(bignum *const n1, bignum *const n2, bignum *quotient)
{
	bignum *multiple = bn_multiply(n2, quotient);
	bignum *rem = NULL;

	if (multiple)
		rem = bn_subtract(n1, multiple);

	bn_free(multiple);
	return (rem);
}

/**
 * get_current_quotient - calculate the current quotient.
 * @slice: the current number being divided.
 * @n2: the denominator.
 * @remainder: Address of a bignum pointer to store the remainder.
 *
 * Return: an int representing current quotient, -1 on error.
 */
static l_int
get_current_quotient(bignum *slice, bignum *const n2, bignum **remainder)
{
	bignum q_estimate = {.len = 1, .is_negative = false, .num = (u_int[3]) {0}};
	size_t excess = 0;
	l_int msd_slice = 0, bigger_than_divisor = 0;

	*remainder = bn_free(*remainder);
	msd_slice = slice->num[slice->len - 1];
	if (slice->len > n2->len)
		msd_slice = (msd_slice * BIGNUM_BASE) + slice->num[slice->len - 2];

	/*quotient ≈ most significant "digit" of slice / m.s.d of denominator.*/
	q_estimate.num[0] = msd_slice / n2->num[n2->len - 1];
	*remainder = get_remainder(slice, n2, &q_estimate);
	if (!(*remainder))
		return (-1);

	/*0 <= (slice - (q_estimate * denominator)) < denominator*/
	bigger_than_divisor = bn_compare(*remainder, n2);
	while ((*remainder)->is_negative || bigger_than_divisor >= 0)
	{
		if ((*remainder)->is_negative)
		{
			/*q_estimate was too big.*/
			/*over_shoot = ceil(m.s.d remainder / m.s.d denominator)*/
			/*CAUTION: Possible case => overshoot.len > n2.len.*/
			excess = (*remainder)->num[(*remainder)->len - 1] / n2->num[n2->len - 1];
			if ((*remainder)->num[(*remainder)->len - 1] % n2->num[n2->len - 1])
				++excess;

			bn_isubtract_int(&q_estimate, excess);
		}
		else
		{
			/*q_estimate was too small.*/
			/*under_shoot = floor(m.s.d remainder / m.s.d denominator)*/
			excess = (*remainder)->num[(*remainder)->len - 1] / n2->num[n2->len - 1];
			bn_iadd_int(&q_estimate, excess);
		}

		*remainder = bn_free(*remainder);
		*remainder = get_remainder(slice, n2, &q_estimate);
		if (!(*remainder))
			return (-1);

		bigger_than_divisor = bn_compare(*remainder, n2);
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
static size_t drop_next(
	bignum *slice, bignum *remainder,
	bignum *const n1, size_t n1_i, bignum *const n2)
{
	size_t due = n2->len, offset = 1;

	if (remainder) /*Move "digits" from remainder into slice.*/
	{
		memmove(&slice->num[slice->len - remainder->len],
				remainder->num, sizeof(*remainder->num) * remainder->len);
		due = n2->len - remainder->len;
	}
	/*If !remainder then; n2.len "digits" will be dropped from n1.*/

	/*Drop in as many "digits" as possible from n1.*/
	/*n1_i should not wrap.*/
	/*calling function must also detect this condition.*/
	if (due > (n1_i + 1))
	{
		offset += due - (n1_i + 1);
		due = n1_i + 1;
		n1_i = 0;
		memmove(&slice->num[offset], &n1->num[n1_i], sizeof(*n1->num) * due);
		return (due);
	}

	/*Drop in "digits" from n1 until due == 0.*/
	if (due)
	{
		n1_i -= due - 1; /*n1_i is already included.*/
		memmove(&slice->num[offset], &n1->num[n1_i], sizeof(*n1->num) * due);
		/*n1_i should point to the index of the next "digit" to drop.*/
		n1_i--;
	}

	if (reverse_cmp_uint32array(&slice->num[offset], n2->num, n2->len) < 0)
	{ /*If slice < n2 then; drop an extra "digit".*/
		slice->num[0] = n1->num[n1_i];
		++due;
	}

	return (due);
}

/**
 * divide - divides two bignums.
 * @n1: numerator/divisor.
 * @n2: denominator/dividend.
 * @remainder: Address of a bignum pointer to store the remainder.
 *
 * Return: pointer ro the result, NULL on failure.
 */
static bignum *divide(bignum *const n1, bignum *const n2, bignum **remainder)
{
	bignum *current_slice = NULL;
	size_t slice_offset = 1, q_i = 0, n1_i = 0, dropped = 0;
	l_int current_q = 0;
	bignum *quotient = NULL;

	/*Since division is reverse of multiplication then;*/
	/*quotient "digits" = numerator "digits" - denominator "digits" + (0 or 1).*/
	if (n1->num[n1->len - 1] < n2->num[n2->len - 1])
		/*If m.s.d of numerator < m.s.d denominator.*/
		quotient = bn_alloc((n1->len - n2->len ? n1->len - n2->len : 1));
	else
		quotient = bn_alloc(n1->len - n2->len + 1);

	/*len_slice = len of n2, +1 for an extra dropdown.*/
	current_slice = bn_alloc(n2->len + 1);
	if (!current_slice || !quotient)
		goto error_cleanup;

	n1_i = n1->len - 1;
	dropped = drop_next(current_slice, NULL, n1, n1_i, n2);
	if (n1_i < dropped)
		n1_i = 0;
	else /*n1_i should point to the index of the next "digit" to drop.*/
		n1_i -= dropped;

	slice_offset = current_slice->len - dropped;
	q_i = quotient->len;
	while (q_i > 0)
	{
		q_i--;
		current_slice->num += slice_offset;
		current_slice->len -= slice_offset;
		current_q = get_current_quotient(current_slice, n2, remainder);
		current_slice->num -= slice_offset;
		current_slice->len += slice_offset;
		if (current_q < 0)
			goto error_cleanup;

		quotient->num[q_i] = current_q;
		dropped = drop_next(current_slice, *remainder, n1, n1_i, n2);
		if (n1_i < dropped)
			n1_i = 0;
		else
			n1_i -= dropped;

		if (dropped)
		{
			slice_offset = current_slice->len - (dropped + (*remainder)->len);
			q_i -= dropped - 1; /*One drop is discounted after every division.*/
			/*For every index "i" dropped into current_slice set quotient[i] to 0.*/
			memset(&quotient->num[q_i], 0, sizeof(*quotient->num) * (dropped - 1));
		}
	}

	if (0)
	{
error_cleanup:
		quotient = bn_free(quotient);
		*remainder = bn_free(*remainder);
	}

	current_slice = bn_free(current_slice);
	trim_bignum(quotient);
	trim_bignum(*remainder);
	return (quotient);
}

/**
 * divide_negatives - handle division of two signed bignums.
 * @n1: numerator.
 * @n2: denominator.
 * @remainder: Address of a bignum pointer to store the remainder.
 *
 * Return: pointer to the result, NULL on failure.
 */
static bignum *
divide_negatives(bignum *const n1, bignum *const n2, bignum **remainder)
{
	bool neg1 = n1->is_negative, neg2 = n2->is_negative;
	bignum *result = NULL;
	u_int a[1] = {1};
	bignum one = {.len = 1, .is_negative = false, .num = a};

	n1->is_negative = false;
	n2->is_negative = false;
	/*If n1 == 0; then *remainder == 0*/
	if (is_zero(n1))
		*remainder = bn_alloc(1);
	else if (check_0_result(n1, n2))
	{
		result = bn_alloc(1);
		*remainder = bn_alloc(n1->len);
		if (!(*remainder))
			goto error_cleanup;

		memmove((*remainder)->num, n1->num, sizeof(*n1->num) * n1->len);
		goto clean_exit;
	}

	if (neg1 && neg2) /* -8 // -5 = 8//5*/
		result = divide(n1, n2, remainder);
	else if (neg1 || neg2)
	{
		/* -8 // 5 = -((8 // 5) + 1)*/
		/* 8 // -5 = -((8 // 5) + 1) */
		result = divide(n1, n2, remainder);
		if (!bn_realloc(result, result->len + 1))
			goto error_cleanup;

		bn_iadd(result, &one);
		result->is_negative = true;
	}

	if (!result)
	{
error_cleanup:
	*remainder = bn_free(*remainder);
	result = bn_free(result);
	}

clean_exit:
	n1->is_negative = neg1;
	n2->is_negative = neg2;
	trim_bignum(result);
	trim_bignum(*remainder);
	return (result);
}

/**
 * bn_divide - handle division of two bignums.
 * @n1: numerator.
 * @n2: denominator.
 *
 * Return: pointer to the result, NULL on failure.
 */
bignum *bn_divide(bignum *const n1, bignum *const n2)
{
	bignum *result = NULL, *remainder = NULL;

	if (!n1 || !n2)
		return (NULL);

	trim_bignum(n1);
	trim_bignum(n2);
	if (check_division_by_0(n2))
		return (NULL);

	if (n1->is_negative || n2->is_negative)
		result = divide_negatives(n1, n2, &remainder);
	else
	{
		/*If n1 == 0; then remainder == 0*/
		/*No need for this check as remainder is not needed here.*/
		if (check_0_result(n1, n2))
			result = bn_alloc(1);
		else
			result = divide(n1, n2, &remainder);
	}

	bn_free(remainder);
	trim_bignum(result);
	return (result);
}

/**
 * bn_modulo - handle modulo of two bignums.
 * @n1: numerator.
 * @n2: denominator.
 *
 * Return: pointer to the result, NULL on failure.
 */
bignum *bn_modulo(bignum *const n1, bignum *const n2)
{
	bool is_negative = 0;
	bignum *result = NULL, *remainder = NULL;

	if (!n1 || !n2)
		return (NULL);

	trim_bignum(n1);
	trim_bignum(n2);
	if (check_division_by_0(n2))
		return (NULL);

	if (n1->is_negative || n2->is_negative)
		result = divide_negatives(n1, n2, &remainder);
	else
	{
		/*If n1 == 0; then remainder == 0*/
		if (is_zero(n1))
			return (bn_alloc(1));

		if (check_0_result(n1, n2))
		{
			/* No need to allocate for the quotient*/
			/* result = bn_alloc(1); */
			remainder = bn_alloc(n1->len);
			if (remainder)
				memmove(remainder->num, n1->num, sizeof(*n1->num) * n1->len);

			return (remainder);
		}

		result = divide(n1, n2, &remainder);
	}

	if (!result)
		return (bn_free(remainder));

	if (remainder && result->is_negative)
	{
		/*for case: -7 // 4 == -2 or 7 // -4 == -2 then;*/
		/*-7 % 4 = 1 and 7 % -4 = -1*/
		is_negative = n2->is_negative;
		n2->is_negative = false;
		bn_free(result);

		result = bn_subtract(n2, remainder);
		n2->is_negative = is_negative;
		bn_free(remainder);
		remainder = result;
		result = NULL;
	}

	if (remainder)
		remainder->is_negative = n2->is_negative;

	bn_free(result);
	trim_bignum(remainder);
	return (remainder);
}
