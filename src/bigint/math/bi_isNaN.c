/*!
 * @file
 * @brief methods for testing if a bigint is Not a Number (NaN).
 */

#include "_bi_internals.h"
#include "bigint.h"

/*!
 * @brief check if a `bigint` is Not a Number.
 * @public @memberof bigint
 *
 * @param[in] n pointer to the bigint.
 *
 * @return true if the bigint is NaN, false otherwise.
 */
bool bi_isNaN(bigint *const n)
{
	if (!n)
		return (false);

	return (_bi_isNaN_const(_bi_trim(n)));
}

/*!
 * @brief check if a const `bigint` is Not a Number.
 * @protected @memberof bigint
 *
 * @param[in] n pointer to the `bigint`.
 *
 * @return true if the bigint is NaN, false otherwise.
 */
bool _bi_isNaN_const(const bigint *const n)
{
	if (!n)
		return (false);

	return (n->len == 0 && !n->num);
}
