#include "infiX.h"

u4b_array *remains = NULL;

static u4b_array *divide_negatives(u4b_array *n1, u4b_array *n2)
	ATTR_NONNULL;
static char check_0_result(u4b_array *n1, u4b_array *n2) ATTR_NONNULL;
static int check_division_by_0(u4b_array *n2) ATTR_NONNULL;
static u4b_array *divide(u4b_array *n1, u4b_array *n2) ATTR_NONNULL;
static ssize_t ATTR_NONNULL_IDX(1, 3)
	get_current_quotient(uint32_t *working_slice, size_t len_slice, u4b_array *n2);

/**
 * infiX_division - divides a numbers stored in an array
 * @n1: numerator
 * @n2: denominator
 *
 * @description
 *
 * Return: array with the result, NULL on failure
 */
u4b_array *infiX_division(u4b_array *n1, u4b_array *n2)
{
	char is_zero = 0;
	u4b_array *result = NULL;

	if (!n1 || !n2)
		return (NULL);

	remains = free_u4b_array(remains);
	trim_u4b_array(n1);
	trim_u4b_array(n2);
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
			result = alloc_u4b_array(1);
		else
			result = divide(n1, n2);
	}

	remains = free_u4b_array(remains);
	trim_u4b_array(result);
	return (result);
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
	char is_zero = 0, is_negative = 0;
	u4b_array *result = NULL;

	if (!n1 || !n2)
		return (NULL);

	remains = free_u4b_array(remains);
	trim_u4b_array(n1);
	trim_u4b_array(n2);
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
			result = alloc_u4b_array(1);
		else
			result = divide(n1, n2);
	}

	if (!result)
		return (free_u4b_array(remains));

	if (result->is_negative)
	{
		/*for case: -7 // 4 == -2 or 7 // -4 == -2 then;*/
		/*-7 % 4 = 1 and 7 % -4 = -1*/
		is_negative = n2->is_negative;
		n2->is_negative = false;
		result = free_u4b_array(result);

		result = infiX_subtraction(n2, remains);
		n2->is_negative = is_negative;
		remains = free_u4b_array(remains);
		remains = result;
		result = NULL;
	}

	if (remains)
		remains->is_negative = n2->is_negative;

	free_u4b_array(result);
	trim_u4b_array(remains);
	return (remains);
}

/**
 * divide_negatives - division of signed numbers (negative numbers).
 * @n1: numerator.
 * @n2: denominator.
 *
 * Return: pointer to the result, NULL on failure.
 */
u4b_array *divide_negatives(u4b_array *n1, u4b_array *n2)
{
	char is_zero = 0, neg1 = n1->is_negative, neg2 = n2->is_negative;
	uint32_t a[] = {1};
	u4b_array *tmp = NULL, *result = NULL;
	u4b_array one = {.len = 1, .is_negative = false, .array = a};

	n1->is_negative = false;
	n2->is_negative = false;
	is_zero = check_0_result(n1, n2);
	if (is_zero < 0)
		return (NULL);
	else if (is_zero > 0)
		return (alloc_u4b_array(1));

	if (neg1 && neg2) /* -8 // -5 = 8//5*/
		result = divide(n1, n2);
	else if (neg1 || neg2)
	{
		/* -8 // 5 = -((8 // 5) + 1)*/
		/* 8 // -5 = -((8 // 5) + 1) */
		tmp = divide(n1, n2);
		result = infiX_addition(tmp, &one);
		if (result)
			result->is_negative = true;
	}

	n1->is_negative = neg1;
	n2->is_negative = neg2;
	free_u4b_array(tmp);
	trim_u4b_array(result);
	trim_u4b_array(remains);
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
char check_0_result(u4b_array *n1, u4b_array *n2)
{
	if (cmp_u4barray(n1, n2) >= 0)
		return (0);

	remains = free_u4b_array(remains);
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
	uint32_t *working_slice = NULL;
	size_t slice_offset = 1, q_i = 0, n1_i = 0, len_slice = 0;
	ssize_t tmp = 0;
	u4b_array *quotient = NULL;

	/*Since division is reverse of multiplication then;*/
	/*quotient digits = numerator digits - denominator digits + (0 or 1).*/
	if (n1->array[n1->len - 1] < n2->array[n2->len - 1])
		/*If msd of numerator < msd denominator.*/
		quotient = alloc_u4b_array((n1->len - n2->len ? n1->len - n2->len : 1));
	else
		quotient = alloc_u4b_array(n1->len - n2->len + 1);

	/*len_slice = len of n2, +1 for a dropdown*/
	len_slice = n2->len + 1;
	working_slice = xcalloc(len_slice, sizeof(*working_slice));
	if (!working_slice || !quotient)
	{
		quotient = free_u4b_array(quotient);
		working_slice = free_n_null(working_slice);
		return (NULL);
	}

	n1_i = n1->len - n2->len;
	memmove(&working_slice[slice_offset], &n1->array[n1_i], sizeof(*n1->array) * n2->len);
	if (n1_i)
		n1_i--;

	if (working_slice[len_slice - 1] < n2->array[n2->len - 1])
	{
		/*If most significant digit (msd) of working_slice < msd of denominator then;*/
		/*dropdown an extra digit from the numerator.*/
		slice_offset = 0;
		working_slice[0] = n1->array[n1_i];
		if (n1_i)
			n1_i--;
	}

	tmp = 0;
	q_i = quotient->len;
	while (q_i > 0)
	{
		q_i--;
		tmp = get_current_quotient(working_slice + slice_offset,
								   len_slice - slice_offset, n2);
		if (tmp < 0)
		{
			remains = free_u4b_array(remains);
			free_n_null(working_slice);
			return (free_u4b_array(quotient));
		}

		quotient->array[q_i] = tmp;
		/*Copy remainder into working_slice starting from most significant digits.*/
		memmove(&working_slice[len_slice - remains->len], remains->array,
				sizeof(*remains->array) * remains->len);
		slice_offset = 1;
		tmp = n2->len - remains->len;
		/*If remainder is shorter than denominator then; drop in more digits*/
		if (q_i && (size_t)tmp > 0)
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

			memmove(&working_slice[slice_offset], &n1->array[n1_i],
					sizeof(*n1->array) * tmp);
			q_i -= tmp - 1;
		}

		if (q_i &&
			cmp_rev_uint32array(&working_slice[slice_offset], n2->array, n2->len) < 0)
		{
			slice_offset = 0;
			working_slice[0] = n1->array[n1_i];
			if (n1_i)
				n1_i--;
		}
	}

	remains = free_u4b_array(remains);
	remains = alloc_u4b_array(len_slice - slice_offset);
	if (remains)
		memmove(remains->array, working_slice + slice_offset,
				sizeof(*working_slice) * (len_slice - slice_offset));
	else
		quotient = free_u4b_array(quotient);

	free_n_null(working_slice);
	trim_u4b_array(quotient);
	trim_u4b_array(remains);
	return (quotient);
}

/**
 * get_current_quotient - calculate the current quotient.
 * @working_slice: the current digits being divided in an array.
 * @len_slice: length of working_slice.
 * @n2: the denominator.
 *
 * Return: an int representing current quotient, -1 on error.
 */
ssize_t get_current_quotient(uint32_t *working_slice, size_t len_slice, u4b_array *n2)
{
	uint32_t temp_array[1] = {0};
	u4b_array q_estimate = {.len = 1, .is_negative = false, .array = temp_array};
	u4b_array slice_array = {.len = len_slice, .is_negative = false, .array = NULL};
	u4b_array *estimate_check = NULL;
	ssize_t msd_slice = 0, is_larger = 0;

	remains = free_u4b_array(remains);
	slice_array.array = working_slice;
	msd_slice = slice_array.array[len_slice - 1];
	if (len_slice > n2->len)
		msd_slice = (msd_slice * MAX_VAL_u4b) + slice_array.array[len_slice - 2];

	/*quotient approximation ≈ most significant digit of current working_slice / */
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

	/*0 <= (current working_slice - (q_estimate * denominator)) < denominator*/
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
			if (q_estimate.array[0] &&
				(remains->array[remains->len - 1] % n2->array[n2->len - 1]))
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
// 	u4b_array num1 = {.len = 0, .is_negative = false, .array = NULL};
// 	u4b_array num2 = {.len = 0, .is_negative = false, .array = NULL};

// 	num1.len = sizeof(in1) / sizeof(*in1);
// 	num1.array = in1;
// 	num2.len = sizeof(in2) / sizeof(*in2);
// 	num2.array = in2;
// 	u4b_array *output = infiX_division(&num1, &num2);

// 	output = free_u4b_array(output);

// 	return (0);
// }
