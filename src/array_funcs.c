#include "infiX.h"

/**
 * cmp_bignum - compare 2 bignums.
 * @a1: the first array.
 * @a2: the second array.
 *
 * Return: +ve number if a1 > a2, -ve number if a1 < a2 else 0.
 */
lint cmp_bignum(bignum * const a1, bignum * const a2)
{
	if (!a1 || !a2)
		return (0);

	trim_bignum(a1);
	trim_bignum(a2);
	if (a1->is_negative && a2->is_negative)
	{
		if ((lint)a2->len - a1->len)
			return ((lint)a2->len - a1->len);

		return (cmp_rev_uint32array(a2->num, a1->num, a1->len));
	}
	else if (a1->is_negative)
		return (-1);
	else if (a2->is_negative)
		return (1);

	if ((lint)a1->len - a2->len)
		return ((lint)a1->len - a2->len);

	return (cmp_rev_uint32array(a1->num, a2->num, a1->len));
}

/**
 * cmp_rev_uint32array - compare int arrays in reverse.
 * @arr1: first array.
 * @arr2: second array.
 * @len: total items to compare.
 *
 * Return: +ve number if arr1 > arr2, -ve number if arr1 < arr2 else 0.
 */
lint cmp_rev_uint32array(
	uint const *const arr1,
	uint const *const arr2,
	size_t len)
{
	size_t i = len ? len - 1 : 0;

	if (!arr1 || !arr2)
		return (0);

	for (; i > 0; i--)
	{
		if ((lint)arr1[i] - arr2[i])
			break;
	}

	return ((lint)arr1[i] - arr2[i]);
}

/**
 * is_zero - check if a bignum is equal to 0.
 * @arr: the bignum to check.
 *
 * Return: true if bignum is NULL or equal to zero, else false.
 */
bool is_zero(bignum * const arr)
{
	trim_bignum(arr);
	if (!arr || !arr->len || (arr->len == 1 && !arr->num[0]))
		return (true);

	return (false);
}

/**
 * trim_bignum - truncate length of a bignum to ignore trailing zeros.
 * @arr: pointer to a bignum struct.
 *
 * Also sets the is_negative flag to 0 for 0s and NULL arrays.
 */
void trim_bignum(bignum * const arr)
{
	if (!arr)
		return;

	if (!arr->num)
		arr->len = 0;

	while (arr->len > 1 && !arr->num[arr->len - 1])
		--arr->len;

	if (arr->len <= 1)
	{
		if (!arr->num || !arr->num[0])
			arr->is_negative = false;
	}
}
