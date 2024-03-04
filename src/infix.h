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

/*Max nnumber of digits for mid_uint*/
#define U32_DIGITS (9)
/*Max size for mid_uint calculations: 10^9*/
#define U32_ROLL (1000000000)
/*Max nnumber of digits for uint64_t*/
#define U64_DIGITS (U32_DIGITS * U32_DIGITS)
/*Max size for uint64_t calculations: 10^18*/
#define U64_ROLL (U32_ROLL * U32_ROLL)
/*mid_uint negative bit toggle*/
#define U32_NEGBIT (1 << 30)

typedef uint64_t hi_uint;
typedef uint32_t mid_uint;
typedef uint8_t lo_uint;

/*A dynamically allocated buffer to store the remainder of division*/
extern mid_uint *remain;

/**
 * struct operations - a structure of an operation and it's function
 * @sign: the operation
 * @f: a pointer to the corresponding function
 */
typedef struct operations
{
	char *sign;
	mid_uint *(*f)(mid_uint *n1, mid_uint *n2);

} op_func;

void print_help(void);
mid_uint *str_u32(lo_uint *num);
lo_uint *u32_str(mid_uint *u32a);
void trim_intarr(mid_uint **arr);
size_t pad_char(char *str, char *ch);
void *memfill(void *mem, char b, size_t start, size_t nbytes);
char *_memcpy(char *dest, char *src, size_t n);
mid_uint *mplug_low(mid_uint **dest, mid_uint *src);
mid_uint *mplug_num_low(mid_uint **dest, mid_uint src);

int infiX_op(char *num1, char *sign, char *num2);
mid_uint *infiX_div(mid_uint *dividend, mid_uint *divisor);
int zero_result_check(mid_uint *dvdend, mid_uint *dvsor, mid_uint **quotient);
hi_uint get_quotient(mid_uint *dvsor);
hi_uint adjust_quotient(mid_uint dvsor_msd, mid_uint *estimate, mid_uint rem_msd, hi_uint quotient_tmp);
mid_uint *infiX_sub(mid_uint *n1_arr, mid_uint *n2_arr);
mid_uint *infiX_mul(mid_uint *n1_arr, mid_uint *n2_arr);
mid_uint *infiX_add(mid_uint *n1_arr, mid_uint *n2_arr);

#endif /*_INFIX_H_*/
