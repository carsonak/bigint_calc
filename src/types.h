#ifndef BIGINT_CALC_TYPES_H
#define BIGINT_CALC_TYPES_H

#include <limits.h>  // USHRT_MAX
#include <stddef.h>  // ptrdiff_t
#include <stdint.h>  // PTRDIFF_MAX

/*! for lengths and counting. */
typedef ptrdiff_t len_ty;
/*! for radix. */
typedef unsigned short int radix_ty;

/* maximums and minimums. */

#define LEN_TY_max PTRDIFF_MAX
#define LEN_TY_min PTRDIFF_MIN
#define RADIX_TY_max USHRT_MAX
#define RADIX_TY_min 0U

/* *printf conversion specifiers. */

#define PRI_len "td"
#define PRI_radix "hu"

#endif  // BIGINT_CALC_TYPES_H
