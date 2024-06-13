#include "infiX.h"

u4b_array *remains = NULL;

static u4b_array *divide_negatives(u4b_array *n1, u4b_array *n2)
	__attribute__((nonnull));
static int check_0_result(u4b_array *n1, u4b_array *n2)
	__attribute__((nonnull));
static int check_division_by_0(u4b_array *n2)
	__attribute__((nonnull));
static ssize_t __attribute__((nonnull(1, 3)))
get_current_quotient(uint32_t *slice, size_t len_slice, u4b_array *n2);

/**
 * infiX_division - divides a numbers stored in an array
 * @n1: numerator
 * @n2: denominator
 *
 * Description: this function will store the remainder of any division
 * in a global variable "remains". This variable should be freed after
 * this function has returned.
 *
 * Return: array with the result, NULL on failure
 */
u4b_array *infiX_division(u4b_array *n1, u4b_array *n2)
{
	ssize_t n1_i = 0, q_i = 0, tmp = 0;
	size_t len_slice = 0;
	u4b_array *quotient = NULL;
	/*slice stores the current chunk being divided*/
	uint32_t *slice = NULL;
	ssize_t slice_offset = 1;

	if (!n1 || !n2)
		return (NULL);

	remains = free_u4b_array(remains);
	if (n1->is_negative || n2->is_negative)
		return (divide_negatives(n1, n2));

	if (check_division_by_0(n2))
		return (NULL);

	tmp = check_0_result(n1, n2);
	if (tmp < 0)
		return (NULL);

	if (tmp > 0)
	{
		quotient = alloc_u4b_array(1);
		if (!quotient)
			remains = free_u4b_array(remains);

		return (quotient);
	}

	/**
	 * Since division is reverse of multiplication then;
	 * quotient digits = numerator digits - denominator digits + (0 or 1).
	 */
	if (n1->array[n1->len - 1] < n2->array[n2->len - 1])
		quotient = alloc_u4b_array(n1->len - n2->len);
	else
		quotient = alloc_u4b_array(n1->len - n2->len + 1);

	/*len_slice = len of n2, +1 for a dropdown*/
	len_slice = n2->len + 1;
	slice = xcalloc(len_slice, sizeof(*slice));
	if (!slice || !quotient)
	{
		quotient = free_u4b_array(quotient);
		slice = free_n_null(slice);
		return (NULL);
	}

	memmove(slice + slice_offset, n1->array, n2->len);
	n1_i = n1->len - n2->len - 1;
	if (slice[len_slice - 1] < n2->array[n2->len - 1])
	{
		/**
		 * If most significant digit (msd) of slice < msd of denominator then;
		 * dropdown an extra digit from the numerator.
		 */
		slice_offset = 0;
		slice[0] = n1->array[n1_i];
		n1_i--;
	}

	tmp = 0;
	q_i = quotient->len - 1;
	while (q_i >= 0)
	{
		tmp = get_current_quotient(slice + slice_offset, len_slice - slice_offset, n2);
		if (tmp < 0)
		{
			quotient = free_u4b_array(quotient);
			slice = free_n_null(slice);
			return (NULL);
		}

		quotient->array[q_i] = tmp;

		slice_offset = len_slice - remains->len;
		/*Copy the remainder into slice starting from most significant digits.*/
		memmove(slice + slice_offset, remains->array, remains->len);
		slice_offset = 1;
		tmp = n2->len - remains->len;
		if (tmp)
		{
			if (n1_i + 1 > tmp)
				n1_i -= tmp;
			else
			{
				slice_offset += tmp - (n1_i + 1);
				tmp = (n1_i + 1);
				n1_i = 0;
			}

			/*Ensure slice has atleast n1->len digits by */
			/*dropping more from numerator.*/
			memmove(slice + slice_offset, &n1->array[n1_i], tmp);
		}

		if (n1_i && slice[len_slice - 1] < n2->array[n2->len - 1])
		{
			slice_offset = 0;
			slice[0] = n1->array[n1_i];
			n1_i--;
		}

		q_i--;
	}

	remains = free_u4b_array(remains);
	remains = alloc_u4b_array(len_slice - slice_offset);
	if (remains)
		memmove(remains->array, slice + slice_offset, len_slice - slice_offset);
	else
		quotient = free_u4b_array(quotient);

	free_n_null(slice);
	trim_u4b_array(quotient);
	return (quotient);
}

/**
 * divide_negatives - division of signed numbers (negative numbers).
 * @n1: numerator.
 * @n2: denominator.
 *
 * Return: pointer to the results, NULL on failure.
 */
u4b_array *divide_negatives(u4b_array *n1, u4b_array *n2)
{
	u4b_array *result = NULL;

	if (!n1 || !n2)
		return (NULL);

	if (!n1->array)
		n1->is_negative = 0;

	if (!n2->array)
		n2->is_negative = 0;

	if (n1->is_negative && n2->is_negative)
	{
		/* -8 / -5 = 8/5 */
		n1->is_negative = 0;
		n2->is_negative = 0;
		result = infiX_division(n1, n2);
		n1->is_negative = 1;
		n2->is_negative = 1;
	}
	else if (n1->is_negative)
	{
		/* -8 / 5 = -(8/5) */
		n1->is_negative = 0;
		result = infiX_division(n1, n2);
		n1->is_negative = 1;
		if (result)
			result->is_negative = 1;
	}
	else if (n2->is_negative)
	{
		/* 8 / -5 = -(8/5) */
		n2->is_negative = 0;
		result = infiX_division(n1, n2);
		n2->is_negative = 1;
		if (result)
			result->is_negative = 1;
	}
	else
		result = infiX_division(n1, n2);

	return (result);
}

/**
 * check_division_by_0 - checks if the denominator is zero.
 * @n2: denominator
 *
 * Return: 1 if n2 is zero, else 0
 */
int check_division_by_0(u4b_array *n2)
{
	if (!n2)
		return (0);

	trim_u4b_array(n2);
	if (!n2->len || (n2->len == 1 && !n2->array[0]))
	{
		fprintf(stderr, "Division by zero error.\n");
		return (1);
	}

	return (0);
}

/**
 * check_0_result - checks if numerator < denominator.
 * @n1: numerator
 * @n2: denominator
 *
 * Description: If numerator < denominator then;
 * quotient = 0 and remainder = n1.
 *
 * Return: 1 if result will be 0, 0 if not, -1 on error.
 */
int check_0_result(u4b_array *n1, u4b_array *n2)
{
	if (!n1 || !n2 || check_division_by_0(n2))
		return (-1);

	remains = free_u4b_array(remains);
	if (cmp_u4barray(n1, n2) < 0)
	{
		remains = alloc_u4b_array(n1->len);
		if (!remains)
			return (-1);

		memmove(remains->array, n1->array, n1->len);
		return (1);
	}

	return (0);
}

/**
 * get_current_quotient - calculate the current quotient.
 * @slice: the current digits being divided in an array.
 * @len_slice: length of slice.
 * @n2: the denominator.
 *
 * Return: an int representing current quotient, -1 on error.
 */
ssize_t get_current_quotient(uint32_t *slice, size_t len_slice, u4b_array *n2)
{
	uint32_t temp_array[1] = {0};
	u4b_array estimate = {.len = 1, .is_negative = 0, .array = temp_array};
	u4b_array slice_array = {0}, *estimate_check = NULL;
	int64_t msd_slice = 0;

	if (!slice || !n2)
		return (-1);

	remains = free_u4b_array(remains);
	slice_array.array = slice;
	slice_array.len = len_slice;
	msd_slice = slice_array.array[len_slice - 1];
	if (len_slice > n2->len)
		msd_slice = (msd_slice * MAX_VAL_u4b) + slice_array.array[len_slice - 2];

	/**
	 * The quotient is approximated by dividing the most significant digits
	 * of current slice and denominator.
	 * The approximate is then adjusted depending on how much it's product
	 * with the denominator overshoot/undershoots the slice.
	 */
	estimate.array[0] = msd_slice / n2->array[n2->len - 1];
	estimate_check = infiX_multiplication(n2, &estimate);
	remains = infiX_subtraction(&slice_array, estimate_check);
	if (!remains)
	{
		free_u4b_array(estimate_check);
		return (-1);
	}

	while (remains->is_negative || cmp_u4barray(remains, n2) > -1)
	{
		if (remains->is_negative)
		{
			/*estimate was too big.*/
			/*over_shoot = ceil(msd remains / msd denominator)*/

			/*Test for possible overflow.*/
			/*Test for longer remains than denominator.*/
			estimate.array[0] -= remains->array[remains->len - 1] / n2->array[n2->len - 1];
			if (remains->array[remains->len - 1] % n2->array[n2->len - 1])
				estimate.array[0]--;
		}
		else
		{
			/*estimate was too small.*/
			/*under_shoot = floor(msd remains / msd denominator)*/
			estimate.array[0] += remains->array[remains->len - 1] / n2->array[n2->len - 1];
		}

		estimate_check = free_u4b_array(estimate_check);
		remains = free_u4b_array(remains);

		estimate_check = infiX_multiplication(n2, &estimate);
		remains = infiX_subtraction(&slice_array, estimate_check);
		if (!remains)
		{
			free_u4b_array(estimate_check);
			return (-1);
		}
	}

	free_u4b_array(estimate_check);
	return (estimate.array[0]);
}
