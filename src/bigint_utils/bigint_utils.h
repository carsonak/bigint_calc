#ifndef bigint_UTILS_H
#define bigint_UTILS_H

#include "macros.h"

#include <limits.h>	 /* type_max */
#include <stdbool.h> /* bool */
#include <stddef.h>	 /* size_t, ptr_diff */
#include <stdint.h>	 /* fixed width types */
#include <string.h>	 /* memset */

#include "xalloc.h"
#include "bigint.h"

/*memory allocation functions*/

void *bi_free(bigint *freeable_ptr);

ATTR_MALLOC ATTR_MALLOC_FREE(bi_free)
bigint *bi_alloc(size_t len);
ATTR_MALLOC ATTR_MALLOC_FREE(bi_free)
bigint *bi_dup(bigint *bn);
bool bi_realloc(bigint *bn, size_t len);

/*utilities*/

bool bi_copy(bigint *src, bigint *dest);
l_int bi_compare(bigint *const a1, bigint *const a2);
bool bi_deepcopy(bigint *src, bigint *dest);
bool bi_swap(bigint *const n1, bigint *const n2);
bool int_to_bni(bigint *dest, long long int n);
bigint *int_to_new_bni(long long int n);
bool bi_is_zero(bigint *const x);
l_int reverse_cmp_uint32array(
	u_int const *const arr1, u_int const *const arr2, size_t len);
void bi_trim(bigint *const arr);

#endif /* bigint_UTILS_H */
