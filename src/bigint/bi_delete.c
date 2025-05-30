/*!
 * @file
 * @brief methods for deallocating bigint types.
 */

#include "_bi_internals.h"
#include "bigint.h"

/*!
 * @brief free a `bigint`, return NULL.
 * @public @memberof bigint
 *
 * @param[in] ptr a pointer to a `bigint`.
 *
 * @return NULL always.
 */
void *bi_delete(bigint *const ptr) { return (_bi_free(ptr)); }
