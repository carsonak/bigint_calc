#ifndef TEXT_PROCESSING_H
#define TEXT_PROCESSING_H

#include "macros.h"

#include <ctype.h>   /* isdigit */
#include <stdio.h>	 /* *printf */
#include <stdbool.h> /* bool */
#include <stddef.h>  /* size_t, ptr_diff */
#include <string.h>  /* strlen, strcpy */

#include "xalloc.h"
#include "bignum_math.h"

/**
 * struct numstr - a string of numbers.
 * @len: length of the string.
 * @is_negative: a bool for signedness of the number.
 * @str: the number string.
 */
typedef struct numstr
{
	size_t len;
	bool is_negative;
	char *str;
} numstr;

/* generic prototype for basic arithmetic functions. */
typedef bignum *(math_function)(bignum *, bignum *);

/**
 * struct operator_function - holds an operator symbol and it's function.
 * @symbol: the operator symbol.
 * @f: a corresponding function pointer.
 */
typedef struct operator_function
{
	char *symbol;
	math_function *f;
} op_func;

/* alloc_funcs */

void *free_numstr(numstr *freeable_ptr);
ATTR_MALLOC ATTR_MALLOC_FREE(free_numstr)
numstr *alloc_numstr(size_t len);

/* text_funcs */

numstr *parse_number(const char *str, unsigned int base);
size_t leading_chars_len(const char *str, char *ch);
bignum *numstr_to_bignum(numstr *num, unsigned int base);
numstr *bignum_to_numstr(bignum *arr, unsigned int base);
size_t print_bignum(bignum *arr);
char *uint_array_to_str(const uint *arr, size_t len);
unsigned int count_digits(size_t num, unsigned int base);

#endif /* TEXT_PROCESSING_H */
