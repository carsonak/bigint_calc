#ifndef BIGINT_H
#define BIGINT_H

#include "macros.h"

#include <stdbool.h> /* bool */
#include <stdlib.h>  /* strtoull */
#include <string.h>  /* memmove */

#include "bigint_types.h"

/* helper functions */

uintmax_t safe_imaxabs(intmax_t n);

/* `bigint` construction */

void *bi_delete(bigint *const ptr);
ATTR_MALLOC ATTR_MALLOC_FREE(bi_delete)
bigint *bi_new(
	char const *const restrict number, const unsigned short int base,
	len_type *const restrict processed
);

bool int_to_bi(bigint *const dest, const intmax_t n);
ATTR_MALLOC ATTR_MALLOC_FREE(bi_delete)
bigint *int_to_new_bi(const intmax_t n);

/* `bigint` manipulation */

intmax_t bi_compare(bigint *const n1, bigint *const n2);
intmax_t bi_compare_int(bigint *const n1, const intmax_t n2);
bigint *bi_dup(bigint *const n);

/* `bigint` math */

bool bi_iszero(bigint const *const x);

bi_div_res
bi_divide_with_remainder(bigint *const restrict n1, bigint *const restrict n2);
bigint *bi_add(bigint *const restrict n1, bigint *const restrict n2);
bigint *bi_divide(bigint *const restrict n1, bigint *const restrict n2);
bigint *bi_modulo(bigint *const restrict n1, bigint *const restrict n2);
bigint *bi_multiply(bigint *const restrict n1, bigint *const restrict n2);
bigint *bi_power(bigint *const base, const intmax_t exponent);
bigint *bi_subtract(bigint *const restrict n1, bigint *const restrict n2);

bool bi_iadd(bigint *const restrict n1, bigint *const restrict n2);
bool bi_isubtract(bigint *const restrict n1, bigint *const restrict n2);

bigint *bi_add_int(bigint *const n1, const intmax_t n2);
bigint *bi_multiply_int(bigint *const n1, const intmax_t n2);
bigint *bi_subtract_int(bigint *const n1, const intmax_t n2);

bool bi_iadd_int(bigint *const n1, const intmax_t n2);
bool bi_isubtract_int(bigint *const n1, const intmax_t n2);

#endif /* BIGINT_H */
