#ifndef BIGINT_CALC_TYPES_H
#define BIGINT_CALC_TYPES_H

#include <limits.h>  // USHRT_MAX

#include "len_type.h"

/*! for radix. */
typedef unsigned short int radix_ty;

/* maximums and minimums. */

#define RADIX_TY_max USHRT_MAX
#define RADIX_TY_min 0U

/* *printf conversion specifiers. */

#define PRI_radix "hu"

#endif  // BIGINT_CALC_TYPES_H
