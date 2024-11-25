#ifndef BIGNUM_H
#define BIGNUM_H

#include <limits.h>	 /* type_max */
#include <stdbool.h> /* bool */
#include <stddef.h>	 /* size_t */
#include <stdint.h>	 /* fixed width types */

#ifdef UINT64_MAX

/*Maximum value of a single bignum "digit".*/
#define BIGNUM_BASE (1000000000)

typedef uint32_t u_int;
typedef int64_t l_int;
typedef uint64_t ul_int;

#define U_INT_MAX UINT32_MAX
#define L_INT_MIN INT64_MIN
#define L_INT_MAX INT64_MAX
#define UL_INT_MAX UINT64_MAX

#else

#define BIGNUM_BASE (10000)

typedef uint16_t u_int;
typedef int32_t l_int;
typedef uint32_t ul_int;

#define U_INT_MAX UINT16_MAX
#define L_INT_MIN INT32_MIN
#define L_INT_MAX INT32_MAX
#define UL_INT_MAX UINT32_MAX

#endif /*UINT64_MAX*/

/**
 * struct bignum_i - an arbitrary precision integer representation.
 * @len: number of items in the int array.
 * @is_negative: a bool for signedness of the number.
 * @num: pointer to an array of unsigned ints.
 */
typedef struct bignum_i
{
	size_t len;
	bool is_negative;
	u_int *num;
} bignum_i;

#endif /*BIGNUM_H*/
