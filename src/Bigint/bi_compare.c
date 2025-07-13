/*!
 * @file
 * @brief methods for comparing Bigint types.
 */

#include "_bi_internals.h"
#include "Bigint.h"

/*!
 * @brief compare a `Bigint` to an int.
 * @public @memberof Bigint
 *
 * @param[in] n1 the `Bigint`.
 * @param[in] n2 the int.
 *
 * @return +ve number if n1 > n2, -ve number if n1 < n2 else 0.
 */
ldigit_ty bi_compare_int(Bigint *const n1, const intmax_t n2)
{
	if (!n1 || n1->len < 0)
		return (0);

	return (_bi_compare_int_const(_bi_trim(n1), n2));
}

/*!
 * @brief compare a const `Bigint` to a const int.
 * @protected @memberof Bigint
 *
 * @param[in] n1 the `Bigint`.
 * @param[in] n2 the int.
 *
 * @return +ve number if n1 > n2, -ve number if n1 < n2 else 0.
 */
ldigit_ty _bi_compare_int_const(const Bigint *const n1, const intmax_t n2)
{
	if (!n1 || n1->len < 0)
		return (0);

	Bigint num2 = {.len = 4, .is_negative = n2 < 0, .num = (udigit_ty[6]){0}};

	return (_bi_compare_const(n1, int_to_bi(&num2, n2)));
}

/*!
 * @brief compare 2 `Bigint`s.
 * @public @memberof Bigint
 *
 * @param[in] n1 the first `Bigint`.
 * @param[in] n2 the second `Bigint`.
 *
 * @return +ve number if n1 > n2, -ve number if n1 < n2 else 0.
 */
ldigit_ty bi_compare(Bigint *const n1, Bigint *const n2)
{
	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (0);

	return (_bi_compare_const(_bi_trim(n1), _bi_trim(n2)));
}

/*!
 * @brief compare 2 const `Bigint`s.
 * @protected @memberof Bigint
 *
 * @param[in] n1 the first `Bigint`.
 * @param[in] n2 the second `Bigint`.
 *
 * @return +ve number if n1 > n2, -ve number if n1 < n2 else 0.
 */
ldigit_ty _bi_compare_const(const Bigint *const n1, const Bigint *const n2)
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
 * @protected @memberof Bigint
 *
 * @param[in] arr1 first array.
 * @param[in] arr2 second array.
 * @param[in] len total items to compare.
 *
 * @return +ve number if arr1 > arr2, -ve number if arr1 < arr2 else 0.
 */
ldigit_ty _cmp_rev_uint_arr(
	udigit_ty const *const arr1, udigit_ty const *const arr2, len_ty len
)
{
	if ((!arr1 && !arr2) || len < 0)
		return (0);

	len_ty i = len > 0 ? len - 1 : 0;

	if (!arr1)
		return ((ldigit_ty)0 - arr2[i]);

	if (!arr2)
		return (arr1[i]);

	for (; i > 0; i--)
	{
		if ((ldigit_ty)arr1[i] - arr2[i])
			break;
	}

	return ((ldigit_ty)arr1[i] - arr2[i]);
}
