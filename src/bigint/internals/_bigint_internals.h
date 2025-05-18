#ifndef BIGINT_INTERNALS_H
#define BIGINT_INTERNALS_H

#include "macros.h"

#include <stdbool.h> /* bool */
#include <string.h>  /* memset, memcpy */

#include "bigint_types.h"
#include "xalloc.h"

/**
 * struct `bigint` - an arbitrary precision integer representation.
 * @len: number of items in the int array.
 * @num: pointer to an array of unsigned ints.
 * @is_negative: a bool for signedness of the number.
 */
struct bigint
{
	len_type len;
	bool is_negative;
	u_int *restrict num;
};

/* memory manipulation functions */

void *_bi_free(bigint *const freeable_ptr);

ATTR_MALLOC ATTR_MALLOC_FREE(_bi_free)
bigint *_bi_alloc(const len_type len);
ATTR_MALLOC ATTR_MALLOC_FREE(_bi_free)
bigint *_bi_dup(bigint const *const restrict bn);
bigint *_bi_resize(bigint *bi, const len_type len);

/* utilities */

bool _bi_isNaN_const(const bigint *const n);
void _bi_trim(bigint *const arr);
intmax_t _bi_compare_int_const(const bigint *const n1, const intmax_t n2);
intmax_t _bi_compare_const(const bigint *const n1, const bigint *const n2);
intmax_t _cmp_rev_uint_arr(
	u_int const *const arr1, u_int const *const arr2, len_type len
);
bigint *_bi_move(bigint *const dest, bigint const *const src);

#endif /* BIGINT_INTERNALS_H */
