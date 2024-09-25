#ifndef BIGNUM_MATH_H
#define BIGNUM_MATH_H

#include "macros.h"

#include <limits.h>	 /* type_max */
#include <stdbool.h> /* bool */
#include <stddef.h>	 /* size_t, ptr_diff */
#include <stdint.h>	 /* fixed width types */
#include <stdio.h>	 /* *printf */
#include <string.h>	 /* memset */

#include "xalloc.h"

#ifdef UINT64_MAX

/*Maximum value of a uint in bignum.*/
#define BIGNUM_UINT_MAX (1000000000)

typedef uint32_t uint;
typedef int64_t lint;
typedef uint64_t ulint;

#else

/*Maximum value of a uint in bignum.*/
#define BIGNUM_UINT_MAX (10000)

typedef uint16_t uint;
typedef int32_t lint;
typedef uint32_t ulint;

#endif /*UINT64_MAX*/

/**
 * struct bignum - an array of unsigned 4 byte ints.
 * @len: number of items in the array.
 * @is_negative: a bool for signedness of the number.
 * @num: pointer to an array of unsigned 4 byte ints.
 */
typedef struct bignum
{
	size_t len;
	bool is_negative;
	uint *num;
} bignum;

/* alloc_funcs */

void *bn_free(bignum *freeable_ptr);
ATTR_MALLOC ATTR_MALLOC_FREE(bn_free)
bignum *bn_alloc(size_t len);
bool bn_realloc(bignum *bn, size_t len);
ATTR_MALLOC ATTR_MALLOC_FREE(bn_free)
bignum *bn_dup(bignum *bn);

/*array_funcs*/

lint cmp_bignum(bignum *const a1, bignum *const a2);
lint cmp_rev_uint32array(
	uint const *const arr1, uint const *const arr2, size_t len);
bool is_zero(bignum *const x);
void trim_bignum(bignum *const arr);

/*math_funcs*/

bool bn_add_inplace(bignum *n1, bignum *n2);
bignum *bn_addition(bignum *n1, bignum *n2);
bignum *bn_division(bignum *n1, bignum *n2);
bignum *bn_modulus(bignum *n1, bignum *n2);
bignum *bn_multiplication(bignum *n1, bignum *n2);
bignum *bn_power(bignum *base, bignum *exponent);
bool bn_sub_inplace(bignum *n1, bignum *n2);
bignum *bn_subtraction(bignum *n1, bignum *n2);

bool bn_swap(bignum *n1, bignum *n2);

#endif /* BIGNUM_MATH_H */
