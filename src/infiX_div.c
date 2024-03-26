#include "infiX.h"

#ifdef __cplusplus
extern "C"
{
#endif
	m_uint *remain = NULL;

	static int
	divide_negatives(m_uint *n1_arr, m_uint *n2_arr, m_uint **result);

	static int
	zero_result_check(m_uint *dvdend, m_uint *dvsor, m_uint **quotient);

	static int64_t get_quotient(m_uint *dvsor);

	static int64_t
	adjust_quotient(m_uint dvsor_msd, m_uint *estimate, m_uint rem_msd,
					int64_t quotient_tmp);
#ifdef __cplusplus
}
#endif

/**
 * infiX_division - divides a numbers stored in an array
 * @dividend: dividend
 * @divisor: divisor
 *
 * Description: this function will store the remainder of any division
 * in a global variable "remain". This variable should be freed after
 * this function has returned.
 *
 * Return: array with the result, NULL on failure
 */
m_uint *infiX_division(m_uint *dividend, m_uint *divisor)
{
	size_t len_sor = 0, len_dend = 0, len_rem = 0, len_quotient = 0;
	size_t nd_i = 0, r_i = 0, q_i = 0;
	m_uint *quotient = NULL;
	int64_t hold = 0;

	remain = NULL;
	if (divide_negatives(dividend, divisor, &quotient))
		return (quotient);

	if (dividend)
		len_dend = dividend[0];

	if (divisor)
		len_sor = divisor[0];

	if (zero_result_check(dividend, divisor, &quotient))
		return (quotient);

	len_quotient = (len_dend - len_sor) + 1;
	/*Length of remain will never be greater than len_sor + 1*/
	remain = calloc_check(len_sor + 1, sizeof(*remain));
	if (!remain)
		return (NULL);

	len_rem = len_sor;
	remain[0] = len_rem;
	/*Copy dividend into remain*/
	for (nd_i = len_dend, r_i = len_rem; r_i > 0; nd_i--, r_i--)
		remain[r_i] = dividend[nd_i];

	if (remain[len_rem] < divisor[len_sor])
	{
		if (!mplug_num_low(&remain, dividend[nd_i]))
		{
			free(quotient);
			return (NULL);
		}

		nd_i--;
	}

	for (q_i = len_quotient; q_i > 0; q_i--)
	{
		errno = 0;
		len_rem = remain[0];
		for (; q_i && nd_i && len_rem < len_sor; q_i--, nd_i--, len_rem++)
		{
			if (!mplug_num_low(&remain, dividend[nd_i]) || !mplug_num_low(&quotient, 0))
			{
				free(quotient);
				return (NULL);
			}
		}

		hold = get_quotient(divisor);
		if (hold < 0 || !mplug_num_low(&quotient, hold))
		{
			free(quotient);
			return (NULL);
		}

		if (nd_i > 0)
		{
			if (!mplug_num_low(&remain, dividend[nd_i]))
				return (NULL);
		}
		else
			break;

		nd_i--;
	}

	trim_intarr(remain);
	trim_intarr(quotient);
	return (quotient);
}

/**
 * divide_negatives - division of only signed numbers (negative numbers)
 * @n1_arr: number to be divided
 * @n2_arr: number to divide
 * @result: address of a pointer to store the result
 *
 * Return: 1 if action taken (error or processed results), 0 on failure
 */
int divide_negatives(m_uint *n1_arr, m_uint *n2_arr, m_uint **result)
{
	m_uint a_msd = 0, b_msd = 0;

	if (!result)
	{
		fprintf(stderr, "divide_negatives: Invalid arguments\n");
		return (1);
	}

	trim_intarr(n1_arr);
	trim_intarr(n2_arr);
	if (n1_arr)
		a_msd = n1_arr[n1_arr[0]];

	if (n2_arr)
		b_msd = n2_arr[n2_arr[0]];

	if ((a_msd & NEGBIT_UI32) && (b_msd & NEGBIT_UI32))
	{ /* -8 / -5 = 8/5 */
		n1_arr[n1_arr[0]] ^= NEGBIT_UI32;
		if (n1_arr != n2_arr)
			n2_arr[n2_arr[0]] ^= NEGBIT_UI32;

		(*result) = infiX_division(n1_arr, n2_arr);
	}
	else if (a_msd & NEGBIT_UI32)
	{ /* -8 / 5 = -(8/5) */
		n1_arr[n1_arr[0]] ^= NEGBIT_UI32;
		(*result) = infiX_division(n1_arr, n2_arr);
		(*result)[(*result)[0]] |= NEGBIT_UI32;
	}
	else if (b_msd & NEGBIT_UI32)
	{ /* 8 / -5 = -(8/5) */
		n2_arr[n2_arr[0]] ^= NEGBIT_UI32;
		(*result) = infiX_division(n1_arr, n2_arr);
		(*result)[(*result)[0]] |= NEGBIT_UI32;
	}

	if (*result || errno)
		return (1);

	return (0);
}

/**
 * zero_result_check - checks if dividend < divisor or divisor == 0
 * @dvdend: dividend array
 * @dvsor: divisor array
 * @quotient: address of the quotient array
 *
 * Description: If [dividend < divisor] quotient will be set to 0 and dividend
 * copied into the global variable "remain".
 * If [divisor == 0] function will just return 1
 *
 * Return: 1 if true, 0 if false
 */
int zero_result_check(m_uint *dvdend, m_uint *dvsor, m_uint **quotient)
{
	ssize_t l_dvsor = -1, l_dvdend = -1, nd_i = 0;

	if (!quotient)
	{
		fprintf(stderr, "zero_result_check: Invalid arguments\n");
		return (1);
	}

	if (dvdend)
		l_dvdend = dvdend[0];

	if (dvsor)
		l_dvsor = dvsor[0];

	if (!dvdend || l_dvdend < l_dvsor || (l_dvdend == 1 && dvdend[1] == 0) ||
		(l_dvdend == l_dvsor && dvdend[l_dvdend] < dvsor[l_dvsor]))
	{
		if (l_dvdend < 1)
			l_dvdend = 1;

		*quotient = calloc_check(2, sizeof(**quotient));
		if (*quotient)
			(*quotient)[0] = 1;

		remain = calloc_check(l_dvdend + 1, sizeof(*remain));
		if (remain)
		{
			remain[0] = l_dvdend;
			for (nd_i = 1; dvdend && nd_i <= l_dvdend; nd_i++)
				remain[nd_i] = dvdend[nd_i];
		}

		return (1);
	}
	else if (!dvsor || (l_dvsor == 1 && dvsor[1] == 0))
	{
		panic("division by zero");
		return (1);
	}

	return (0);
}

/**
 * get_quotient - calculates the quotient of number currently in "remain"
 * @dvsor: divisor array
 *
 * Description: the current dividend is stored in the global array "remain"
 * The function will try to estimate the precise quotient for the number
 * currently stored in "remain". The estimates may overshoot and undershoot
 * and the function will use the difference to make the next estimate and
 * therefore will oscilate closer and closer to the answer.
 *
 * Return: the quotient, -1 on failure
 */
int64_t get_quotient(m_uint *dvsor)
{
	m_uint *rem_tmp = NULL, *mul_est = NULL, quot_tmp[] = {1, 0, 0};
	int64_t hold = 0;

	if (!dvsor)
		return (-1);

	hold = remain[remain[0]];
	if (remain[0] > dvsor[0])
		hold = (hold * MID_MAX_VAL) + (int64_t)remain[remain[0] - 1];

	quot_tmp[1] = hold / (int64_t)dvsor[dvsor[0]];
	while (!rem_tmp ||
		   (((rem_tmp[0] > dvsor[0] ||
			  (rem_tmp[0] == dvsor[0] && rem_tmp[rem_tmp[0]] >= dvsor[dvsor[0]]))) &&
			quot_tmp[1] > 0))
	{
		if (rem_tmp && ((rem_tmp[0] >= dvsor[0]) || (rem_tmp[rem_tmp[0]] & NEGBIT_UI32)))
		{
			hold = adjust_quotient(dvsor[dvsor[0]], mul_est, rem_tmp[rem_tmp[0]], quot_tmp[1]);
			if (hold < 0)
			{
				free(rem_tmp);
				free(mul_est);
				return (-1);
			}

			quot_tmp[1] = hold;
		}

		free(mul_est);
		mul_est = NULL;
		free(rem_tmp);
		rem_tmp = NULL;
		mul_est = infiX_multiplication(dvsor, quot_tmp);
		if (mul_est)
		{
			rem_tmp = infiX_subtraction(remain, mul_est);
			if (!rem_tmp)
				return (-1);
		}
		else
			return (-1);
	}

	memmove(remain, rem_tmp, ((rem_tmp[0] + 1) * sizeof(*rem_tmp)));
	free(rem_tmp);
	free(mul_est);
	return (quot_tmp[1]);
}

/**
 * adjust_quotient - check for overshoot or undershoot in quotient and adjust appropriately
 * @dvsor_msd: number in the highest index of the divisor array
 * @estimate: multiplied estimate
 * @rem_msd: number in the highest index of the temporary remainder array
 * @quotient_tmp: current quotient to be adjuststed
 *
 * Return: the adjusted quotient, -1 on failure
 */
int64_t adjust_quotient(m_uint dvsor_msd, m_uint *estimate,
						m_uint rem_msd, int64_t quotient_tmp)
{
	m_uint *tmp_sub = NULL;
	int64_t hold = 0, o_shoot = 0;

	if (rem_msd & NEGBIT_UI32)
	{ /*Decrease the quotient*/
		tmp_sub = infiX_subtraction(estimate, remain);
		if (!tmp_sub)
			return (-1);

		hold = tmp_sub[tmp_sub[0]];
		/*How many of dvsor_msd can fit in hold?*/
		if (hold < dvsor_msd)
			o_shoot = 1;
		else
			o_shoot = hold / (int64_t)dvsor_msd;

		free(tmp_sub);
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
