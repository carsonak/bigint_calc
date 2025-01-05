#ifndef BI_TYPEDEFS_H
#define BI_TYPEDEFS_H

#include <limits.h> /* type_max */
#include <stdint.h> /* fixed width types */

#ifdef UINT64_MAX

	/*Maximum value of a single bigint "digit".*/
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

#endif /*UINT64_MAX*/

/*Radix range*/
#define MINIMUM_BIGINT_RADIX 2
#define MAXIMUM_BIGINT_RADIX 36

/**
 * mapping_func - a pointer to a function that maps one character to another.
 * @c: the character to be transformed.
 * @args: pointer to more arguments that can be passed to the function.
 *
 * Return: the transformed character.
 */
typedef char (*mapping_func)(const char c, void const *const args);

typedef struct bigint bigint;

#endif /* BI_TYPEDEFS_H */
