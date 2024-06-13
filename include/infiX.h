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

/*https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#index-cleanup-variable-attribute*/
#define autofree_gen_p __attribute__((cleanup(free_n_null)))
#define autofree_u4b_array __attribute__((cleanup(free_u4b_array)))

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

/*A function that operates on two int arrays and returns a new array.*/
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

/*A dynamically allocated array to store remainders of division.*/
extern u4b_array *remains;

char *infiX_manager(char *num1, char *op_symbol, char *num2);
void panic(const char *err_type);
void helpme(const char *which_help);

/*mem_funcs*/
void *free_n_null(void *ptr);
void *free_u4b_array(u4b_array *arr);
u4b_array *alloc_u4b_array(size_t len);
__attribute__((malloc(free_n_null))) void *
xcalloc(size_t items, size_t sizeof_item);
__attribute__((malloc(free_n_null))) void *xmalloc(size_t size);

/*string_funcs*/
str_array *parse_numstr(const char *numstr);
uint32_t *str_to_intarray(const char *num_str);
char *intarr_to_str(uint32_t *u32array);
size_t padding_chars_len(char *str, char *ch);

/*array_funcs*/
__attribute__((nonnull)) void trim_u4b_array(u4b_array *arr);
__attribute__((nonnull(1, 2))) ssize_t
cmp_u4barray(u4b_array *arr1, u4b_array *arr2);
void print_u4b_array(u4b_array *arr);

/*math_funcs*/
__attribute__((nonnull)) u4b_array *
infiX_division(u4b_array *n1, u4b_array *n2);
__attribute__((nonnull)) u4b_array *
infiX_subtraction(u4b_array *n1, u4b_array *n2);
__attribute__((nonnull)) u4b_array *
infiX_multiplication(u4b_array *n1, u4b_array *n2);
__attribute__((nonnull)) u4b_array *
infiX_addition(u4b_array *n1, u4b_array *n2);

#endif /* !INFIX_H */
