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

/*Max nnumber of digits for mid_int*/
#define U32_DIGITS (9)
/*Max size for mid_int calculations: 10^9*/
#define U32_ROLL (1000000000)
/*Max nnumber of digits for uint64_t*/
#define U64_DIGITS (U32_DIGITS * U32_DIGITS)
/*Max size for uint64_t calculations: 10^18*/
#define U64_ROLL (U32_ROLL * U32_ROLL)
/*mid_int negative bit toggle*/
#define U32_NEGBIT (1 << 30)

typedef uint64_t hi_int;
typedef uint32_t mid_int;
typedef uint8_t lo_int;

/*A dynamically allocated buffer to store the remainder of division*/
extern mid_int *remain;

/**
 * struct operations - a structure of an operation and it's function
 * @sign: the operation
 * @f: a pointer to the corresponding function
 */
typedef struct operations
{
	char *sign;
	mid_int *(*f)(mid_int *n1, mid_int *n2);

} op_func;

void print_help(void);
mid_int *str_u32(lo_int *num);
lo_int *u32_str(mid_int *u32a);
void trim_intarr(mid_int **arr);
size_t pad_char(char *str, char *ch);
void *memfill(void *mem, char b, size_t start, size_t nbytes);
char *_memcpy(char *dest, char *src, size_t n);
mid_int *mplug_low(mid_int **dest, mid_int *src);
mid_int *mplug_num_low(mid_int **dest, mid_int src);

int infiX_op(char *num1, char *sign, char *num2);
mid_int *infiX_div(mid_int *dividend, mid_int *divisor);
int zero_result_check(mid_int *dend, mid_int *sor, mid_int **qt);
hi_int get_quotient(mid_int *dvsor);
hi_int adjust_q(mid_int ds_msd, mid_int *m_est, mid_int rem_msd, hi_int q_tmp);
mid_int *infiX_sub(mid_int *n1_arr, mid_int *n2_arr);
mid_int *infiX_mul(mid_int *n1_arr, mid_int *n2_arr);
mid_int *infiX_add(mid_int *n1_arr, mid_int *n2_arr);

#endif /*_INFIX_H_*/
