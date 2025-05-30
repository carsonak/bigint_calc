#ifndef BIGINT_INTERNALS_H
#define BIGINT_INTERNALS_H

#include "attribute_macros.h"

#include <stdbool.h> /* bool */

#include "bigint_types.h"
#include "xalloc.h"

/*!
 * @struct bigint
 * @brief a representation of an arbitrary precision integer.
 */
struct bigint
{
	/*! @protected a bool for signedness of the number. */
	bool is_negative : 1;
	/*! @protected number of items in the int array. */
	len_type len : sizeof(len_type) * 8 - 1;
	/*! @protected pointer to an array of unsigned ints. */
	u_int *restrict num;
};

/* memory manipulation */

void *_bi_free(bigint *const freeable_ptr);

bigint *_bi_alloc(const len_type len) ATTR_MALLOC ATTR_MALLOC_FREE(_bi_free);
bigint *_bi_dup(bigint const *const restrict bn) ATTR_MALLOC ATTR_MALLOC_FREE(
	_bi_free
);
bigint *_bi_resize(bigint *bi, const len_type len);

/* utility */

bool _bi_isNaN_const(const bigint *const n);
bigint *_bi_trim(bigint *const restrict n);
l_int _bi_compare_int_const(const bigint *const n1, const intmax_t n2);
l_int _bi_compare_const(const bigint *const n1, const bigint *const n2);
l_int _cmp_rev_uint_arr(
	u_int const *const arr1, u_int const *const arr2, len_type len
);
bigint *_bi_move(bigint *const dest, bigint const *const src);

#endif /* BIGINT_INTERNALS_H */
