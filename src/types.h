#ifndef BIGINT_TYPES_H
#define BIGINT_TYPES_H

#include <stdint.h> /* intmax_t */

/* for lengths and counting */
typedef intmax_t len_type;
#define LENTYPE_MAX INTMAX_MAX
#define LENTYPE_MIN INTMAX_MIN

/* for radix */
typedef unsigned short int radix_type;
#define RADIXTYPE_MAX USHRT_MAX
#define RADIXTYPE_MIN 0U

#endif /* BIGINT_TYPES_H */
