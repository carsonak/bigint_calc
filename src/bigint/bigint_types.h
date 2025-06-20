#ifndef BIGINT_TYPES_H
#define BIGINT_TYPES_H

#include <inttypes.h>  // *printf macros
#include <limits.h>
#include <stdint.h>  // fixed width types

#include "types.h"

#if UINTMAX_MAX >= UINT64_MAX

typedef int32_t digit_ty;
typedef int64_t ldigit_ty;
typedef uint32_t udigit_ty;
typedef uint64_t uldigit_ty;

	/* Minimums and maximums. */

	#define DIGIT_TY_max INT32_MAX
	#define DIGIT_TY_min INT32_MIN
	#define LDIGIT_TY_max INT64_MAX
	#define LDIGIT_TY_min INT64_MIN
	#define UDIGIT_TY_max UINT32_MAX
	#define ULDIGIT_TY_max UINT64_MAX

	/* *printf conversion specifiers. */

	#define PRI_digit PRIi32
	#define PRI_ldigit PRId64
	#define PRI_udigit PRIu32
	#define PRI_uldigit PRIu64

	#define BIGINT_BASE 1000000000  //!< Radix of a `bigint`.

#elif UINTMAX_MAX >= UINT32_MAX

typedef int16_t digit_ty;
typedef int32_t ldigit_ty;
typedef uint16_t udigit_ty;
typedef uint32_t uldigit_ty;

	/* Minimums and maximums. */

	#define DIGIT_TY_max INT16_MAX
	#define DIGIT_TY_min INT16_MIN
	#define LDIGIT_TY_max INT32_MAX
	#define LDIGIT_TY_min INT32_MIN
	#define UDIGIT_TY_max UINT16_MAX
	#define ULDIGIT_TY_max UINT32_MAX

	/* *printf conversion specifiers. */

	#define PRI_digit PRIi16
	#define PRI_ldigit PRId32
	#define PRI_udigit PRIu16
	#define PRI_uldigit PRIu32

	/* Radix for the `bigint` type. */
	#define BIGINT_BASE 10000

#elif UINTMAX_MAX >= UINT16_MAX

typedef int8_t digit_ty;
typedef int16_t ldigit_ty;
typedef uint8_t udigit_ty;
typedef uint16_t uldigit_ty;

	/* Minimums and maximums. */

	#define DIGIT_TY_max INT8_MAX
	#define DIGIT_TY_min INT8_MIN
	#define LDIGIT_TY_max INT16_MAX
	#define LDIGIT_TY_min INT16_MIN
	#define UDIGIT_TY_max UINT8_MAX
	#define ULDIGIT_TY_max UINT16_MAX

	/* *printf conversion specifiers. */

	#define PRI_digit PRIi8
	#define PRI_ldigit PRId16
	#define PRI_udigit PRIu8
	#define PRI_uldigit PRIu16

	/* Radix for the `bigint` type. */
	#define BIGINT_BASE 100

#else
	#error "uintmax_t types less than 16 bits wide are not supported."
#endif  // UINTMAX_MAX >= UINT64_MAX

typedef struct bigint bigint;

/*!
 * @brief container that holds the quotient and remainder of a
 * `bigint` division.
 */
typedef struct bigint_division_result
{
	/*! pointer to the quotient. */
	bigint *restrict quotient;
	/*! pointer to the remainder. */
	bigint *restrict remainder;
} bi_divmod_res;

#endif  // BIGINT_TYPES_H
