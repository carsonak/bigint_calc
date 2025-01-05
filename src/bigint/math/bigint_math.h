#ifndef bigint_MATH_H
#define bigint_MATH_H

#include "macros.h"

#include <stdbool.h> /* bool */
#include <stddef.h>  /* size_t, ptr_diff */
#include <stdint.h>  /* fixed width types */
#include <string.h>  /* memset */

#include "bigint.h"
#include "bigint_typedefs.h"

bool int_to_bi(bigint *dest, long long int n);
bigint *int_to_new_bi(long long int n);

bool bi_iadd(bigint *const n1, bigint *const n2);
bigint *bi_add(bigint *const n1, bigint *const n2);
bigint *bi_add_int(bigint *const n1, long long int n2);
bool bi_iadd_int(bigint *const n1, long long int n2);
bigint *bi_divide(bigint *const n1, bigint *const n2);
bigint *bi_modulo(bigint *const n1, bigint *const n2);
bigint *bi_multiply(bigint *const n1, bigint *const n2);
bigint *bi_power(bigint *base, bigint *exponent);
bool bi_isubtract(bigint *const n1, bigint *const n2);
bigint *bi_subtract(bigint *const n1, bigint *const n2);
bigint *bi_subtract_int(bigint *const n1, long long int n2);
bool bi_isubtract_int(bigint *const n1, long long int n2);

#endif /* bigint_MATH_H */
