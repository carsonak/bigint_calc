#include "infiX.h"

u4b_array *remains = NULL;

static u4b_array *divide_negatives(u4b_array *n1, u4b_array *n2);
static int check_0_result(u4b_array *n1, u4b_array *n2);
static int check_division_by_0(u4b_array *n2);
static int64_t get_quotient(uint32_t *n2);
static int64_t adjust_quotient(
	uint32_t dvsor_msd, uint32_t *estimate, uint32_t rem_msd,
	int64_t quotient_tmp);

/**
 * infiX_division - divides a numbers stored in an array
 * @n1: dividend
 * @n2: divisor
 *
 * Description: this function will store the remainder of any division
 * in a global variable "remains". This variable should be freed after
 * this function has returned.
 *
 * Return: array with the result, NULL on failure
 */
u4b_array *infiX_division(u4b_array *n1, u4b_array *n2)
{
	size_t len_quotient = 0, len_rem = 0;
	size_t n1_i = 0, rem_i = 0, quot_i = 0;
	u4b_array *quotient = NULL;
	int64_t hold = 0;

	if (!n1 || !n2)
		return (NULL);

	remains = NULL;
	if (n1->is_negative || n2->is_negative)
		return (divide_negatives(n1, n2));

	if (check_division_by_0(n2))
		return (NULL);

	hold = check_0_result(n1, n2);
	if (hold < 0)
		return (NULL);

	if (hold > 0)
	{
		quotient = alloc_u4b_array(1);
		if (!quotient)
			free_u4b_array(remains);

		return (quotient);
	}

	len_quotient = (n1->len - n2->len) + 1;
	quotient = alloc_u4b_array(len_quotient);
	if (!quotient)
		return (NULL);

	/*Length of remains will never be greater than n2->len*/
	remains = alloc_u4b_array(n2->len);
	if (!remains)
	{
		free_u4b_array(quotient);
		return (NULL);
	}

	memmove(remains->array, n1->array, n1->len);
	// if (remains->array[remains->len] < n2->array[n2->len - 1])
	// {
	// 	if (!mplug_num_low(&remains, n1[n1_i]))
	// 	{
	// 		free_n_null(quotient);
	// 		return (NULL);
	// 	}

	// 	n1_i--;
	// }

	for (quot_i = len_quotient; quot_i > 0; quot_i--)
	{
		len_rem = remains->len;
		for (; quot_i && n1_i && len_rem < n2->len; quot_i--, n1_i--, len_rem++)
		{
			if (!mplug_num_low(&remains, n1[n1_i]) || !mplug_num_low(&quotient, 0))
			{
				free_n_null(quotient);
				return (NULL);
			}
		}

		hold = get_quotient(n2);
		if (hold < 0 || !mplug_num_low(&quotient, hold))
		{
			free_n_null(quotient);
			return (NULL);
		}

		if (n1_i > 0)
		{
			if (!mplug_num_low(&remains, n1[n1_i]))
				return (NULL);
		}
		else
			break;

		n1_i--;
	}

	trim_u4b_array(remains);
	trim_u4b_array(quotient);
	return (quotient);
}

/**
 * divide_negatives - division of signed numbers (negative numbers).
 * @n1: dividend.
 * @n2: divisor.
 *
 * Return: pointer to the results, NULL on failure.
 */
u4b_array *divide_negatives(u4b_array *n1, u4b_array *n2)
{
	u4b_array *result = NULL;

	if (!n1 || !n2)
		return NULL;

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

	if (!result)
	{
		free_u4b_array(remains);
		return (NULL);
	}

	return (result);
}

/**
 * check_division_by_0 - checks if the divisor is zero.
 * @n2: divisor
 *
 * Return: 1 if n2 is zero, else 0
 */
int check_division_by_0(u4b_array *n2)
{
	if (!n2)
		return (0);

	trim_u4b_array(n2);
	if ((n2->len == 1 && n2->array[0] == 0) || n2->len == 0)
		return (1);

	return (0);
}

/**
 * check_0_result - checks if n1 < n2.
 * @n1: dividend
 * @n2: divisor
 *
 * Description: If n1 < n2 then; quotient = 0 and remains = n1.
 * If n2 == 0 function will just return 0.
 *
 * Return: 1 if result will be 0, 0 if not, -1 on error.
 */
int check_0_result(u4b_array *n1, u4b_array *n2)
{
	size_t i = 0;
	u4b_array *quotient = NULL;

	if (!n1 || !n2)
		return (-1);

	remains = NULL;
	trim_u4b_array(n1);
	trim_u4b_array(n2);
	if (
		n1->len > n2->len ||
		n1->array == NULL ||
		n2->array == NULL ||
		(n2->len == 1 && n2->array[0] == 0))
	{
		return (0);
	}

	if (n1->len < n2->len)
	{
		if (n1->len == 1 && n1->array[0] == 0)
			i = 0;
		else
			i = n1->len;
	}
	else if (n1->len == n2->len)
	{
		/*Determine smaller number.*/
		for (i = n1->len - 1; i > 0; i--)
		{
			if (n1->array[i] > n2->array[i])
				return (0);

			if (n1->array[i] < n2->array[i])
				break;
		}
	}

	if (i > 0)
		remains = alloc_u4b_array(n1->len);
	else
		remains = alloc_u4b_array(1);

	if (!remains)
		return (-1);

	if (i > 0)
		memmove(remains->array, n1->array, n1->len);

	return (1);
}

/**
 * get_quotient - calculates the quotient of number currently in "remains"
 * @n2: n2 array
 *
 * Description: the current n1 is stored in the global array "remains"
 * The function will try to estimate the precise quotient for the number
 * currently stored in "remains". The estimates may overshoot and undershoot
 * and the function will use the difference to make the next estimate and
 * therefore will oscilate closer and closer to the answer.
 *
 * Return: the quotient, -1 on failure
 */
int64_t get_quotient(u4b_array *n2)
{
	u4b_array *rem_tmp = NULL, *mul_est = NULL;
	uint32_t blank[] = {0};
	u4b_array quot_tmp = {.len = 1, .is_negative = 0, .array = blank};
	int64_t hold = 0;

	if (!n2)
		return (-1);

	/**
	 * Compare most significant digits of dividend and divisor.
	 * If divisor's >= dividend's borrow an extra digit.
	 */
	hold = remains->array[remains->len - 1];
	if (remains->len > n2->len)
		hold = (hold * MAX_VAL_u4b) + remains->array[remains->len - 2];

	/**
	 * Divided those most significant digits.
	 * If result >= 1 then; return
	 * else; 
	*/
	quot_tmp.array[0] = hold / n2->array[n2->len - 1];
	while (!rem_tmp ||
		   ((rem_tmp->len > n2->len ||
			 (rem_tmp->len == n2->len && rem_tmp->array[rem_tmp->len - 1] >= n2->array[n2->len - 1])) &&
			quot_tmp.array[0] > 0))
	{
		if (rem_tmp && ((rem_tmp->len >= n2->len) || (rem_tmp->array[rem_tmp->len - 1] & NEGBIT_u4b)))
		{
			hold = adjust_quotient(n2->array[n2->len - 1], mul_est, rem_tmp->array[rem_tmp->len - 1], quot_tmp.array[0]);
			if (hold < 0)
			{
				free_n_null(rem_tmp);
				free_n_null(mul_est);
				return (-1);
			}

			quot_tmp.array[0] = hold;
		}

		free_n_null(mul_est);
		mul_est = NULL;
		free_n_null(rem_tmp);
		rem_tmp = NULL;
		mul_est = infiX_multiplication(n2, quot_tmp);
		if (mul_est)
		{
			rem_tmp = infiX_subtraction(remains, mul_est);
			if (!rem_tmp)
				return (-1);
		}
		else
			return (-1);
	}

	memmove(remains, rem_tmp, ((rem_tmp->len + 1) * sizeof(*rem_tmp)));
	free_n_null(rem_tmp);
	free_n_null(mul_est);
	return (quot_tmp.array[0]);
}

/**
 * adjust_quotient - check for overshoot or undershoot in quotient and adjust appropriately
 * @dvsor_msd: number in the highest index of the n2 array
 * @estimate: multiplied estimate
 * @rem_msd: number in the highest index of the temporary remainder array
 * @quotient_tmp: current quotient to be adjuststed
 *
 * Return: the adjusted quotient, -1 on failure
 */
int64_t adjust_quotient(uint32_t dvsor_msd, uint32_t *estimate,
						uint32_t rem_msd, int64_t quotient_tmp)
{
	uint32_t *tmp_sub = NULL;
	int64_t hold = 0, o_shoot = 0;

	if (rem_msd & NEGBIT_u4b)
	{ /*Decrease the quotient*/
		tmp_sub = infiX_subtraction(estimate, remains);
		if (!tmp_sub)
			return (-1);

		hold = tmp_sub[tmp_sub[0]];
		/*How many of dvsor_msd can fit in hold?*/
		if (hold < dvsor_msd)
			o_shoot = 1;
		else
			o_shoot = hold / (int64_t)dvsor_msd;

		free_n_null(tmp_sub);
		quotient_tmp -= o_shoot;
	}
	else
	{ /*Increase the quotient*/
		hold = rem_msd;
		/*How many of dvsor_msd can fit in hold?*/
		o_shoot = hold / (int64_t)dvsor_msd;
		quotient_tmp += o_shoot;
	}

	return (quotient_tmp);
}
