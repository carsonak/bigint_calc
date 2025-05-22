#ifndef BIGINT_CALC_TYPES_H
#define BIGINT_CALC_TYPES_H

#include <inttypes.h> /* *printf macros */
#include <stdint.h> /* intmax_t */

/* for lengths and counting */
typedef intmax_t len_type;
/* for radix */
typedef unsigned short int radix_type;

/* maximums and minimums. */

#define LENTYPE_MAX INTMAX_MAX
#define LENTYPE_MIN INTMAX_MIN
#define RADIXTYPE_MAX USHRT_MAX
#define RADIXTYPE_MIN 0U

/* *printf conversion specifiers. */

#define PRIlen_type PRIdMAX
#define PRIradix_type "hu"

#endif /* BIGINT_CALC_TYPES_H */
