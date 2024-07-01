#ifndef INFIX_H
#define INFIX_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE /*program_invocation_name*/
#endif

#include <ctype.h>	 /* isdigit */
#include <limits.h>	 /* type_max */
#include <stdbool.h> /* bool */
#include <stdio.h>	 /* *printf, perror */
#include <stdlib.h>	 /* *alloc */
#include <string.h>	 /* strlen, strcpy */

#define ATTR_ALLOC_SIZE(...)
#define ATTR_MALLOC
#define ATTR_MALLOC_FREE(...)
#define ATTR_NONNULL
#define ATTR_NONNULL_IDX(...)

#if defined __has_attribute

/*https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-alloc_005fsize-variable-attribute*/
#if __has_attribute(alloc_size)
#undef ATTR_ALLOC_SIZE
#define ATTR_ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
#endif /*__has_attribute(alloc_size)*/

/*https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-malloc-function-attribute*/
#if __has_attribute(malloc)
#undef ATTR_MALLOC
#define ATTR_MALLOC __attribute__((malloc))
#undef ATTR_MALLOC_FREE
#define ATTR_MALLOC_FREE(...) __attribute__((malloc(__VA_ARGS__)))
#endif /*__has_attribute(malloc)*/

/*https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-malloc-function-attribute*/
#if __has_attribute(nonnull)
#undef ATTR_NONNULL
#define ATTR_NONNULL __attribute__((nonnull))
#undef ATTR_NONNULL_IDX
#define ATTR_NONNULL_IDX(...) __attribute__((nonnull(__VA_ARGS__)))
#endif /*__has_attribute(nonnull)*/

#endif /*defined __has_attribute*/

/*Maximum value a uint_32 should hold.*/
#define MAX_VAL_u4b (1000000000)
/*Max value for unsigned long int calculations*/
#define MAX_VAL_u8b (MAX_VAL_u4b * MAX_VAL_u4b)

/**
 * struct numstr - a string of numbers.
 * @len: length of the string.
 * @is_negative: a bool for signedness of the number.
 * @str: the number string.
 */
typedef struct numstr
{
	unsigned long int len;
	bool is_negative;
	char *str;
} numstr;

/**
 * struct BigNum - an array of unsigned 4 byte ints.
 * @len: number of items in the array.
 * @is_negative: a bool for signedness of the number.
 * @num: pointer to an array of unsigned 4 byte ints.
 */
typedef struct BigNum
{
	unsigned long int len;
	bool is_negative;
	unsigned int *num;
} BigNum;

/**
 * math_function - generic prototype for basic arithmetic functions.
 */
typedef BigNum * math_function(BigNum *, BigNum *);

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

void *free_bignum(BigNum *freeable_ptr);
void *free_numstr(numstr *freeable_ptr);
void *free_n_null(void *freeable_ptr);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_bignum)
BigNum *alloc_bignum(unsigned long int len);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_numstr)
numstr *alloc_numstr(unsigned long int len);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_n_null)
ATTR_ALLOC_SIZE(1, 2)
void *xcalloc(unsigned long int items, unsigned long int sizeof_item);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_n_null)
ATTR_ALLOC_SIZE(1)
void *xmalloc(unsigned long int size);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_n_null)
ATTR_ALLOC_SIZE(2)
void *xrealloc(void *nullable_ptr, unsigned long int size);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_n_null)
char *xstrdup(const char *str);

/*parsing_funcs*/

numstr *parse_str(const char *str);
unsigned long int leading_chars_len(const char *str, char *ch);
BigNum *numstr_to_bignum(numstr *num);
numstr *bignum_to_numstr(BigNum *arr);
long int print_bignum(BigNum *arr);
char *uint_array_to_str(const unsigned int *arr, unsigned long int len);
unsigned int count_digits(unsigned long int num, unsigned int base);
unsigned int get_base(void);
unsigned int set_base(unsigned int base);

/*array_funcs*/

void trim_bignum(BigNum *const arr);
long int cmp_bignum(BigNum *const a1, BigNum *const a2);
long int cmp_rev_uint32array(
	unsigned int const *const arr1,
	unsigned int const *const arr2,
	unsigned long int len);

/*math_funcs*/

BigNum *infiX_division(BigNum *n1, BigNum *n2);
BigNum *infiX_modulus(BigNum *n1, BigNum *n2);
BigNum *infiX_subtraction(BigNum *n1, BigNum *n2);
BigNum *infiX_multiplication(BigNum *n1, BigNum *n2);
BigNum *infiX_addition(BigNum *n1, BigNum *n2);

#endif /*INFIX_H*/
