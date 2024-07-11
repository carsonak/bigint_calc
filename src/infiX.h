#ifndef INFIX_H
#define INFIX_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE /*program_invocation_name*/
#endif

#include <ctype.h>	 /* isdigit */
#include <limits.h>	 /* type_max */
#include <stdbool.h> /* bool */
#include <stdint.h>	 /* fixed width types */
#include <stdio.h>	 /* *printf, perror */
#include <stdlib.h>	 /* *alloc */
#include <string.h>	 /* strlen, strcpy */

#define ATTR_ALLOC_SIZE(...)
#define ATTR_MALLOC
#define ATTR_MALLOC_FREE(...)
#define ATTR_NONNULL
#define ATTR_NONNULL_IDX(...)

#if defined __has_attribute

/*https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html*/
#if __has_attribute(alloc_size)
#undef ATTR_ALLOC_SIZE
#define ATTR_ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
#endif /*__has_attribute(alloc_size)*/

/*https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html*/
#if __has_attribute(malloc)
#undef ATTR_MALLOC
#define ATTR_MALLOC __attribute__((malloc))
#undef ATTR_MALLOC_FREE
#define ATTR_MALLOC_FREE(...) __attribute__((malloc(__VA_ARGS__)))
#endif /*__has_attribute(malloc)*/

#if __has_attribute(nonnull)
#undef ATTR_NONNULL
#define ATTR_NONNULL __attribute__((nonnull))
#undef ATTR_NONNULL_IDX
#define ATTR_NONNULL_IDX(...) __attribute__((nonnull(__VA_ARGS__)))
#endif /*__has_attribute(nonnull)*/

#endif /*defined __has_attribute*/

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
 * struct numstr - a string of numbers.
 * @len: length of the string.
 * @is_negative: a bool for signedness of the number.
 * @str: the number string.
 */
typedef struct numstr
{
	size_t len;
	bool is_negative;
	char *str;
} numstr;

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

/* generic prototype for basic arithmetic functions. */
typedef bignum * math_function(bignum *, bignum *);

/**
 * struct operator_function - holds an operator symbol and it's function.
 * @symbol: the operator symbol.
 * @f: a corresponding function pointer.
 */
typedef struct operator_function
{
	char *symbol;
	math_function *f;
} op_func;

char *infiX_manager(char *num1, char *op_symbol, char *num2);
void panic(const char *err_type);
void help_me(const char *which_help);

/*mem_funcs*/

void *free_bignum(bignum *freeable_ptr);
void *free_numstr(numstr *freeable_ptr);
void *free_n_null(void *freeable_ptr);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_bignum)
bignum *alloc_bignum(size_t len);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_numstr)
numstr *alloc_numstr(size_t len);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_n_null)
ATTR_ALLOC_SIZE(1, 2)
void *xcalloc(size_t items, size_t sizeof_item);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_n_null)
ATTR_ALLOC_SIZE(1)
void *xmalloc(size_t size);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_n_null)
ATTR_ALLOC_SIZE(2)
void *xrealloc(void *nullable_ptr, size_t size);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_n_null)
char *xstrdup(const char *str);

/*parsing_funcs*/

numstr *parse_number(const char *str, unsigned int base);
size_t leading_chars_len(const char *str, char *ch);
bignum *numstr_to_bignum(numstr *num, unsigned int base);
numstr *bignum_to_numstr(bignum *arr);
size_t print_bignum(bignum *arr);
char *uint_array_to_str(const uint *arr, size_t len);
unsigned int count_digits(size_t num, unsigned int base);

/*array_funcs*/

void trim_bignum(bignum *const arr);
lint cmp_bignum(bignum *const a1, bignum *const a2);
lint cmp_rev_uint32array(
	uint const *const arr1, uint const *const arr2, size_t len);

/*math_funcs*/

bignum *bn_division(bignum *n1, bignum *n2);
bignum *bn_modulus(bignum *n1, bignum *n2);
bignum *bn_subtraction(bignum *n1, bignum *n2);
bignum *bn_multiplication(bignum *n1, bignum *n2);
bignum *bn_addition(bignum *n1, bignum *n2);

#endif /*INFIX_H*/
