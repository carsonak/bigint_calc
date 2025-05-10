#include "_bigint_internals.h"
#include "bigint.h"

/**
 * bi_compare - compare 2 bigints.
 * @n1: the first array.
 * @n2: the second array.
 *
 * Return: +ve number if n1 > n2, -ve number if n1 < n2 else 0.
 */
intmax_t bi_compare(bigint *const n1, bigint *const n2)
{
	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (0);

	_bi_trim(n1);
	_bi_trim(n2);
	if (n1->is_negative && n2->is_negative)
	{
		/* WARNING: len might truncated when casted to l_int. */
		if (n2->len - n1->len)
			return (n2->len - n1->len);

		return (_cmp_rev_uint_arr(n2->num, n1->num, n1->len));
	}
	else if (n1->is_negative)
		return (-1);
	else if (n2->is_negative)
		return (1);

	if (n1->len - n2->len)
		return (n1->len - n2->len);

	return (_cmp_rev_uint_arr(n1->num, n2->num, n1->len));
}

/**
 * _cmp_rev_uint_arr - compare int arrays in reverse.
 * @arr1: first array.
 * @arr2: second array.
 * @len: total items to compare.
 *
 * Return: +ve number if arr1 > arr2, -ve number if arr1 < arr2 else 0.
 */
intmax_t _cmp_rev_uint_arr(
	u_int const *const arr1, u_int const *const arr2, intmax_t len
)
{
	if (!arr1 || !arr2 || len < 0)
		return (0);

	intmax_t i = len ? len - 1 : 0;

	for (; i > 0; i--)
	{
		if ((intmax_t)arr1[i] - arr2[i])
			break;
	}

	return ((intmax_t)arr1[i] - arr2[i]);
}
