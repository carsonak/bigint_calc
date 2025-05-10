#ifndef BIGINT_H
#define BIGINT_H

#include <limits.h>	 /* type_max */
#include <stdbool.h> /* bool */
#include <stdint.h>	 /* fixed width types */

#include "types.h"

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

/* Radix range */
#define MINIMUM_BIGINT_RADIX 2
#define MAXIMUM_BIGINT_RADIX 36

/**
 * struct BIGINT_i - an arbitrary precision integer representation.
 * @len: number of items in the int array.
 * @is_negative: a bool for signedness of the number.
 * @num: pointer to an array of unsigned ints.
 */
struct bigint
{
	len_type len;
	bool is_negative;
	u_int *num;
};

typedef struct bigint bigint;

#endif /* BIGINT_H */
