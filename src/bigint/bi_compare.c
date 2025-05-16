#include "_bigint_internals.h"
#include "bigint.h"

/**
 * bi_compare_int - compare a bigint to an int.
 * @n1: the `bigint`.
 * @n2: the int.
 *
 * Return: +ve number if n1 > n2, -ve number if n1 < n2 else 0.
 */
intmax_t bi_compare_int(bigint *const n1, const intmax_t n2)
{
	if (!n1 || n1->len < 0)
		return (0);

	_bi_trim(n1);
	return (_bi_compare_int_const(n1, n2));
}

/**
 * bi_compare_int - compare a const bigint to a const int.
 * @n1: the `bigint`.
 * @n2: the int.
 *
 * Return: +ve number if n1 > n2, -ve number if n1 < n2 else 0.
 */
intmax_t _bi_compare_int_const(const bigint *const n1, const intmax_t n2)
{
	if (!n1 || n1->len < 0)
		return (0);

	bigint num2 = {.len = 4, .is_negative = n2 < 0, .num = (u_int[4]){0}};

	int_to_bi(&num2, n2);
	return (_bi_compare_const(n1, &num2));
}

/**
 * bi_compare - compare 2 bigints.
 * @n1: the first `bigint`.
 * @n2: the second `bigint`.
 *
 * Return: +ve number if n1 > n2, -ve number if n1 < n2 else 0.
 */
intmax_t bi_compare(bigint *const n1, bigint *const n2)
{
	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (0);

	_bi_trim(n1);
	_bi_trim(n2);
	return (_bi_compare_const(n1, n2));
}

/**
 * bi_compare_const - compare 2 const bigints.
 * @n1: the first `bigint`.
 * @n2: the second `bigint`.
 *
 * Return: +ve number if n1 > n2, -ve number if n1 < n2 else 0.
 */
intmax_t _bi_compare_const(const bigint *const n1, const bigint *const n2)
{
	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (0);

	if (!n1->num || !n2->num)
		return (0);

	if (n1->is_negative && n2->is_negative)
	{
		if (n2->len - n1->len)
			return (n2->len - n1->len);

		return (_cmp_rev_uint_arr(n2->num, n1->num, n1->len));
	}

	if (n1->is_negative)
		return (-1);

	if (n2->is_negative)
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
	u_int const *const arr1, u_int const *const arr2, len_typ len
)
{
	if ((!arr1 && !arr2) || len < 0)
		return (0);

	len_typ i = len > 0 ? len - 1 : 0;

	if (!arr1)
		return ((intmax_t)0 - arr2[i]);

	if (!arr2)
		return (arr1[i]);

	for (; i > 0; i--)
	{
		if ((intmax_t)arr1[i] - arr2[i])
			break;
	}

	return ((intmax_t)arr1[i] - arr2[i]);
}
