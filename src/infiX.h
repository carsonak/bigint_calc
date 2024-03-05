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
#define MID_NEGBIT (1 << 30)

/*A dynamically allocated buffer to store the remainder of division*/
extern mid_uint *remain;

/**
 * struct operator_function - a structure of an operation and it's function
 * @sign: the operation
 * @f: a pointer to the corresponding function
 */
typedef struct operator_function
{
	char *sign;
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

int infiX_op(char *num1, char *sign, char *num2);
mid_uint *infiX_div(mid_uint *dividend, mid_uint *divisor);
int zero_result_check(mid_uint *dvdend, mid_uint *dvsor, mid_uint **quotient);
int64_t get_quotient(mid_uint *dvsor);
int64_t adjust_quotient(mid_uint dvsor_msd, mid_uint *estimate, mid_uint rem_msd, int64_t quotient_tmp);
mid_uint *infiX_sub(mid_uint *n1_arr, mid_uint *n2_arr);
mid_uint *infiX_mul(mid_uint *n1_arr, mid_uint *n2_arr);
mid_uint *infiX_add(mid_uint *n1_arr, mid_uint *n2_arr);

#endif /*_INFIX_H_*/
