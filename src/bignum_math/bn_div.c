#include "infiX.h"

/* Stores the remainder of division. */
bignum *remains = NULL;

static bignum *divide_negatives(bignum *n1, bignum *n2) ATTR_NONNULL;
static int check_0_result(bignum *n1, bignum *n2) ATTR_NONNULL;
static bool check_division_by_0(bignum *n2) ATTR_NONNULL;
static bignum *divide(bignum *n1, bignum *n2) ATTR_NONNULL;
ATTR_NONNULL_IDX(1, 3)
static lint get_current_quotient(uint *slice, size_t len_slice, bignum *n2);

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
	bignum *result = NULL;

	if (!n1 || !n2)
		return (NULL);

	remains = free_bignum(remains);
	trim_bignum(n1);
	trim_bignum(n2);
	if (check_division_by_0(n2))
		return (NULL);

	if (n1->is_negative || n2->is_negative)
		result = divide_negatives(n1, n2);
	else
	{
		is_zero = check_0_result(n1, n2);
		if (is_zero < 0)
			return (NULL);
		else if (is_zero > 0)
			result = alloc_bignum(1);
		else
			result = divide(n1, n2);
	}

	remains = free_bignum(remains);
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
	bignum *result = NULL;

	if (!n1 || !n2)
		return (NULL);

	remains = free_bignum(remains);
	trim_bignum(n1);
	trim_bignum(n2);
	if (check_division_by_0(n2))
		return (NULL);

	if (n1->is_negative || n2->is_negative)
		result = divide_negatives(n1, n2);
	else
	{
		is_zero = check_0_result(n1, n2);
		if (is_zero < 0)
			return (NULL);
		else if (is_zero > 0)
			result = alloc_bignum(1);
		else
			result = divide(n1, n2);
	}

	if (!result)
		return (free_bignum(remains));

	if (result->is_negative)
	{
		/*for case: -7 // 4 == -2 or 7 // -4 == -2 then;*/
		/*-7 % 4 = 1 and 7 % -4 = -1*/
		is_negative = n2->is_negative;
		n2->is_negative = false;
		result = free_bignum(result);

		result = bn_subtraction(n2, remains);
		n2->is_negative = is_negative;
		remains = free_bignum(remains);
		remains = result;
		result = NULL;
	}

	if (remains)
		remains->is_negative = n2->is_negative;

	free_bignum(result);
	trim_bignum(remains);
	return (remains);
}

/**
 * divide_negatives - handle division of two signed bignums.
 * @n1: numerator.
 * @n2: denominator.
 *
 * Return: pointer to the result, NULL on failure.
 */
bignum *divide_negatives(bignum *n1, bignum *n2)
{
	int is_zero = 0;
	bool neg1 = n1->is_negative, neg2 = n2->is_negative;
	bignum *tmp = NULL, *result = NULL;
	uint a[1] = {1};
	bignum one = {.len = 1, .is_negative = false, .num = a};

	n1->is_negative = false;
	n2->is_negative = false;
	is_zero = check_0_result(n1, n2);
	if (is_zero < 0)
		return (NULL);
	else if (is_zero > 0)
		return (alloc_bignum(1));

	if (neg1 && neg2) /* -8 // -5 = 8//5*/
		result = divide(n1, n2);
	else if (neg1 || neg2)
	{
		/* -8 // 5 = -((8 // 5) + 1)*/
		/* 8 // -5 = -((8 // 5) + 1) */
		tmp = divide(n1, n2);
		result = bn_addition(tmp, &one);
		if (result)
			result->is_negative = true;
	}

	n1->is_negative = neg1;
	n2->is_negative = neg2;
	free_bignum(tmp);
	trim_bignum(result);
	trim_bignum(remains);
	return (result);
}

/**
 * check_division_by_0 - checks if the denominator is zero.
 * @n2: denominator.
 *
 * Return: 1 if n2 is zero, else 0.
 */
bool check_division_by_0(bignum *n2)
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
 * Description: If numerator is less than denominator then;.
 * quotient will be 0 and remainder will be equal to numerator.
 *
 * Return: 1 if numerator < denominator, 0 if not, -1 on error.
 */
int check_0_result(bignum *n1, bignum *n2)
{
	if (cmp_bignum(n1, n2) >= 0)
		return (0);

	remains = free_bignum(remains);
	/*If n1 == 0; then remains == 0*/
	if (is_zero(n1))
		remains = alloc_bignum(1);
	else
		remains = alloc_bignum(n1->len);

	if (!remains)
		return (-1);

	if (!is_zero(n1))
		memmove(remains->num, n1->num, sizeof(*n1->num) * n1->len);

	return (1);
}

/**
 * divide - divides two bignums.
 * @n1: numerator.
 * @n2: denominator.
 *
 * Return: pointer ro the result, NULL on failure.
 */
bignum *divide(bignum *n1, bignum *n2)
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
		tmp = get_current_quotient(slice + slice_offset,
								   len_slice - slice_offset, n2);
		if (tmp < 0)
		{
			remains = free_bignum(remains);
			free_n_null(slice);
			return (free_bignum(quotient));
		}

		quotient->num[q_i] = tmp;
		/*Copy remainder into slice starting from most significant digits.*/
		memmove(&slice[len_slice - remains->len], remains->num,
				sizeof(*remains->num) * remains->len);
		slice_offset = 1;
		tmp = n2->len - remains->len;
		/*If remainder is shorter than denominator then; drop in more digits*/
		if (q_i && (ulint)tmp > 0)
		{
			/*Checking for overflow.*/
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
			/*For every dropped digit set quotient to 0.*/
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

	remains = free_bignum(remains);
	remains = alloc_bignum(len_slice - slice_offset);
	if (remains)
		memmove(remains->num, slice + slice_offset,
				sizeof(*slice) * (len_slice - slice_offset));
	else
		quotient = free_bignum(quotient);

	free_n_null(slice);
	trim_bignum(quotient);
	trim_bignum(remains);
	return (quotient);
}

/**
 * get_current_quotient - calculate the current quotient.
 * @slice: the current digits being divided in an array.
 * @len_slice: length of slice.
 * @n2: the denominator.
 *
 * Return: an int representing current quotient, -1 on error.
 */
lint get_current_quotient(
	uint *slice, size_t len_slice, bignum *n2)
{
	uint temp_array[1] = {0};
	bignum q_estimate = {
		.len = 1, .is_negative = false, .num = temp_array};
	bignum slice_bignum = {
		.len = len_slice, .is_negative = false, .num = slice};
	bignum *estimate_check = NULL;
	lint msd_slice = 0, is_larger = 0;

	remains = free_bignum(remains);
	msd_slice = slice_bignum.num[len_slice - 1];
	if (len_slice > n2->len)
		msd_slice = (msd_slice * BIGNUM_UINT_MAX) + slice_bignum.num[len_slice - 2];

	/*quotient ≈ most significant digit of slice / msd of denominator.*/
	q_estimate.num[0] = msd_slice / n2->num[n2->len - 1];
	estimate_check = bn_multiplication(n2, &q_estimate);
	remains = bn_subtraction(&slice_bignum, estimate_check);
	if (!remains || !estimate_check)
	{
		remains = free_bignum(remains);
		free_bignum(estimate_check);
		return (-1);
	}

	/*0 <= (slice - (q_estimate * denominator)) < denominator*/
	is_larger = cmp_bignum(remains, n2);
	while (remains->is_negative || is_larger >= 0)
	{
		if (remains->is_negative)
		{
			/*q_estimate was too big.*/
			/*over_shoot = ceil(msd remains / msd denominator)*/

			/*Test: for possible overflow.*/
			/*Test: overshoot might be longer than denominator*/
			q_estimate.num[0] -= remains->num[remains->len - 1] / n2->num[n2->len - 1];
			if (q_estimate.num[0] &&
				(remains->num[remains->len - 1] % n2->num[n2->len - 1]))
				q_estimate.num[0]--;
		}
		else
		{
			/*q_estimate was too small.*/
			/*under_shoot = floor(msd remains / msd denominator)*/
			q_estimate.num[0] += remains->num[remains->len - 1] / n2->num[n2->len - 1];
		}

		estimate_check = free_bignum(estimate_check);
		remains = free_bignum(remains);

		estimate_check = bn_multiplication(n2, &q_estimate);
		remains = bn_subtraction(&slice_bignum, estimate_check);
		if (!remains || !estimate_check)
		{
			remains = free_bignum(remains);
			free_bignum(estimate_check);
			return (-1);
		}

		is_larger = cmp_bignum(remains, n2);
	}

	free_bignum(estimate_check);
	return (q_estimate.num[0]);
}
