#include "_bigint_internals.h"
#include "bigint.h"

static ATTR_NONNULL void
isubtract(bigint *const restrict n1, bigint *const restrict n2);
static ATTR_NONNULL bool
isubtract_negatives(bigint *const restrict n1, bigint *const restrict n2);

/**
 * isubtract - subtract two bigints inplace.
 * @n1: first number.
 * @n2: second numbers.
 *
 * Return: pointer to the result, NULL on failure.
 */
static void isubtract(bigint *const restrict n1, bigint *const restrict n2)
{
	len_typ n1_i = 0, n2_i = 0, final_len = 0;
	l_int byt_diff = 0;
	/* result_len = max(n1->len, n2->len) */
	len_typ tmp_len = (n1->len > n2->len) ? n1->len : n2->len;
	bool const n1_is_bigger = (bi_compare(n1, n2) > 0) ? true : false;

	/* If both arrays are of the same length then; */
	/* result_len = n1->len - */
	/* (length of continuous matches in n1 and n2 from msd down to lsd) */
	if (n1->len == n2->len)
		while (tmp_len > 1 && n1->num[tmp_len - 1] == n2->num[tmp_len - 1])
			tmp_len--;

	while (n1_i < tmp_len && (n1_i < n1->len || n2_i < n2->len))
	{
		if (n1_is_bigger == true) /* then; n1 - n2 */
		{
			if (n2_i < n2->len)
				byt_diff += (l_int)n1->num[n1_i] - n2->num[n2_i];
			else
				byt_diff += n1->num[n1_i];
		}
		else /* n2 - n1 */
		{
			if (n1_i < n1->len)
				byt_diff += (l_int)n2->num[n2_i] - n1->num[n1_i];
			else
				byt_diff += n2->num[n2_i];
		}

		if (byt_diff < 0) /* borrow 1 from next. */
		{
			byt_diff += BIGINT_BASE;
			n1->num[n1_i] = byt_diff % BIGINT_BASE;
			byt_diff = -1;
			++final_len;
		}
		else
		{
			/* For cases such as: 1,000,000,000 - 5 = 999,999,995 */
			/* n1 is reduced in length from the original number, therefore */
			/* don't update n1 as the borrow reduces the next "digit" to 0. */
			if (byt_diff || (n2_i + 1 < n2->len || n1_i < n1->len))
			{
				n1->num[n1_i] = byt_diff % BIGINT_BASE;
				++final_len;
			}

			byt_diff = 0;
		}

		++n1_i;
		++n2_i;
	}

	if (n1_is_bigger == false)
		n1->is_negative = true;

	n1->len = final_len;
	_bi_trim(n1);
}

/**
 * isubtract_negatives - handle subtraction of two signed bigints.
 * @n1: first number.
 * @n2: second number.
 *
 * Return: pointer to the result, NULL on failure.
 */
static bool
isubtract_negatives(bigint *const restrict n1, bigint *const restrict n2)
{
	bool neg1 = n1->is_negative, neg2 = n2->is_negative;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2) /* -8 - -5 = -(8-5) */
	{
		isubtract(n1, n2);
		n1->is_negative = !n1->is_negative;
	}
	else if (neg2) /* 8 - -5 = 8+5 */
	{
		if (!bi_iadd(n1, n2))
			return (false);
	}
	else if (neg1)
	{
		/* -8 - 5 = -(8+5) */
		if (!bi_iadd(n1, n2))
			return (false);

		n1->is_negative = !n1->is_negative;
	}

	n2->is_negative = neg2;
	_bi_trim(n1);
	return (true);
}

/**
 * bi_isubtract - handles subtraction of two bigints inplace.
 * @n1: first number, must have enough space to store the result.
 * @n2: second number.
 *
 * The results of the subtraction will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * Return: 1 on success, 0 on failure.
 */
bool bi_isubtract(bigint *const restrict n1, bigint *const restrict n2)
{
	/* n1.num cannot be NULL as this function does not allocate any memory. */
	if (!n1 || !n1->num)
		return (false);

	_bi_trim(n1);
	if (!n2) /* This case is treated as -(n1). */
	{
		n1->is_negative = !n1->is_negative;
		return (true);
	}

	_bi_trim(n2);
	if (bi_isNaN(n1) || bi_isNaN(n2))
		return (false);

	if (n1->is_negative || n2->is_negative)
		return (isubtract_negatives(n1, n2));

	isubtract(n1, n2);
	return (true);
}

/**
 * bi_isubtract_int - subtract an int from a `bigint` inplace.
 * @n1: the first number, must have enough memory allocated to hold the answer.
 * @n2: the second number.
 *
 * The results of the subtraction will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * Return: true on success, false on failure.
 */
bool bi_isubtract_int(bigint *const n1, const intmax_t n2)
{
	bigint num2 = {.len = 4, .is_negative = 0, .num = (u_int[3]){0}};

	if (!n1)
		return (false);

	int_to_bi(&num2, n2);
	return (bi_isubtract(n1, &num2));
}
