#ifndef BIGNUM_MATH_H
#define BIGNUM_MATH_H

#include "macros.h"

#include <limits.h>	 /* type_max */
#include <stdbool.h> /* bool */
#include <stddef.h>	 /* size_t, ptr_diff */
#include <stdint.h>	 /* fixed width types */
#include <string.h>	 /* memset */

#include "bignum.h"
#include "bignum_utils.h"

bool bni_iadd(bignum_i *const n1, bignum_i *const n2);
bignum_i *bni_add(bignum_i *const n1, bignum_i *const n2);
bignum_i *bni_add_int(bignum_i *const n1, long long int n2);
bool bni_iadd_int(bignum_i *const n1, long long int n2);
bignum_i *bni_divide(bignum_i *const n1, bignum_i *const n2);
bignum_i *bni_modulo(bignum_i *const n1, bignum_i *const n2);
bignum_i *bni_multiply(bignum_i *const n1, bignum_i *const n2);
bignum_i *bni_power(bignum_i *base, bignum_i *exponent);
bool bni_isubtract(bignum_i *const n1, bignum_i *const n2);
bignum_i *bni_subtract(bignum_i *const n1, bignum_i *const n2);
bignum_i *bni_subtract_int(bignum_i *const n1, long long int n2);
bool bni_isubtract_int(bignum_i *const n1, long long int n2);

#endif /* BIGNUM_MATH_H */
