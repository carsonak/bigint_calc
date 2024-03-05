#ifndef _INFIX_H_
#define _INFIX_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>
#include <math.h> /*pow(), Need to link with -lm*/

typedef uint64_t hi_uint;
typedef uint32_t mid_uint;
typedef uint8_t lo_uchar;

/*Max number of digits mid_uint should hold*/
#define MID_MAX_DIGITS (9)
/*Max size for mid_uint: 10^9*/
#define MID_MAX_VAL (1000000000)
/*Max nnumber of digits for uint64_t*/
#define HI_MAX_DIGITS (MID_MAX_DIGITS * MID_MAX_DIGITS)
/*Max size for uint64_t calculations: 10^18*/
#define HI_MAX_VAL (MID_MAX_VAL * MID_MAX_VAL)
/*mid_uint negative bit toggle*/
#define NEGBIT_MIDUINT (1 << 30)

/*A dynamically allocated buffer to store remainders of division*/
extern mid_uint *remain;

/**
 * struct operator_function - holds an operator symbol and it's function
 * @op_symbol: the operator symbol
 * @f: a corresponding function pointer
 */
typedef struct operator_function
{
	char *symbol;
	mid_uint *(*f)(mid_uint *n1, mid_uint *n2);

} op_func;

void print_help(void);
mid_uint *str_to_intarray(lo_uchar *num);
lo_uchar *intarr_to_str(mid_uint *u32a);
void trim_intarr(mid_uint *arr);
size_t pad_char(char *str, char *ch);
void *memfill(void *mem, char b, size_t start, size_t nbytes);
void *_memcpy(void *dest, void *src, size_t n);
mid_uint *mplug_low(mid_uint **dest, mid_uint *src);
mid_uint *mplug_num_low(mid_uint **dest, mid_uint src);

int infiX_dealer(char *num1, char *op_symbol, char *num2);
mid_uint *infiX_division(mid_uint *dividend, mid_uint *divisor);
mid_uint *infiX_subtraction(mid_uint *n1_arr, mid_uint *n2_arr);
mid_uint *infiX_multiplication(mid_uint *n1_arr, mid_uint *n2_arr);
mid_uint *infiX_addition(mid_uint *n1_arr, mid_uint *n2_arr);

#endif /*_INFIX_H_*/
