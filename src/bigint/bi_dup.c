/*!
 * @file
 * @brief methods for duplicating bigint types.
 */

#include "_bi_internals.h"
#include "bigint.h"

/*!
 * @brief duplicates a `bigint`.
 * @public @memberof bigint
 *
 * @param[in] n pointer to the `bigint` to duplicate.
 *
 * @return pointer to the duplicated int, NULL on failure.
 */
bigint *bi_dup(bigint *const n)
{
	if (!n)
		return (NULL);

	return (_bi_dup(_bi_trim(n)));
}
