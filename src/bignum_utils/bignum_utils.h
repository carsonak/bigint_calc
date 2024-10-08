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

void *bn_free(bignum *freeable_ptr);

ATTR_MALLOC ATTR_MALLOC_FREE(bn_free)
bignum *bn_alloc(size_t len);
ATTR_MALLOC ATTR_MALLOC_FREE(bn_free)
bignum *bn_dup(bignum *bn);
bool bn_realloc(bignum *bn, size_t len);

/*utilities*/

bool bn_copy(bignum *src, bignum *dest);
l_int bn_compare(bignum *const a1, bignum *const a2);
bool bn_deepcopy(bignum *src, bignum *dest);
bool bn_swap(bignum *const n1, bignum *const n2);
bool int_to_bignum(bignum *dest, long long int n);
bignum *int_to_bignuma(long long int n);
bool is_zero(bignum *const x);
l_int reverse_cmp_uint32array(
	u_int const *const arr1, u_int const *const arr2, size_t len);
void trim_bignum(bignum *const arr);

#endif /* BIGNUM_UTILS_H */
