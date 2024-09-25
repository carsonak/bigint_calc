#include "bignum_math.h"

static ATTR_NONNULL lint
get_current_quotient(bignum *slice, bignum *n2, bignum **rem);
static ATTR_NONNULL bool check_division_by_0(bignum *n2);
static ATTR_NONNULL bool check_0_result(bignum *n1, bignum *n2);
static ATTR_NONNULL bignum *
divide(bignum *n1, bignum *n2, bignum **rem);
static ATTR_NONNULL bignum *
divide_negatives(bignum *n1, bignum *n2, bignum **rem);

/**
 * get_current_quotient - calculate the current quotient.
 * @slice: the current digits being divided in an array.
 * @len_slice: length of slice.
 * @n2: the denominator.
 * @rem: Address of a bignum pointer to store the remainder.
 *
 * Return: an int representing current quotient, -1 on error.
 */
static lint
get_current_quotient(bignum *slice, bignum *n2, bignum **rem)
{
	uint temp_array[1] = {0};
	bignum q_estimate = {
		.len = 1, .is_negative = false, .num = temp_array};
	bignum *estimate_check = NULL;
	lint msd_slice = 0, is_larger = 0;

	*rem = bn_free(*rem);
	msd_slice = slice->num[slice->len - 1];
	if (slice->len > n2->len)
		msd_slice = (msd_slice * BIGNUM_UINT_MAX) + slice->num[slice->len - 2];

	/*quotient ≈ most significant digit of slice / m.s.d of denominator.*/
	q_estimate.num[0] = msd_slice / n2->num[n2->len - 1];
	estimate_check = bn_multiplication(n2, &q_estimate);
	*rem = bn_subtraction(slice, estimate_check);
	if (!(*rem) || !estimate_check)
	{
		*rem = bn_free(*rem);
		bn_free(estimate_check);
		return (-1);
	}

	/*0 <= (slice - (q_estimate * denominator)) < denominator*/
	is_larger = cmp_bignum(*rem, n2);
	while ((*rem)->is_negative || is_larger >= 0)
	{
		if ((*rem)->is_negative)
		{
			/*q_estimate was too big.*/
			/*over_shoot = ceil(m.s.d rem / m.s.d denominator)*/

			/*TODO: Test for possible overflow.*/
			/*TODO: Test, overshoot might be longer than denominator*/
			q_estimate.num[0] -= (*rem)->num[(*rem)->len - 1] / n2->num[n2->len - 1];
			if (q_estimate.num[0] &&
				((*rem)->num[(*rem)->len - 1] % n2->num[n2->len - 1]))
				q_estimate.num[0]--;
		}
		else
		{
			/*q_estimate was too small.*/
			/*under_shoot = floor(m.s.d rem / m.s.d denominator)*/
			q_estimate.num[0] += (*rem)->num[(*rem)->len - 1] / n2->num[n2->len - 1];
		}

		estimate_check = bn_free(estimate_check);
		bn_free(*rem);

		estimate_check = bn_multiplication(n2, &q_estimate);
		*rem = bn_subtraction(slice, estimate_check);
		if (!(*rem) || !estimate_check)
		{
			*rem = bn_free(*rem);
			bn_free(estimate_check);
			return (-1);
		}

		is_larger = cmp_bignum(*rem, n2);
	}

	bn_free(estimate_check);
	return (q_estimate.num[0]);
}

/**
 * check_division_by_0 - checks if the denominator is zero.
 * @n2: denominator.
 *
 * Return: 1 if n2 is zero, else 0.
 */
static bool check_division_by_0(bignum *n2)
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
 * @n1: numerator.
 * @n2: denominator.
 *
 * Return: 1 if numerator < denominator, 0 if not.
 */
static bool check_0_result(bignum *n1, bignum *n2)
{
	if (cmp_bignum(n1, n2) >= 0)
		return (false);

	return (true);
}

/**
 * drop_next - drops in the next digits from numerator.
 */
// static size_t drop_next(bignum *slice, bignum *rem, bignum *n1, size_t n1_i, bignum *n2)
// {
// }

/**
 * divide - divides two bignums.
 * @n1: numerator/divisor.
 * @n2: denominator/dividend.
 * @rem: Address of a bignum pointer to store the remainder.
 *
 * Return: pointer ro the result, NULL on failure.
 */
static bignum *divide(bignum *n1, bignum *n2, bignum **rem)
{
	bignum *current_slice = NULL;
	size_t slc_offset = 1, q_i = 0, n1_i = 0;
	lint tmp = 0;
	bignum *quotient = NULL;

	/*Since division is reverse of multiplication then;*/
	/*quotient digits = numerator digits - denominator digits + (0 or 1).*/
	if (n1->num[n1->len - 1] < n2->num[n2->len - 1])
		/*If m.s.d of numerator < m.s.d denominator.*/
		quotient = bn_alloc((n1->len - n2->len ? n1->len - n2->len : 1));
	else
		quotient = bn_alloc(n1->len - n2->len + 1);

	/*len_slice = len of n2, +1 for a dropdown*/
	current_slice = bn_alloc(n2->len + 1);
	if (!current_slice || !quotient)
		goto error_cleanup;

	n1_i = n1->len - n2->len;
	memmove(&current_slice->num[slc_offset], &n1->num[n1_i], sizeof(*n1->num) * n2->len);
	if (n1_i)
		n1_i--;

	if (current_slice->num[current_slice->len - 1] < n2->num[n2->len - 1])
	{
		/*If most significant digit (m.s.d) of current_slice < m.s.d of denominator then;*/
		/*dropdown an extra digit from the numerator.*/
		slc_offset = 0;
		current_slice->num[0] = n1->num[n1_i];
		if (n1_i)
			n1_i--;
	}

	tmp = 0;
	q_i = quotient->len;
	while (q_i > 0)
	{
		q_i--;
		current_slice->num += slc_offset;
		current_slice->len -= slc_offset;
		tmp = get_current_quotient(current_slice, n2, rem);
		current_slice->num -= slc_offset;
		current_slice->len += slc_offset;
		if (tmp < 0)
			goto error_cleanup;

		quotient->num[q_i] = tmp;
		/*Copy remainder into current_slice starting from m.s.ds.*/
		memmove(&current_slice->num[current_slice->len - (*rem)->len], (*rem)->num, sizeof(*(*rem)->num) * (*rem)->len);
		slc_offset = 1;
		tmp = n2->len - (*rem)->len;
		/*If remainder is shorter than denominator then; drop in more digits*/
		if (q_i && (ulint)tmp > 0)
		{
			/*Be careful of unsigned int wrapping.*/
			if (n1_i + 1 > (ulint)tmp)
				n1_i -= tmp;
			else
			{
				slc_offset += (ulint)tmp - (n1_i + 1);
				tmp = (n1_i + 1);
				n1_i = 0;
			}

			memmove(&current_slice->num[slc_offset], &n1->num[n1_i], sizeof(*n1->num) * tmp);
			q_i -= tmp - 1;
			/*For every index "i" dropped into current_slice set quotient[i] to 0.*/
			memset(&quotient->num[q_i], 0, sizeof(*quotient->num) * ((ulint)tmp - 1));
		}

		if (q_i && cmp_rev_uint32array(&current_slice->num[slc_offset], n2->num, n2->len) < 0)
		{
			slc_offset = 0;
			current_slice->num[0] = n1->num[n1_i];
			if (n1_i)
				n1_i--;
		}
	}

	bn_free(*rem);
	*rem = bn_alloc(current_slice->len - slc_offset);
	if (*rem) /* The number left over in current_slice is the remainder */
		memmove((*rem)->num, current_slice->num + slc_offset, sizeof(*current_slice->num) * (current_slice->len - slc_offset));

	if (!(*rem))
	{
	error_cleanup:
		quotient = bn_free(quotient);
		*rem = bn_free(*rem);
	}

	bn_free(current_slice);
	trim_bignum(quotient);
	trim_bignum(*rem);
	return (quotient);
}

/**
 * divide_negatives - handle division of two signed bignums.
 * @n1: numerator.
 * @n2: denominator.
 * @rem: Address of a bignum pointer to store the remainder.
 *
 * Return: pointer to the result, NULL on failure.
 */
static bignum *divide_negatives(bignum *n1, bignum *n2, bignum **rem)
{
	bool neg1 = n1->is_negative, neg2 = n2->is_negative;
	bignum *result = NULL;
	uint a[1] = {1};
	bignum one = {.len = 1, .is_negative = false, .num = a};

	n1->is_negative = false;
	n2->is_negative = false;
	/*If n1 == 0; then *rem == 0*/
	if (is_zero(n1))
		*rem = bn_alloc(1);
	else
	{
		*rem = bn_alloc(n1->len);
		if (*rem)
			memmove((*rem)->num, n1->num, sizeof(*n1->num) * n1->len);
	}

	if (!(*rem))
		goto error_cleanup;

	if (check_0_result(n1, n2))
	{
		result = bn_alloc(1);
		goto clean_exit;
	}

	if (neg1 && neg2) /* -8 // -5 = 8//5*/
		result = divide(n1, n2, rem);
	else if (neg1 || neg2)
	{
		/* -8 // 5 = -((8 // 5) + 1)*/
		/* 8 // -5 = -((8 // 5) + 1) */
		result = divide(n1, n2, rem);
		if (!bn_realloc(result, result->len + 1))
			goto error_cleanup;

		bn_add_inplace(result, &one);
		result->is_negative = true;
	}

	if (!result)
	{
	error_cleanup:
		*rem = bn_free(*rem);
		result = bn_free(result);
	}

clean_exit:
	n1->is_negative = neg1;
	n2->is_negative = neg2;
	trim_bignum(result);
	trim_bignum(*rem);
	return (result);
}

/**
 * bn_division - handle division of two bignums.
 * @n1: numerator.
 * @n2: denominator.
 *
 * This function does preliminary checks on the parameters.
 *
 * Return: pointer to the result, NULL on failure.
 */
bignum *bn_division(bignum *n1, bignum *n2)
{
	bignum *result = NULL, *rem = NULL;

	if (!n1 || !n2)
		return (NULL);

	trim_bignum(n1);
	trim_bignum(n2);
	if (check_division_by_0(n2))
		return (NULL);

	if (n1->is_negative || n2->is_negative)
		result = divide_negatives(n1, n2, &rem);
	else
	{
		/*If n1 == 0; then rem == 0*/
		/*No need for this check as rem is not needed here.*/
		if (check_0_result(n1, n2))
			result = bn_alloc(1);
		else
			result = divide(n1, n2, &rem);
	}

	bn_free(rem);
	trim_bignum(result);
	return (result);
}

/**
 * bn_modulus - handle modulo of two bignums.
 * @n1: numerator.
 * @n2: denominator.
 *
 * This function does preliminary checks on the parameters.
 *
 * Return: pointer to the result, NULL on failure.
 */
bignum *bn_modulus(bignum *n1, bignum *n2)
{
	bool is_negative = 0;
	bignum *result = NULL, *rem = NULL;

	if (!n1 || !n2)
		return (NULL);

	trim_bignum(n1);
	trim_bignum(n2);
	if (check_division_by_0(n2))
		return (NULL);

	if (n1->is_negative || n2->is_negative)
		result = divide_negatives(n1, n2, &rem);
	else
	{
		/*If n1 == 0; then rem == 0*/
		if (is_zero(n1))
			rem = bn_alloc(1);
		else
		{
			rem = bn_alloc(n1->len);
			if (rem)
				memmove(rem->num, n1->num, sizeof(*n1->num) * n1->len);
		}

		if (!rem)
			return (NULL);

		if (check_0_result(n1, n2))
			result = bn_alloc(1);
		else
			result = divide(n1, n2, &rem);
	}

	if (!result)
		return (bn_free(rem));

	if (result->is_negative)
	{
		/*for case: -7 // 4 == -2 or 7 // -4 == -2 then;*/
		/*-7 % 4 = 1 and 7 % -4 = -1*/
		is_negative = n2->is_negative;
		n2->is_negative = false;
		bn_free(result);

		result = bn_subtraction(n2, rem);
		n2->is_negative = is_negative;
		bn_free(rem);
		rem = result;
		result = NULL;
	}

	if (rem)
		rem->is_negative = n2->is_negative;

	bn_free(result);
	trim_bignum(rem);
	return (rem);
}
