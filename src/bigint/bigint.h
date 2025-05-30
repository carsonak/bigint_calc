#ifndef BIGINT_H
#define BIGINT_H

#include "attribute_macros.h"

#include <stdbool.h> /* bool */

#include "bigint_types.h"

/* `bigint` construction */

void *bi_delete(bigint *const ptr);
bigint *bi_new(
	char const *const restrict number, const radix_type base,
	len_type *const restrict processed
) ATTR_MALLOC ATTR_MALLOC_FREE(bi_delete);

bigint *int_to_bi(bigint *const dest, const intmax_t n);
bigint *
int_to_new_bi(const intmax_t n) ATTR_MALLOC ATTR_MALLOC_FREE(bi_delete);

/* `bigint` manipulation */

bigint *bi_dup(bigint *const n);
char *bi_tostr(bigint const *const n);
l_int bi_compare_int(bigint *const n1, const intmax_t n2);
l_int bi_compare(bigint *const n1, bigint *const n2);

/* `bigint` math */

bool bi_iszero(bigint const *const x);
bool bi_isNaN(bigint *const n);

bi_divmod_res bi_divmod(bigint *const restrict n1, bigint *const restrict n2);
bigint *bi_add(bigint *const restrict n1, bigint *const restrict n2);
bigint *bi_divide(bigint *const restrict n1, bigint *const restrict n2);
bigint *bi_modulo(bigint *const restrict n1, bigint *const restrict n2);
bigint *bi_multiply(bigint *const restrict n1, bigint *const restrict n2);
bigint *bi_power(bigint *const restrict base, const intmax_t exponent);
bigint *bi_shift_l(bigint *const restrict n, const len_type c);
bigint *bi_shift_r(bigint *const restrict n, const len_type c);
bigint *bi_subtract(bigint *const restrict n1, bigint *const restrict n2);

bigint *bi_ishift_l(bigint *const restrict n, const len_type c);
bigint *bi_ishift_r(bigint *const restrict n, const len_type c);
bool bi_iadd(bigint *const restrict n1, bigint *const restrict n2);
bool bi_isubtract(bigint *const restrict n1, bigint *const restrict n2);

bigint *bi_add_int(bigint *const n1, const intmax_t n2);
bigint *bi_multiply_int(bigint *const n1, const intmax_t n2);
bigint *bi_subtract_int(bigint *const n1, const intmax_t n2);

bool bi_iadd_int(bigint *const n1, const intmax_t n2);
bool bi_isubtract_int(bigint *const n1, const intmax_t n2);

#endif /* BIGINT_H */
