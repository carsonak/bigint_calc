#ifndef INFIX_H
#define INFIX_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE /*program_invocation_name*/
#endif

#include <ctype.h>	 /* isdigit */
#include <limits.h>	 /* type_max */
#include <stdbool.h> /* bool */
#include <stdint.h>	 /* specific width types */
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
/*Max value for uint64_t calculations*/
#define MAX_VAL_u8b (MAX_VAL_u4b * MAX_VAL_u4b)

/**
 * struct numstr - a string of numbers
 * @len: length of the string
 * @is_negative: a bool for signedness of the number
 * @str: the number string
 */
typedef struct numstr
{
	size_t len;
	bool is_negative;
	char *str;
} numstr;

/**
 * struct u4b_bignum - an array of unsigned 4 byte ints
 * @len: number of items in the array
 * @is_negative: a bool for signedness of the number
 * @array: pointer to an array of unsigned 4 byte ints
 */
typedef struct u4b_bignum
{
	size_t len;
	bool is_negative;
	uint32_t *array;
} u4b_bignum;

/**
 * math_function - generic prototype for basic arithmetic functions.
 */
typedef u4b_bignum *math_function(u4b_bignum *, u4b_bignum *);

/**
 * struct operator_function - holds an operator symbol and it's function
 * @symbol: the operator symbol
 * @f: a corresponding function pointer
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

void *free_n_null(void *freeable_ptr);
void *free_bignum(u4b_bignum *freeable_arr);
void *free_numstr(numstr *freeable_arr);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_bignum)
u4b_bignum *alloc_bignum(size_t len);
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

numstr *parse_str(const char *str);
u4b_bignum *numstr_to_u4b(numstr *num);
numstr *u4b_to_numstr(u4b_bignum *arr);
ssize_t print_bignum(u4b_bignum *arr);
char *uint_array_to_str(const uint32_t *arr, size_t len);
size_t leading_chars_len(const char *str, char *ch);
unsigned int count_digits(size_t num, unsigned int base);
char to_base36(unsigned int num);
int from_base36(char c);
unsigned int get_base(void);
unsigned int set_base(unsigned int base);

/*array_funcs*/

void trim_bignum(u4b_bignum *arr);
ssize_t cmp_bignum(u4b_bignum *arr1, u4b_bignum *arr2);
ssize_t cmp_rev_uint32array(uint32_t *arr1, uint32_t *arr2, size_t len);

/*math_funcs*/

u4b_bignum *infiX_division(u4b_bignum *n1, u4b_bignum *n2);
u4b_bignum *infiX_modulus(u4b_bignum *n1, u4b_bignum *n2);
u4b_bignum *infiX_subtraction(u4b_bignum *n1, u4b_bignum *n2);
u4b_bignum *infiX_multiplication(u4b_bignum *n1, u4b_bignum *n2);
u4b_bignum *infiX_addition(u4b_bignum *n1, u4b_bignum *n2);

#endif /*INFIX_H*/
