#ifndef BIGINT_H
#define BIGINT_H

#include "macros.h"

#include <ctype.h>   /* isdigit */
#include <stdbool.h> /* bool */
#include <stddef.h>  /* size_t, ptr_diff */
#include <stdlib.h>  /* strtoull */
#include <string.h>  /* strlen, strcpy */

#include "bigint_typedefs.h"

/* bigint construction */

void *bi_delete(bigint *const freeable_ptr);
bigint *bi_new(
	char const *const num_str, unsigned short int base,
	size_t *const processed);
char *filter_str(
	char const *const str, size_t *const processed, const mapping_func f,
	void const *const f_args);

/* bigint manipulation */

bool bi_copy(bigint *src, bigint *dest);
l_int bi_compare(bigint *const a1, bigint *const a2);
bool bi_move(bigint *src, bigint *dest);
bool bi_swap(bigint *const n1, bigint *const n2);
char *bi_tostr(bigint *bi, const unsigned int base);

bool bi_is_zero(bigint *const x);

#endif /* BIGINT_H */
