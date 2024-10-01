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

bool bn_add_inplace(bignum *n1, bignum *n2);
bignum *bn_addition(bignum *n1, bignum *n2);
bool bn_addint(bignum *n1, long long int n2);
bool bn_addint_inplace(bignum *n1, long long int n2);
bignum *bn_division(bignum *n1, bignum *n2);
bignum *bn_modulus(bignum *n1, bignum *n2);
bignum *bn_multiplication(bignum *n1, bignum *n2);
bignum *bn_power(bignum *base, bignum *exponent);
bool bn_sub_inplace(bignum *n1, bignum *n2);
bignum *bn_subtraction(bignum *n1, bignum *n2);
bool bn_subint(bignum *n1, long long int n2);
bool bn_subint_inplace(bignum *n1, long long int n2);

#endif /* BIGNUM_MATH_H */
