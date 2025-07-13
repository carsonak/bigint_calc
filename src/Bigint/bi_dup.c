/*!
 * @file
 * @brief methods for duplicating Bigint types.
 */

#include "_bi_internals.h"
#include "Bigint.h"

/*!
 * @brief duplicates a `Bigint`.
 * @public @memberof Bigint
 *
 * @param[in] n pointer to the `Bigint` to duplicate.
 *
 * @return pointer to the duplicated int, NULL on failure.
 */
Bigint *bi_dup(Bigint *const n)
{
	if (!n)
		return (NULL);

	return (_bi_dup(_bi_trim(n)));
}
