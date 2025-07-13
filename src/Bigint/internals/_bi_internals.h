#ifndef BIGINT_INTERNALS_H
#define BIGINT_INTERNALS_H

#include "compiler_attributes_macros.h"

#include <limits.h>   // CHAR_BIT
#include <stdbool.h>  // bool

#include "bigint_types.h"
#include "xalloc.h"

/*!
 * @brief a representation of an arbitrary precision integer.
 */
struct Bigint
{
	/*! @protected boolean indicating signedness of the number. */
	bool is_negative : 1;
	/*! @protected number of "digits" in the number. */
	len_ty len : sizeof(len_ty) * CHAR_BIT - 1;
	/*! @protected pointer to an array of unsigned ints. */
	udigit_ty *restrict num;
};

/* memory manipulation */

void *_bi_free(Bigint *const freeable_ptr);

Bigint *_bi_alloc(const len_ty len) _malloc _malloc_free(_bi_free);
Bigint *
_bi_dup(Bigint const *const restrict bn) _malloc _malloc_free(_bi_free);
Bigint *_bi_resize(Bigint *bi, const len_ty len) _malloc_free(_bi_free);

/* utility */

bool _bi_isNaN_const(const Bigint *const n);
Bigint *_bi_trim(Bigint *const restrict n);
ldigit_ty _bi_compare_int_const(const Bigint *const n1, const intmax_t n2);
ldigit_ty _bi_compare_const(const Bigint *const n1, const Bigint *const n2);
ldigit_ty _cmp_rev_uint_arr(
	udigit_ty const *const arr1, udigit_ty const *const arr2, len_ty len
);
Bigint *_bi_move(Bigint *const dest, Bigint const *const src);

#endif  // BIGINT_INTERNALS_H
