/*!
 * @file
 * @brief methods for deallocating Bigint types.
 */

#include "_bi_internals.h"
#include "Bigint.h"

/*!
 * @brief free a `Bigint`, return NULL.
 * @public @memberof Bigint
 *
 * @param[in] ptr a pointer to a `Bigint`.
 *
 * @return NULL always.
 */
void *bi_delete(Bigint *const ptr) { return (_bi_free(ptr)); }
