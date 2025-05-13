#ifndef BIGINT_TYPEDEFS_H
#define BIGINT_TYPEDEFS_H

#include <limits.h>
#include <stdbool.h>
#include <stdint.h> /* fixed width types */

#ifdef UINT64_MAX

	/* Maximum value of a single `bigint` "digit". */
	#define BIGINT_BASE 1000000000U

typedef uint32_t u_int;
typedef int64_t l_int;
typedef uint64_t ul_int;

	#define U_INT_MAX UINT32_MAX
	#define L_INT_MIN INT64_MIN
	#define L_INT_MAX INT64_MAX
	#define UL_INT_MAX UINT64_MAX

#else

	#define BIGINT_BASE 10000U

typedef uint16_t u_int;
typedef int32_t l_int;
typedef uint32_t ul_int;

	#define U_INT_MAX UINT16_MAX
	#define L_INT_MIN INT32_MIN
	#define L_INT_MAX INT32_MAX
	#define UL_INT_MAX UINT32_MAX

#endif /* UINT64_MAX */

/* type used for counting and array lengths. */
typedef intmax_t len_type;

/* Radix range */
#define MINIMUM_BIGINT_RADIX 2U
#define MAXIMUM_BIGINT_RADIX 36U

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
	bigint *quotient, *remainder;
} bi_div_res;

#endif /* BIGINT_TYPEDEFS_H */
