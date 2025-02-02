#ifndef BIGINT_H
#define BIGINT_H

#include "macros.h"

#include <ctype.h>   /* isdigit */
#include <stdbool.h> /* bool */
#include <stddef.h>  /* size_t, ptr_diff */
#include <stdlib.h>  /* strtoull */
#include <string.h>  /* memmove */

#include "bigint_typedefs.h"

/* bigint construction */

void *bi_delete(bigint *const ptr);
ATTR_MALLOC ATTR_MALLOC_FREE(bi_delete)
bigint *bi_new(
	char const *const number, const unsigned short int base,
	size_t *const processed);
bool int_to_bi(bigint *dest, const long long int n);
ATTR_MALLOC ATTR_MALLOC_FREE(bi_delete)
bigint *int_to_new_bi(const long long int n);

/* bigint manipulation */

l_int bi_compare(bigint *const n1, bigint *const n2);
bigint *bi_dup(bigint *const n);
char *bi_tostr(bigint const *const n);

/* bigint math */

bool bi_iszero(bigint const *const x);

bigint *bi_add(bigint *const n1, bigint *const n2);
bigint *bi_add_int(bigint *const n1, long long int n2);
bool bi_iadd(bigint *const n1, bigint *const n2);
bool bi_iadd_int(bigint *const n1, long long int n2);

bool bi_isubtract(bigint *const n1, bigint *const n2);
bool bi_isubtract_int(bigint *const n1, long long int n2);
bigint *bi_subtract(bigint *const n1, bigint *const n2);
bigint *bi_subtract_int(bigint *const n1, long long int n2);

bigint *bi_multiply(bigint *const n1, bigint *const n2);

bigint *bi_divide(bigint *const n1, bigint *const n2);
bigint *bi_modulo(bigint *const n1, bigint *const n2);

bigint *bi_power(bigint *base, bigint *exponent);

#endif /* BIGINT_H */
