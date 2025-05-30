/*!
 * @file
 * @brief methods for comparing bigint types.
 */

#include "_bi_internals.h"
#include "bigint.h"

/*!
 * @brief compare a `bigint` to an int.
 * @public @memberof bigint
 *
 * @param[in] n1 the `bigint`.
 * @param[in] n2 the int.
 *
 * @return +ve number if n1 > n2, -ve number if n1 < n2 else 0.
 */
l_int bi_compare_int(bigint *const n1, const intmax_t n2)
{
	if (!n1 || n1->len < 0)
		return (0);

	return (_bi_compare_int_const(_bi_trim(n1), n2));
}

/*!
 * @brief compare a const `bigint` to a const int.
 * @protected @memberof bigint
 *
 * @param[in] n1 the `bigint`.
 * @param[in] n2 the int.
 *
 * @return +ve number if n1 > n2, -ve number if n1 < n2 else 0.
 */
l_int _bi_compare_int_const(const bigint *const n1, const intmax_t n2)
{
	if (!n1 || n1->len < 0)
		return (0);

	bigint num2 = {.len = 4, .is_negative = n2 < 0, .num = (u_int[4]){0}};

	return (_bi_compare_const(n1, int_to_bi(&num2, n2)));
}

/*!
 * @brief compare 2 `bigint`s.
 * @public @memberof bigint
 *
 * @param[in] n1 the first `bigint`.
 * @param[in] n2 the second `bigint`.
 *
 * @return +ve number if n1 > n2, -ve number if n1 < n2 else 0.
 */
l_int bi_compare(bigint *const n1, bigint *const n2)
{
	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (0);

	return (_bi_compare_const(_bi_trim(n1), _bi_trim(n2)));
}

/*!
 * @brief compare 2 const `bigint`s.
 * @protected @memberof bigint
 *
 * @param[in] n1 the first `bigint`.
 * @param[in] n2 the second `bigint`.
 *
 * @return +ve number if n1 > n2, -ve number if n1 < n2 else 0.
 */
l_int _bi_compare_const(const bigint *const n1, const bigint *const n2)
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

/*!
 * @brief compare int arrays in reverse.
 * @protected @memberof bigint
 *
 * @param[in] arr1 first array.
 * @param[in] arr2 second array.
 * @param[in] len total items to compare.
 *
 * @return +ve number if arr1 > arr2, -ve number if arr1 < arr2 else 0.
 */
l_int _cmp_rev_uint_arr(
	u_int const *const arr1, u_int const *const arr2, len_type len
)
{
	if ((!arr1 && !arr2) || len < 0)
		return (0);

	len_type i = len > 0 ? len - 1 : 0;

	if (!arr1)
		return ((l_int)0 - arr2[i]);

	if (!arr2)
		return (arr1[i]);

	for (; i > 0; i--)
	{
		if ((l_int)arr1[i] - arr2[i])
			break;
	}

	return ((l_int)arr1[i] - arr2[i]);
}
