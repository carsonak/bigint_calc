#include "bignum_utils.h"

/**
 * bn_compare - compare 2 bignums.
 * @a1: the first array.
 * @a2: the second array.
 *
 * Return: +ve number if a1 > a2, -ve number if a1 < a2 else 0.
 */
lint bn_compare(bignum * const a1, bignum * const a2)
{
	if (!a1 || !a2)
		return (0);

	trim_bignum(a1);
	trim_bignum(a2);
	if (a1->is_negative && a2->is_negative)
	{
		if ((lint)a2->len - a1->len)
			return ((lint)a2->len - a1->len);

		return (reverse_cmp_uint32array(a2->num, a1->num, a1->len));
	}
	else if (a1->is_negative)
		return (-1);
	else if (a2->is_negative)
		return (1);

	if ((lint)a1->len - a2->len)
		return ((lint)a1->len - a2->len);

	return (reverse_cmp_uint32array(a1->num, a2->num, a1->len));
}

/**
 * reverse_cmp_uint32array - compare int arrays in reverse.
 * @arr1: first array.
 * @arr2: second array.
 * @len: total items to compare.
 *
 * Return: +ve number if arr1 > arr2, -ve number if arr1 < arr2 else 0.
 */
lint reverse_cmp_uint32array(
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
