#ifndef BIGNUM_H
#define BIGNUM_H

#include <limits.h>	 /* type_max */
#include <stdbool.h> /* bool */
#include <stddef.h>	 /* size_t, ptr_diff */
#include <stdint.h>	 /* fixed width types */

#ifdef UINT64_MAX

/*Maximum value of a uint in bignum.*/
#define BIGNUM_UINT_MAX (1000000000)

typedef uint32_t uint;
typedef int64_t lint;
typedef uint64_t ulint;

#else

/*Maximum value of a uint in bignum.*/
#define BIGNUM_UINT_MAX (10000)

typedef uint16_t uint;
typedef int32_t lint;
typedef uint32_t ulint;

#endif /*UINT64_MAX*/

/**
 * struct bignum - an array of unsigned 4 byte ints.
 * @len: number of items in the array.
 * @is_negative: a bool for signedness of the number.
 * @num: pointer to an array of unsigned 4 byte ints.
 */
typedef struct bignum
{
	size_t len;
	bool is_negative;
	uint *num;
} bignum;

#endif /*BIGNUM_H*/
