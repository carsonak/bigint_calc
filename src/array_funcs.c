#include "infiX.h"

/**
 * cmp_bignum - compare 2 bignums.
 * @a1: the first array.
 * @a2: the second array.
 *
 * Return: +ve number if a1 > a2, -ve number if a1 < a2 else 0.
 */
long int cmp_bignum(BigNum * const a1, BigNum * const a2)
{
	if (!a1 || !a2)
		return (0);

	trim_bignum(a1);
	trim_bignum(a2);
	if (a1->is_negative && a2->is_negative)
	{
		if ((long int)a2->len - a1->len)
			return ((long int)a2->len - a1->len);

		return (cmp_rev_uint32array(a2->num, a1->num, a1->len));
	}
	else if (a1->is_negative)
		return (-1);
	else if (a2->is_negative)
		return (1);

	if ((long int)a1->len - a2->len)
		return ((long int)a1->len - a2->len);

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
long int cmp_rev_uint32array(
	unsigned int const * const arr1,
	unsigned int const * const arr2,
	unsigned long int len)
{
	unsigned long int i = len ? len - 1 : 0;

	if (!arr1 || !arr2)
		return (0);

	for (; i > 0; i--)
	{
		if ((long int)arr1[i] - arr2[i])
			break;
	}

	return ((long int)arr1[i] - arr2[i]);
}

/**
 * trim_bignum - truncate length of a BigNum to ignore trailing zeros.
 * @arr: pointer to a BigNum struct.
 *
 * Also sets the is_negative flag to 0 for 0s and NULL arrays.
 */
void trim_bignum(BigNum * const arr)
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
