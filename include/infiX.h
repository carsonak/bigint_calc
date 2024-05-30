#ifndef INFIX_H
#define INFIX_H

#ifdef __cplusplus
extern "C"
{
#endif

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
#define MID_MAX_DIGITS (9)
/*Max size for uint32_t: 10^9.*/
#define MID_MAX_VAL (1000000000)
/*Max nnumber of digits for uint64_t.*/
#define HI_MAX_DIGITS (MID_MAX_DIGITS * MID_MAX_DIGITS)
/*Max size for uint64_t calculations: 10^18.*/
#define HI_MAX_VAL (MID_MAX_VAL * MID_MAX_VAL)
/*Negative bit toggle for uint32_t types.*/
#define NEGBIT_UI32 (1 << 30)

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
	 * struct string_attributes - holds details about a string of numbers
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

	void panic(const char *err_type);
	void helpme(const char *which_help);
	void *malloc_check(size_t size);
	void *calloc_check(size_t items, size_t sizeof_item);
	str_array *parse_numstr(const char *numstr);
	uint32_t *str_to_intarray(const char *num_str);
	char *intarr_to_str(uint32_t *u32array);
	void trim_intarr(uint32_t *arr);
	size_t padding_chars_len(char *str, char *ch);
	void *_memcpy(void *dest, void *src, size_t n);
	uint32_t *mplug_low(uint32_t **dest, uint32_t *src);
	uint32_t *mplug_num_low(uint32_t **dest, uint32_t src);

	char *infiX_manager(char *num1, char *op_symbol, char *num2);
	uint32_t *infiX_division(uint32_t *dividend, uint32_t *divisor);
	uint32_t *infiX_subtraction(uint32_t *n1_arr, uint32_t *n2_arr);
	uint32_t *infiX_multiplication(uint32_t *n1_arr, uint32_t *n2_arr);
	uint32_t *infiX_addition(uint32_t *n1_arr, uint32_t *n2_arr);

#ifdef __cplusplus
}
#endif

#endif /* !INFIX_H */
