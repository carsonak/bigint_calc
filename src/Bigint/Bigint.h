#ifndef BIGINT_H
#define BIGINT_H

#include "compiler_attributes_macros.h"

#include <stdbool.h>  // bool

#include "bigint_types.h"

/* `Bigint` construction */

void *bi_delete(Bigint *const ptr);
Bigint *bi_new(
	char const *const restrict number, const radix_ty base,
	len_ty *const restrict processed
) _malloc _malloc_free(bi_delete);

Bigint *int_to_bi(Bigint *const dest, const intmax_t n);
Bigint *int_to_new_bi(const intmax_t n) _malloc _malloc_free(bi_delete);

/* `Bigint` manipulation */

Bigint *bi_dup(Bigint *const n);
char *bi_tostr(Bigint const *const restrict n);
ldigit_ty bi_compare_int(Bigint *const n1, const intmax_t n2);
ldigit_ty bi_compare(Bigint *const n1, Bigint *const n2);

/* `Bigint` math */

bool bi_iszero(Bigint const *const x);
bool bi_isNaN(Bigint *const n);

bi_divmod_res bi_divmod(Bigint *const restrict n1, Bigint *const restrict n2);
Bigint *bi_add(Bigint *const restrict n1, Bigint *const restrict n2);
Bigint *bi_divide(Bigint *const restrict n1, Bigint *const restrict n2);
Bigint *bi_modulo(Bigint *const restrict n1, Bigint *const restrict n2);
Bigint *bi_multiply(Bigint *const restrict n1, Bigint *const restrict n2);
Bigint *bi_power(Bigint *const restrict base, const intmax_t exponent);
Bigint *bi_shift_l(Bigint *const restrict n, const intmax_t c);
Bigint *bi_shift_r(Bigint *const restrict n, const intmax_t c);
Bigint *bi_subtract(Bigint *const restrict n1, Bigint *const restrict n2);

Bigint *bi_ishift_l(Bigint *const restrict n, const intmax_t c);
Bigint *bi_ishift_r(Bigint *const restrict n, const intmax_t c);
Bigint *bi_iadd(Bigint *const restrict n1, Bigint *const restrict n2);
Bigint *bi_isubtract(Bigint *const restrict n1, Bigint *const restrict n2);

Bigint *bi_add_int(Bigint *const restrict n1, const intmax_t n2);
Bigint *bi_multiply_int(Bigint *const restrict n1, const intmax_t n2);
Bigint *bi_subtract_int(Bigint *const restrict n1, const intmax_t n2);

Bigint *bi_iadd_int(Bigint *const restrict n1, const intmax_t n2);
Bigint *bi_isubtract_int(Bigint *const restrict n1, const intmax_t n2);

#endif  // BIGINT_H
