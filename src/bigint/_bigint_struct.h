#ifndef BIGINT_STRUCT_H
#define BIGINT_STRUCT_H

#include "macros.h"

#include <stdbool.h> /* bool */
#include <stddef.h>  /* size_t */

#include "bigint_typedefs.h"
#include "xalloc.h"

/**
 * struct bigint - an arbitrary precision integer representation.
 * @len: number of items in the int array.
 * @is_negative: a bool for signedness of the number.
 * @num: pointer to an array of unsigned ints.
 */
struct bigint
{
	size_t len;
	bool is_negative;
	u_int *num;
};

/* memory manipulation functions */

void *bi_delete(bigint *const freeable_ptr);

ATTR_MALLOC ATTR_MALLOC_FREE(bi_delete)
bigint *_bi_alloc(const size_t len);
ATTR_MALLOC ATTR_MALLOC_FREE(bi_delete)
bigint *bi_dup(bigint const *const bn);
bigint *_bi_resize(bigint *bi, const size_t len);

/* utilities */

void _bi_trim(bigint *const arr);
l_int _cmp_rev_uint32_arr(
	u_int const *const arr1, u_int const *const arr2, size_t len);

#endif /*BIGINT_STRUCT_H*/
