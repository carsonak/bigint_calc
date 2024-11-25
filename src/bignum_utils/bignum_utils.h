#ifndef BIGNUM_UTILS_H
#define BIGNUM_UTILS_H

#include "macros.h"

#include <limits.h>	 /* type_max */
#include <stdbool.h> /* bool */
#include <stddef.h>	 /* size_t, ptr_diff */
#include <stdint.h>	 /* fixed width types */
#include <string.h>	 /* memset */

#include "xalloc.h"
#include "bignum.h"

/*memory allocation functions*/

void *bni_free(bignum_i *freeable_ptr);

ATTR_MALLOC ATTR_MALLOC_FREE(bni_free)
bignum_i *bni_alloc(size_t len);
ATTR_MALLOC ATTR_MALLOC_FREE(bni_free)
bignum_i *bn_dup(bignum_i *bn);
bool bn_realloc(bignum_i *bn, size_t len);

/*utilities*/

bool bni_copy(bignum_i *src, bignum_i *dest);
l_int bni_compare(bignum_i *const a1, bignum_i *const a2);
bool bni_deepcopy(bignum_i *src, bignum_i *dest);
bool bni_swap(bignum_i *const n1, bignum_i *const n2);
bool int_to_bni(bignum_i *dest, long long int n);
bignum_i *int_to_new_bni(long long int n);
bool bni_is_zero(bignum_i *const x);
l_int reverse_cmp_uint32array(
	u_int const *const arr1, u_int const *const arr2, size_t len);
void trim_bni(bignum_i *const arr);

#endif /* BIGNUM_UTILS_H */
