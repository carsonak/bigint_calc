#include "bignum_utils.h"

/**
 * bn_compare - compare 2 bignums.
 * @n1: the first array.
 * @n2: the second array.
 *
 * Return: +ve number if n1 > n2, -ve number if n1 < n2 else 0.
 */
l_int bn_compare(bignum * const n1, bignum * const n2)
{
	if (!n1 || !n2)
		return (0);

	trim_bignum(n1);
	trim_bignum(n2);
	if (n1->is_negative && n2->is_negative)
	{
		/*CAUTION: possible case => len truncated when casting to l_int.*/
		if ((l_int)n2->len - n1->len)
			return ((l_int)n2->len - n1->len);

		return (reverse_cmp_uint32array(n2->num, n1->num, n1->len));
	}
	else if (n1->is_negative)
		return (-1);
	else if (n2->is_negative)
		return (1);

	if ((l_int)n1->len - n2->len)
		return ((l_int)n1->len - n2->len);

	return (reverse_cmp_uint32array(n1->num, n2->num, n1->len));
}

/**
 * reverse_cmp_uint32array - compare int arrays in reverse.
 * @arr1: first array.
 * @arr2: second array.
 * @len: total items to compare.
 *
 * Return: +ve number if arr1 > arr2, -ve number if arr1 < arr2 else 0.
 */
l_int reverse_cmp_uint32array(
	u_int const *const arr1,
	u_int const *const arr2,
	size_t len)
{
	size_t i = len ? len - 1 : 0;

	if (!arr1 || !arr2)
		return (0);

	for (; i > 0; i--)
	{
		if ((l_int)arr1[i] - arr2[i])
			break;
	}

	return ((l_int)arr1[i] - arr2[i]);
}
