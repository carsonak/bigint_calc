#include "infiX.h"

u4b_array *remains = NULL;

static u4b_array *divide_negatives(u4b_array *n1, u4b_array *n2)
	ATTR_NONNULL;
static int check_0_result(u4b_array *n1, u4b_array *n2) ATTR_NONNULL;
static int check_division_by_0(u4b_array *n2) ATTR_NONNULL;
static u4b_array *divide(u4b_array *n1, u4b_array *n2) ATTR_NONNULL;
static ssize_t ATTR_NONNULL_IDX(1, 3)
	get_current_quotient(uint32_t *slice, size_t len_slice, u4b_array *n2);

/**
 * infiX_division - preliminary checks before division
 * @n1: numerator
 * @n2: denominator
 *
 * Return: array with the result, NULL on failure
 */
u4b_array *infiX_division(u4b_array *n1, u4b_array *n2)
{
	int is_zero = 0;
	u4b_array *results = NULL;

	if (!n1 || !n2)
		return (NULL);

	remains = free_u4b_array(remains);
	trim_u4b_array(n1);
	trim_u4b_array(n2);

	if (n1->is_negative || n2->is_negative)
		return (divide_negatives(n1, n2));

	if (check_division_by_0(n2))
		return (NULL);

	is_zero = check_0_result(n1, n2);
	if (is_zero < 0)
		return (NULL);
	else if (is_zero > 0)
		results = alloc_u4b_array(1);
	else
		results = divide(n1, n2);

	remains = free_u4b_array(remains);
	return (results);
}

/**
 * infiX_modulus - does preliminary checks before getting remainder
 * @n1: numerator
 * @n2: denominator
 *
 * Return: array with the result, NULL on failure
 */
u4b_array *infiX_modulus(u4b_array *n1, u4b_array *n2)
{
	int is_zero = 0;
	u4b_array *results = NULL;

	if (!n1 || !n2)
		return (NULL);

	remains = free_u4b_array(remains);
	trim_u4b_array(n1);
	trim_u4b_array(n2);

	if (n1->is_negative || n2->is_negative)
	{
		results = divide_negatives(n1, n2);
		free_u4b_array(remains);
		remains = infiX_subtraction(n1, results);
		free_u4b_array(results);
		return (remains);
	}

	if (check_division_by_0(n2))
		return (NULL);

	is_zero = check_0_result(n1, n2);
	if (is_zero < 0)
		return (NULL);
	else if (!is_zero)
		free_u4b_array(divide(n1, n2));

	return (remains);
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
	uint32_t a[] = {1};
	u4b_array *tmp = NULL, *result = NULL;
	u4b_array one = {.len = 1, .is_negative = 0, .array = a};

	if (n1->is_negative && n2->is_negative)
	{
		/* -8 // -5 = 8//5*/
		n1->is_negative = 0;
		n2->is_negative = 0;
		result = infiX_division(n1, n2);
		n1->is_negative = 1;
		n2->is_negative = 1;
	}
	else if (n1->is_negative)
	{
		/* -8 // 5 = -((8 // 5) + 1)*/
		n1->is_negative = 0;
		tmp = infiX_division(n1, n2);
		result = infiX_addition(tmp, &one);
		tmp = free_u4b_array(tmp);
		n1->is_negative = 1;
		if (result)
			result->is_negative = 1;
	}
	else if (n2->is_negative)
	{
		/* 8 // -5 = -((8 // 5) + 1) */
		n2->is_negative = 0;
		tmp = infiX_division(n1, n2);
		result = infiX_addition(tmp, &one);
		tmp = free_u4b_array(tmp);
		n2->is_negative = 1;
		if (result)
			result->is_negative = 1;
	}

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
 * Description: If numerator is less than denominator then;
 * quotient will be 0 and remainder will be equal to numerator.
 *
 * Return: 1 if result will be 0, 0 if not, -1 on error.
 */
int check_0_result(u4b_array *n1, u4b_array *n2)
{
	if (cmp_u4barray(n1, n2) >= 0)
		return (0);

	/*If n1 == 0; then remains == 0*/
	if (n1->len < 2 && (!n1->array || !n1->array[0]))
		remains = alloc_u4b_array(1);
	else
		remains = alloc_u4b_array(n1->len);

	if (!remains)
		return (-1);

	if (n1->len > 1 || (n1->len == 1 && n1->array[0]))
		memmove(remains->array, n1->array, sizeof(*n1->array) * n1->len);

	return (1);
}

/**
 * divide - divides a numbers stored in an array
 * @n1: numerator
 * @n2: denominator
 *
 * Return: array with the result, NULL on failure
 */
u4b_array *divide(u4b_array *n1, u4b_array *n2)
{
	ssize_t q_i = 0, tmp = 0;
	size_t n1_i = 0, len_slice = 0;
	u4b_array *quotient = NULL;
	/*slice stores the current chunk being divided*/
	uint32_t *slice = NULL;
	size_t slice_offset = 1;

	/**
	 * Since division is reverse of multiplication then;
	 * quotient digits = numerator digits - denominator digits + (0 or 1).
	 */
	if (n1->array[n1->len - 1] < n2->array[n2->len - 1])
		/*If msd of numerator < msd denominator.*/
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

	n1_i = n1->len - n2->len;
	memmove(&slice[slice_offset], &n1->array[n1_i], sizeof(*n1->array) * n2->len);
	if (n1_i)
		n1_i--;

	if (slice[len_slice - 1] < n2->array[n2->len - 1])
	{
		/*If most significant digit (msd) of slice < msd of denominator then;*/
		/*dropdown an extra digit from the numerator.*/
		slice_offset = 0;
		slice[0] = n1->array[n1_i];
		if (n1_i)
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

		/*Copy the remainder into slice starting from most significant digits.*/
		memmove(&slice[len_slice - remains->len], remains->array,
				sizeof(*remains->array) * remains->len);
		slice_offset = 1;
		tmp = n2->len - remains->len;
		/*If remainder is shorter than denominator then; drop in more digits*/
		if ((size_t)tmp > 0)
		{
			/*Checking for overflow.*/
			if (n1_i + 1 > (size_t)tmp)
				n1_i -= tmp;
			else
			{
				slice_offset += (size_t)tmp - (n1_i + 1);
				tmp = (n1_i + 1);
				n1_i = 0;
			}

			memmove(&slice[slice_offset], &n1->array[n1_i], sizeof(*n1->array) * tmp);
			q_i -= tmp - 1;
		}

		if (cmp_rev_uint32array(&slice[slice_offset], n2->array, n2->len) < 0)
		{
			slice_offset = 0;
			slice[0] = n1->array[n1_i];
			if (n1_i)
				n1_i--;
		}

		q_i--;
	}

	remains = free_u4b_array(remains);
	remains = alloc_u4b_array(len_slice - slice_offset);
	if (remains)
		memmove(remains->array, slice + slice_offset,
				sizeof(*slice) * (len_slice - slice_offset));
	else
		quotient = free_u4b_array(quotient);

	free_n_null(slice);
	trim_u4b_array(quotient);
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
ssize_t get_current_quotient(uint32_t *slice, size_t len_slice, u4b_array *n2)
{
	uint32_t temp_array[1] = {0};
	u4b_array q_estimate = {.len = 1, .is_negative = 0, .array = temp_array};
	u4b_array slice_array = {.len = len_slice, .is_negative = 0, .array = NULL};
	u4b_array *estimate_check = NULL;
	ssize_t msd_slice = 0, is_larger = 0;

	remains = free_u4b_array(remains);
	slice_array.array = slice;
	msd_slice = slice_array.array[len_slice - 1];
	if (len_slice > n2->len)
		msd_slice = (msd_slice * MAX_VAL_u4b) + slice_array.array[len_slice - 2];

	/*quotient approximation ≈ most significant digit of current slice / */
	/*msd of denominator.*/
	q_estimate.array[0] = (int64_t)msd_slice / n2->array[n2->len - 1];
	estimate_check = infiX_multiplication(n2, &q_estimate);
	remains = infiX_subtraction(&slice_array, estimate_check);
	if (!remains || !estimate_check)
	{
		remains = free_u4b_array(remains);
		free_u4b_array(estimate_check);
		return (-1);
	}

	/*0 <= (current slice - (q_estimate * denominator)) < denominator*/
	is_larger = cmp_u4barray(remains, n2);
	while (remains->is_negative || is_larger >= 0)
	{
		if (remains->is_negative)
		{
			/*q_estimate was too big.*/
			/*over_shoot = ceil(msd remains / msd denominator)*/

			/*Test: for possible overflow.*/
			/*Test: overshoot might be longer than denominator*/
			q_estimate.array[0] -= remains->array[remains->len - 1] / n2->array[n2->len - 1];
			if (remains->array[remains->len - 1] % n2->array[n2->len - 1])
				q_estimate.array[0]--;
		}
		else
		{
			/*q_estimate was too small.*/
			/*under_shoot = floor(msd remains / msd denominator)*/
			q_estimate.array[0] += remains->array[remains->len - 1] / n2->array[n2->len - 1];
		}

		estimate_check = free_u4b_array(estimate_check);
		remains = free_u4b_array(remains);

		estimate_check = infiX_multiplication(n2, &q_estimate);
		remains = infiX_subtraction(&slice_array, estimate_check);
		if (!remains || !estimate_check)
		{
			remains = free_u4b_array(remains);
			free_u4b_array(estimate_check);
			return (-1);
		}

		is_larger = cmp_u4barray(remains, n2);
	}

	free_u4b_array(estimate_check);
	return (q_estimate.array[0]);
}

/**
 * main - test
 *
 * Return: 0
 */
// int main(void)
// {
// 	uint32_t in1[] = {65000, 000000005, 45000, 0, 550005550, 2100};
// 	uint32_t in2[] = {0, 75006};
// 	// uint32_t out[] = {164342, 0, 0, 0, 0, 999775000, 22499999};
// 	u4b_array num1 = {.len = 0, .is_negative = 0, .array = NULL};
// 	u4b_array num2 = {.len = 0, .is_negative = 0, .array = NULL};

// 	num1.len = sizeof(in1) / sizeof(*in1);
// 	num1.array = in1;
// 	num2.len = sizeof(in2) / sizeof(*in2);
// 	num2.array = in2;
// 	u4b_array *output = infiX_division(&num1, &num2);

// 	output = free_u4b_array(output);

// 	return (0);
// }
