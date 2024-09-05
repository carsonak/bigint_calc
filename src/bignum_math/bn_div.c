#include "bignum_math.h"

static ATTR_NONNULL lint
get_current_quotient(uint *slice, size_t len_slice, bignum *n2, bignum **rem);
static ATTR_NONNULL bool check_division_by_0(bignum *n2);
static ATTR_NONNULL int
check_0_result(bignum *n1, bignum *n2, bignum **rem);
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
get_current_quotient(uint *slice, size_t len_slice, bignum *n2, bignum **rem)
{
	uint temp_array[1] = {0};
	bignum q_estimate = {
		.len = 1, .is_negative = false, .num = temp_array};
	bignum slice_bignum = {
		.len = len_slice, .is_negative = false, .num = slice};
	bignum *estimate_check = NULL;
	lint msd_slice = 0, is_larger = 0;

	*rem = free_bignum(*rem);
	msd_slice = slice_bignum.num[len_slice - 1];
	if (len_slice > n2->len)
		msd_slice = (msd_slice * BIGNUM_UINT_MAX) + slice_bignum.num[len_slice - 2];

	/*quotient ≈ most significant digit of slice / msd of denominator.*/
	q_estimate.num[0] = msd_slice / n2->num[n2->len - 1];
	estimate_check = bn_multiplication(n2, &q_estimate);
	*rem = bn_subtraction(&slice_bignum, estimate_check);
	if (!(*rem) || !estimate_check)
	{
		*rem = free_bignum(*rem);
		free_bignum(estimate_check);
		return (-1);
	}

	/*0 <= (slice - (q_estimate * denominator)) < denominator*/
	is_larger = cmp_bignum(*rem, n2);
	while ((*rem)->is_negative || is_larger >= 0)
	{
		if ((*rem)->is_negative)
		{
			/*q_estimate was too big.*/
			/*over_shoot = ceil(msd rem / msd denominator)*/

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
			/*under_shoot = floor(msd rem / msd denominator)*/
			q_estimate.num[0] += (*rem)->num[(*rem)->len - 1] / n2->num[n2->len - 1];
		}

		estimate_check = free_bignum(estimate_check);
		free_bignum(*rem);

		estimate_check = bn_multiplication(n2, &q_estimate);
		*rem = bn_subtraction(&slice_bignum, estimate_check);
		if (!(*rem) || !estimate_check)
		{
			*rem = free_bignum(*rem);
			free_bignum(estimate_check);
			return (-1);
		}

		is_larger = cmp_bignum(*rem, n2);
	}

	free_bignum(estimate_check);
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
 * @rem: Address of a bignum pointer to store the remainder.
 *
 * Description: If numerator is less than denominator then;.
 * quotient will be 0 and remainder will be equal to numerator.
 *
 * Return: 1 if numerator < denominator, 0 if not, -1 on error.
 */
static int check_0_result(bignum *n1, bignum *n2, bignum **rem)
{
	if (cmp_bignum(n1, n2) >= 0)
		return (0);

	/*If n1 == 0; then *rem == 0*/
	if (is_zero(n1))
		*rem = alloc_bignum(1);
	else
		*rem = alloc_bignum(n1->len);

	if (!(*rem))
		return (-1);

	if (!is_zero(n1))
		memmove((*rem)->num, n1->num, sizeof(*n1->num) * n1->len);

	return (1);
}

/**
 * divide - divides two bignums.
 * @n1: numerator.
 * @n2: denominator.
 * @rem: Address of a bignum pointer to store the remainder.
 *
 * Return: pointer ro the result, NULL on failure.
 */
static bignum *divide(bignum *n1, bignum *n2, bignum **rem)
{
	uint *slice = NULL;
	size_t slice_offset = 1, q_i = 0, n1_i = 0, len_slice = 0;
	lint tmp = 0;
	bignum *quotient = NULL;

	/*Since division is reverse of multiplication then;*/
	/*quotient digits = numerator digits - denominator digits + (0 or 1).*/
	if (n1->num[n1->len - 1] < n2->num[n2->len - 1])
		/*If msd of numerator < msd denominator.*/
		quotient = alloc_bignum((n1->len - n2->len ? n1->len - n2->len : 1));
	else
		quotient = alloc_bignum(n1->len - n2->len + 1);

	/*len_slice = len of n2, +1 for a dropdown*/
	len_slice = n2->len + 1;
	slice = xcalloc(len_slice, sizeof(*slice));
	if (!slice || !quotient)
	{
		free_n_null(slice);
		return (free_bignum(quotient));
	}

	n1_i = n1->len - n2->len;
	memmove(&slice[slice_offset], &n1->num[n1_i], sizeof(*n1->num) * n2->len);
	if (n1_i)
		n1_i--;

	if (slice[len_slice - 1] < n2->num[n2->len - 1])
	{
		/*If most significant digit (msd) of slice < msd of denominator then;*/
		/*dropdown an extra digit from the numerator.*/
		slice_offset = 0;
		slice[0] = n1->num[n1_i];
		if (n1_i)
			n1_i--;
	}

	tmp = 0;
	q_i = quotient->len;
	while (q_i > 0)
	{
		q_i--;
		tmp = get_current_quotient(
			slice + slice_offset, len_slice - slice_offset, n2, rem);
		if (tmp < 0)
		{
			*rem = free_bignum(*rem);
			free_n_null(slice);
			return (free_bignum(quotient));
		}

		quotient->num[q_i] = tmp;
		/*Copy remainder into slice starting from most significant digits.*/
		memmove(&slice[len_slice - (*rem)->len], (*rem)->num,
				sizeof(*(*rem)->num) * (*rem)->len);
		slice_offset = 1;
		tmp = n2->len - (*rem)->len;
		/*If remainder is shorter than denominator then; drop in more digits*/
		if (q_i && (ulint)tmp > 0)
		{
			/*Have to be careful of unsigned int wrapping.*/
			if (n1_i + 1 > (ulint)tmp)
				n1_i -= tmp;
			else
			{
				slice_offset += (ulint)tmp - (n1_i + 1);
				tmp = (n1_i + 1);
				n1_i = 0;
			}

			memmove(&slice[slice_offset], &n1->num[n1_i],
					sizeof(*n1->num) * tmp);
			q_i -= tmp - 1;
			/*For every index "i" dropped into slice set quotient[i] to 0.*/
			memset(&quotient->num[q_i], 0, sizeof(*quotient->num) * ((ulint)tmp - 1));
		}

		if (q_i &&
			cmp_rev_uint32array(&slice[slice_offset], n2->num, n2->len) < 0)
		{
			slice_offset = 0;
			slice[0] = n1->num[n1_i];
			if (n1_i)
				n1_i--;
		}
	}

	free_bignum(*rem);
	*rem = alloc_bignum(len_slice - slice_offset);
	if (*rem) /* The number left over in slice is the remainder */
		memmove((*rem)->num, slice + slice_offset,
				sizeof(*slice) * (len_slice - slice_offset));
	else
		quotient = free_bignum(quotient);

	free_n_null(slice);
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
	int is_zero = 0;
	bool neg1 = n1->is_negative, neg2 = n2->is_negative;
	bignum *tmp = NULL, *result = NULL;
	uint a[1] = {1};
	bignum one = {.len = 1, .is_negative = false, .num = a};

	n1->is_negative = false;
	n2->is_negative = false;
	is_zero = check_0_result(n1, n2, rem);
	if (is_zero < 0)
		return (NULL);
	else if (is_zero > 0)
		return (alloc_bignum(1));

	if (neg1 && neg2) /* -8 // -5 = 8//5*/
		result = divide(n1, n2, rem);
	else if (neg1 || neg2)
	{
		/* -8 // 5 = -((8 // 5) + 1)*/
		/* 8 // -5 = -((8 // 5) + 1) */
		tmp = divide(n1, n2, rem);
		result = bn_addition(tmp, &one);
		if (result)
			result->is_negative = true;
	}

	n1->is_negative = neg1;
	n2->is_negative = neg2;
	free_bignum(tmp);
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
	int is_zero = 0;
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
		is_zero = check_0_result(n1, n2, &rem);
		if (is_zero < 0)
			return (NULL);
		else if (is_zero > 0)
			result = alloc_bignum(1);
		else
			result = divide(n1, n2, &rem);
	}

	free_bignum(rem);
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
	int is_zero = 0;
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
		is_zero = check_0_result(n1, n2, &rem);
		if (is_zero < 0)
			return (NULL);
		else if (is_zero > 0)
			result = alloc_bignum(1);
		else
			result = divide(n1, n2, &rem);
	}

	if (!result)
		return (free_bignum(rem));

	if (result->is_negative)
	{
		/*for case: -7 // 4 == -2 or 7 // -4 == -2 then;*/
		/*-7 % 4 = 1 and 7 % -4 = -1*/
		is_negative = n2->is_negative;
		n2->is_negative = false;
		free_bignum(result);

		result = bn_subtraction(n2, rem);
		n2->is_negative = is_negative;
		free_bignum(rem);
		rem = result;
		result = NULL;
	}

	if (rem)
		rem->is_negative = n2->is_negative;

	free_bignum(result);
	trim_bignum(rem);
	return (rem);
}
