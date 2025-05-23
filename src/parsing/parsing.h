#ifndef PARSING_H
#define PARSING_H

#include "attribute_macros.h"

#include <ctype.h>   /* isdigit */
#include <stdbool.h> /* bool */
#include <stddef.h>  /* size_t, ptr_diff */
#include <stdio.h>   /* *printf */
#include <string.h>  /* strlen, strcpy */

#include "bigint.h"
#include "stack.h"
#include "xalloc.h"

#define ANSI_RED "\x1b[31m"
#define ANSI_NORMAL "\x1b[m"

/* generic prototype for basic arithmetic functions. */
typedef bigint *(math_function)(bigint *, bigint *);

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
	numstr *number;
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
	char const *start;
	union ops op;
} token;

/* shunting_yard */

deque *parse_tokens(char const *const expression);
void print_syntax_error(
	char const *const expression, len_type idx, char const *const msg
);
void *free_token(token *freeable_token);

#endif /* PARSING_H */
