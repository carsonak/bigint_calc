#ifndef BIGINT_TYPES_H
#define BIGINT_TYPES_H

#include <inttypes.h> /* *printf macros */
#include <limits.h>
#include <stdint.h> /* fixed width types */

#include "types.h"

#if UINTPTR_MAX >= UINT64_MAX

typedef uint32_t u_int;
typedef int64_t l_int;
typedef uint64_t ul_int;

	/* Minimums and maximums. */

	#define U_INT_MAX UINT32_MAX
	#define L_INT_MIN INT64_MIN
	#define L_INT_MAX INT64_MAX
	#define UL_INT_MAX UINT64_MAX

	/* *printf conversion specifiers. */

	#define PRIu_int PRIu32
	#define PRIl_int PRId64
	#define PRIul_int PRIu64

	/* Radix for the `bigint` type. */
	#define BIGINT_BASE 1000000000U

#elif UINTPTR_MAX == UINT32_MAX

typedef uint16_t u_int;
typedef int32_t l_int;
typedef uint32_t ul_int;

	/* Minimums and maximums. */

	#define U_INT_MAX UINT16_MAX
	#define L_INT_MIN INT32_MIN
	#define L_INT_MAX INT32_MAX
	#define UL_INT_MAX UINT32_MAX

	/* *printf conversion specifiers. */

	#define PRIu_int PRIu16
	#define PRIl_int PRId32
	#define PRIul_int PRIu32

	/* Radix for the `bigint` type. */
	#define BIGINT_BASE 10000U

#elif UINTPTR_MAX == UINT16_MAX

typedef uint8_t u_int;
typedef int16_t l_int;
typedef uint16_t ul_int;

	/* Minimums and maximums. */

	#define U_INT_MAX UINT8_MAX
	#define L_INT_MIN INT16_MIN
	#define L_INT_MAX INT16_MAX
	#define UL_INT_MAX UINT16_MAX

	/* *printf conversion specifiers. */

	#define PRIu_int PRIu8
	#define PRIl_int PRId16
	#define PRIul_int PRIu16

	/* Radix for the `bigint` type. */
	#define BIGINT_BASE 100U

#else
	#error "Unknown pointer size or missing integer size macros."
#endif /* UINTPTR_MAX >= UINT64_MAX */

typedef struct bigint bigint;
typedef struct numstr numstr;

/**
 * struct bigint_division_result - container that holds the quotient and
 * remainder of a `bigint` division.
 * @quotient: pointer to the quotient.
 * @remainder: pointer to the remainder.
 */
typedef struct bigint_division_result
{
	bigint *restrict quotient, *restrict remainder;
} bi_div_res;

#endif /* BIGINT_TYPES_H */
