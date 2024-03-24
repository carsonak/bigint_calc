#ifndef _INFIX_H_
#define _INFIX_H_

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/

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

	typedef uint64_t l_uint;
	typedef uint32_t m_uint;
	typedef uint8_t s_uchar;
	/* math_func: a function that takes in 2 int arrays and returns an int array*/
	typedef m_uint *math_func(m_uint *, m_uint *);

/*Max number of digits m_uint should hold*/
#define MID_MAX_DIGITS (9)
/*Max size for m_uint: 10^9*/
#define MID_MAX_VAL (1000000000)
/*Max nnumber of digits for uint64_t*/
#define HI_MAX_DIGITS (MID_MAX_DIGITS * MID_MAX_DIGITS)
/*Max size for uint64_t calculations: 10^18*/
#define HI_MAX_VAL (MID_MAX_VAL * MID_MAX_VAL)
/*m_uint negative bit toggle*/
#define NEGBIT_UI32 (1 << 30)

	/*A dynamically allocated buffer to store remainders of division*/
	extern m_uint *remain;

	/**
	 * struct operator_function - holds an operator symbol and it's function
	 * @symbol: the operator symbol
	 * @f: a corresponding function pointer
	 */
	typedef struct operator_function
	{
		char *symbol;
		math_func *f;
	} op_func;

	/**
	 * struct numstring_attributes - holds details about a string of numbers
	 * @str: the number string
	 * @len: length of the string
	 * @digits: number of digits in the string
	 * @is_positive: atleast one bit should be set if positive
	 */
	typedef struct numstring_attributes
	{
		s_uchar *str;
		size_t len;
		size_t digits;
		s_uchar is_positive;
	} str_attr;

	void panic(const char *err_type);
	void helpme(const char *which_help);
	int parse_numstr(str_attr *numstr);
	m_uint *str_to_intarray(s_uchar *num);
	s_uchar *intarr_to_str(m_uint *u32a);
	void trim_intarr(m_uint *arr);
	size_t pad_char(char *str, char *ch);
	int _strcmp(const char *s1, const char *s2);
	void *memfill(void *mem, char b, size_t start, size_t nbytes);
	void *_memcpy(void *dest, void *src, size_t n);
	m_uint *mplug_low(m_uint **dest, m_uint *src);
	m_uint *mplug_num_low(m_uint **dest, m_uint src);

	char *infiX_manager(char *num1, char *op_symbol, char *num2);
	m_uint *infiX_division(m_uint *dividend, m_uint *divisor);
	m_uint *infiX_subtraction(m_uint *n1_arr, m_uint *n2_arr);
	m_uint *infiX_multiplication(m_uint *n1_arr, m_uint *n2_arr);
	m_uint *infiX_addition(m_uint *n1_arr, m_uint *n2_arr);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*_INFIX_H_*/
