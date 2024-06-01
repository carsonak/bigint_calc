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

/*Max number of digits uint32_t should hold.*/
#define MAX_DIGITS_u4b (9)
/*Max size for uint32_t: 10^9.*/
#define MAX_VAL_u4b (1000000000)
/*Max nnumber of digits for uint64_t.*/
#define MAX_DIGITS_u8b (MAX_DIGITS_u4b * MAX_DIGITS_u4b)
/*Max size for uint64_t calculations: 10^18.*/
#define MAX_VAL_u8b (MAX_VAL_u4b * MAX_VAL_u4b)
/*Negative bit toggle for uint32_t types.*/
#define NEGBIT_u4b (1 << 30)

/*A dynamically allocated array to store remainders of division.*/
extern uint32_t *remains;

/*A function that operates on two int arrays and returns a new array.*/
typedef uint32_t *math_function(uint32_t *, uint32_t *);

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

char *infiX_manager(char *num1, char *op_symbol, char *num2);
void panic(const char *err_type);
void helpme(const char *which_help);

void *check_calloc(size_t items, size_t sizeof_item);
void *check_malloc(size_t size);
void free_n_null(void *ptr);
void free_u4b_array(u4b_array *arr);

str_array *parse_numstr(const char *numstr);
uint32_t *str_to_intarray(const char *num_str);
char *intarr_to_str(uint32_t *u32array);
void trim_u4b_array(u4b_array *arr);
size_t padding_chars_len(char *str, char *ch);
uint32_t *mplug_low(uint32_t **dest, uint32_t *src);
uint32_t *mplug_num_low(uint32_t **dest, uint32_t src);

uint32_t *infiX_division(uint32_t *dividend, uint32_t *divisor);
u4b_array *infiX_subtraction(u4b_array *n1, u4b_array *n2);
uint32_t *infiX_multiplication(uint32_t *n1_arr, uint32_t *n2_arr);
u4b_array *infiX_addition(u4b_array *n1, u4b_array *n2);

#endif /* !INFIX_H */
