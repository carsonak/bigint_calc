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

bool bn_add_inplace(bignum * const n1, bignum * const n2);
bignum *bn_add(bignum * const n1, bignum * const n2);
bool bn_add_int(bignum * const n1, long long int n2);
bool bn_addint_inplace(bignum * const n1, long long int n2);
bignum *bn_divide(bignum * const n1, bignum * const n2);
bignum *bn_modulo(bignum * const n1, bignum * const n2);
bignum *bn_multiplication(bignum * const n1, bignum * const n2);
bignum *bn_power(bignum *base, bignum *exponent);
bool bn_sub_inplace(bignum * const n1, bignum * const n2);
bignum *bn_subtract(bignum * const n1, bignum * const n2);
bool bn_subtract_int(bignum * const n1, long long int n2);
bool bn_subint_inplace(bignum * const n1, long long int n2);

#endif /* BIGNUM_MATH_H */
