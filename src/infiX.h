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

/*Max size for uint32_t: 10^9.*/
#define MAX_VAL_u4b (1000000000)
/*Max size for uint64_t calculations: 10^18.*/
#define MAX_VAL_u8b (MAX_VAL_u4b * MAX_VAL_u4b)

/**
 * struct numstring_array_attributes - a string of numbers
 * @len: length of the string
 * @is_negative: a bool for signedness of the number
 * @str: the number string
 */
typedef struct numstring_array_attributes
{
	size_t len;
	bool is_negative;
	char *str;
} numstr_array;

/**
 * struct uint32_t_array_attributes - an array of unsigned 4 byte ints
 * @len: number of items in the array
 * @is_negative: a bool for signedness of the number
 * @array: pointer to an array of unsigned 4 byte ints
 */
typedef struct uint32_t_array_attributes
{
	size_t len;
	bool is_negative;
	uint32_t *array;
} u4b_array;

/**
 * math_function - generic prototype for basic arithmetic functions.
 */
typedef u4b_array *math_function(u4b_array *, u4b_array *);

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
void *free_u4b_array(u4b_array *freeable_arr);
void *free_numstr_array(numstr_array *freeable_arr);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_u4b_array)
u4b_array *alloc_u4b_array(size_t len);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_numstr_array)
numstr_array *alloc_numstr_array(size_t len);
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

/*parsing_funcs*/

size_t leading_chars_len(const char *str, char *ch);
unsigned int count_digits(ssize_t num);
ssize_t print_u4b_array(u4b_array *arr);
char *uint_array_to_str(const uint32_t *arr, size_t len);
numstr_array *parse_numstr(const char *numstr);

/*array_funcs*/

void trim_u4b_array(u4b_array *arr);
ssize_t cmp_u4barray(u4b_array *arr1, u4b_array *arr2);
ssize_t cmp_rev_uint32array(uint32_t *arr1, uint32_t *arr2, size_t len);

/*math_funcs*/

u4b_array *infiX_division(u4b_array *n1, u4b_array *n2);
u4b_array *infiX_modulus(u4b_array *n1, u4b_array *n2);
u4b_array *infiX_subtraction(u4b_array *n1, u4b_array *n2);
u4b_array *infiX_multiplication(u4b_array *n1, u4b_array *n2);
u4b_array *infiX_addition(u4b_array *n1, u4b_array *n2);

#endif /*INFIX_H*/
