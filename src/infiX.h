#ifndef INFIX_H
#define INFIX_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE /*program_invocation_name*/
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>
#include <math.h> /*pow(), Need to link with -lm*/

#define ATTR_MALLOC
#define ATTR_MALLOC_FREE(...)
#define ATTR_ALLOC_SIZE(...)
#define ATTR_NONNULL
#define ATTR_NONNULL_IDX(...)

#if defined __has_attribute

/*https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-malloc-function-attribute*/
#if __has_attribute(malloc)
#undef ATTR_MALLOC
#define ATTR_MALLOC __attribute__((malloc))
#undef ATTR_MALLOC_FREE
#define ATTR_MALLOC_FREE(...) __attribute__((malloc))
#endif /*__has_attribute(malloc)*/

/*https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-alloc_005fsize-variable-attribute*/
#if __has_attribute(alloc_size)
#undef ATTR_ALLOC_SIZE
#define ATTR_ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
#endif /*__has_attribute(alloc_size)*/

/*https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-malloc-function-attribute*/
#if __has_attribute(nonnull)
#undef ATTR_NONNULL
#define ATTR_NONNULL __attribute__((nonnull))
#undef ATTR_NONNULL_IDX
#define ATTR_NONNULL_IDX(...) __attribute__((nonnull(__VA_ARGS__)))
#endif /*__has_attribute(nonnull)*/

#endif /*defined __has_attribute*/

/*Max number of digits uint32_t should hold.*/
#define MAX_DIGITS_u4b (9)
/*Max size for uint32_t: 10^9.*/
#define MAX_VAL_u4b (1000000000)
/*Max number of digits for uint64_t.*/
#define MAX_DIGITS_u8b (MAX_DIGITS_u4b * MAX_DIGITS_u4b)
/*Max size for uint64_t calculations: 10^18.*/
#define MAX_VAL_u8b (MAX_VAL_u4b * MAX_VAL_u4b)
/*Negative bit toggle for uint32_t types.*/
#define NEGBIT_u4b (1 << 30)

/**
 * struct uint32_t_array_attributes - an array of unsigned 4 byte ints
 * @len: number of items in the array
 * @is_negative: flag for signedness of number
 * @array: pointer to an array of 4 byte ints
 */
typedef struct uint32_t_array_attributes
{
	size_t len;
	char is_negative;
	uint32_t *array;
} u4b_array;

/**
 * struct string_attributes - holds details about a string of characters
 * @str: the number string
 * @len: length of the string
 * @digits: number of digits in the string
 * @is_negative: atleast one bit should be set if negative
 */
typedef struct string_attributes
{
	uint8_t *str;
	size_t len;
	size_t digits;
	uint8_t is_negative;
} str_array;

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
void *free_n_null(void *ptr);
void *free_u4b_array(u4b_array *arr);
u4b_array *alloc_u4b_array(size_t len);

ATTR_MALLOC_FREE(free_n_null)
ATTR_ALLOC_SIZE(1, 2)
void *xcalloc(size_t items, size_t sizeof_item);

ATTR_MALLOC_FREE(free_n_null)
ATTR_ALLOC_SIZE(1)
void *xmalloc(size_t size);

/*string_funcs*/
str_array *parse_numstr(const char *numstr);
uint32_t *str_to_intarray(const char *num_str);
char *intarr_to_str(uint32_t *u32array);
size_t padding_chars_len(char *str, char *ch);

/*array_funcs*/
void trim_u4b_array(u4b_array *arr);
void print_u4b_array(u4b_array *arr);
ssize_t cmp_u4barray(u4b_array *arr1, u4b_array *arr2);
ssize_t cmp_rev_uint32array(uint32_t *arr1, uint32_t *arr2, size_t len);

/*math_funcs*/
u4b_array *infiX_division(u4b_array *n1, u4b_array *n2);
u4b_array *infiX_modulus(u4b_array *n1, u4b_array *n2);
u4b_array *infiX_subtraction(u4b_array *n1, u4b_array *n2);
u4b_array *infiX_multiplication(u4b_array *n1, u4b_array *n2);
u4b_array *infiX_addition(u4b_array *n1, u4b_array *n2);

#endif /* !INFIX_H */
