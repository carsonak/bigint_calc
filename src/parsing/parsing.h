#ifndef PARSING_H
#define PARSING_H

#include "macros.h"

#include <ctype.h>	 /* isdigit */
#include <stdio.h>	 /* *printf */
#include <stdbool.h> /* bool */
#include <stddef.h>	 /* size_t, ptr_diff */
#include <string.h>	 /* strlen, strcpy */

#include "xalloc.h"
#include "bignum_math.h"
#include "data_structures.h"

#define ANSI_RED "\x1b[31m"
#define ANSI_NORMAL "\x1b[m"

/**
 * struct numstr - a string representing a number.
 * @len: length of the string.
 * @is_negative: a bool indicating signedness of the number.
 * @str: the string.
 */
typedef struct numstr
{
	size_t len;
	bool is_negative;
	char *str;
} numstr;

/* generic prototype for basic arithmetic functions. */
typedef bignum * (math_function)(bignum *, bignum *);

/**
 * enum token_type - tags for all the components of an expression.
 * @NUMBER: a number.
 * @PAREN_L: a left parenthesis.
 * @PAREN_R: a right parenthesis.
 * @ADD_OP: an addition operator.
 * @SUB_OP: a subtraction operator.
 * @MUL_OP: a multiplication operator.
 * @DIV_OP: a division operator.
 * @MOD_OP: a modulo operator.
 * @POW_OP: an exponentiation operator.
 * @INVALID: an invalid character.
 */
enum token_type
{
	NUMBER,
	PAREN_L,
	PAREN_R,
	ADD_OP,
	SUB_OP,
	MUL_OP,
	DIV_OP,
	MOD_OP,
	POW_OP,
	INVALID
};

/**
 * union ops - union of numbers and operator functions.
 * @number: pointer to a number.
 * @operator: pointer to an operator function.
 */
union ops
{
	math_function *operator;
	numstr number;
};

/**
 * struct token - a token of a math expression.
 * @id: the type of the token.
 * @start: pointer to the start of the token in the original string.
 * @op: a pointer to a number or operator function.
 */
typedef struct token
{
	enum token_type id;
	const char *start;
	union ops op;
} token;

/* alloc_funcs */

void *
free_numstr(numstr *freeable_ptr);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_numstr)
numstr *alloc_numstr(size_t len);

/* text_funcs */

char int_to_char(unsigned int num);
int char_to_int(char c);
size_t leading_chars_len(const char *str, char *ch);
unsigned int count_digits(size_t num);

numstr *str_to_numstr(const char *str, unsigned int base, size_t *processed);
bignum *numstr_to_bignum(numstr *num);
numstr *bignum_to_numstr(bignum *arr, unsigned int base);
bignum *anybase_to_bignum(numstr *num, unsigned int base);

size_t print_bignum(bignum *arr);
char *uintarray_to_str(const uint *arr, size_t len);

/* shunting_yard */
deque *parse_tokens(const char *expression);
void print_syntax_error(const char *expression, size_t idx, const char *msg);
void *free_token(void *freeable_token);

#endif /* PARSING_H */
