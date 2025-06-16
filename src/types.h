#ifndef BIGINT_CALC_TYPES_H
#define BIGINT_CALC_TYPES_H

#include <limits.h> /* USHRT_MAX */
#include <stddef.h> /* ptrdiff_t */
#include <stdint.h> /* PTRDIFF_MAX */

/*! for lengths and counting. */
typedef ptrdiff_t len_type;
/*! for radix. */
typedef unsigned short int radix_type;

/* maximums and minimums. */

#define LENTYPE_MAX PTRDIFF_MAX
#define LENTYPE_MIN PTRDIFF_MIN
#define RADIXTYPE_MAX USHRT_MAX
#define RADIXTYPE_MIN 0U

/* *printf conversion specifiers. */

#define PRIlen_type "td"
#define PRIradix_type "hu"

#endif /* BIGINT_CALC_TYPES_H */
