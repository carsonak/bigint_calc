/*!
 * @file
 * @brief methods to test if a bigint is 0.
 */

#include "_bi_internals.h"
#include "bigint.h"

/*!
 * @brief check if a `bigint` is equal to 0.
 * @public @memberof bigint
 *
 * @param[in] n pointer to the `bigint` to check.
 *
 * @return false if the `bigint`:
 * 		a. has a NULL array.
 * 		b. has len < 1.
 * 		c. is not equal to 0.
 * else false.
 */
bool bi_iszero(bigint const *const n)
{
	len_type i = 0;

	if (!n || !n->num || n->len < 1)
		return (false);

	for (i = n->len - 1; i > 0 && n->num[i] == 0; --i)
		;

	if (n->num[i] == 0)
		return (true);

	return (false);
}
