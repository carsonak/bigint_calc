#ifndef BIGINT_TYPEDEFS_H
#define BIGINT_TYPEDEFS_H

#include <limits.h>
#include <stdint.h> /* fixed width types */

#ifdef UINT64_MAX

	/* Maximum value of a single bigint "digit". */
	#define BIGINT_BASE (1000000000)

typedef uint32_t u_int;
typedef int64_t l_int;
typedef uint64_t ul_int;

	#define U_INT_MAX UINT32_MAX
	#define L_INT_MIN INT64_MIN
	#define L_INT_MAX INT64_MAX
	#define UL_INT_MAX UINT64_MAX

#else

	#define BIGINT_BASE (10000)

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

#ifndef BIGINT_IS_VALID_RADIX
	#define BIGINT_IS_VALID_RADIX

/**
 * is_valid_radix - check if a number is within the accepted radix range.
 * @base: the number to check.
 *
 * minimum radix: `MINIMUM_BIGINT_RADIX`.
 * maximum radix: `MAXIMUM_BIGINT_RADIX`.
 *
 * Return: true if `base` is within the valid range, false otherwise.
 */
static bool is_valid_radix(const unsigned short int base)
{
	return (base >= MINIMUM_BIGINT_RADIX && base <= MAXIMUM_BIGINT_RADIX);
}

#endif /* BIGINT_IS_VALID_RADIX */

typedef struct bigint bigint;
typedef struct numstr numstr;

#endif /* BIGINT_TYPEDEFS_H */
