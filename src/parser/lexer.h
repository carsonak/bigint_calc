#ifndef BIGINT_LEXER_H
#define BIGINT_LEXER_H

#include "bigint.h"
#include "deque.h"
#include "number_parsing.h"
#include "xalloc.h"

/* generic prototype for basic arithmetic functions. */
typedef bigint *(math_function)(bigint *, bigint *);

/**
 * enum token_type - tags for all the components of an expression.
 * @NUMBER: a number.
 * @OP_ADD: an addition operator.
 * @OP_DIV: a division operator.
 * @OP_MOD: a modulo operator.
 * @OP_MUL: a multiplication operator.
 * @OP_POW: an exponentiation operator.
 * @OP_SUB: a subtraction operator.
 * @PAREN_L: a left parenthesis.
 * @PAREN_R: a right parenthesis.
 * @INVALID: an invalid character.
 */
enum token_type
{
	NUMBER,
	OP_ADD,
	OP_DIV,
	OP_MOD,
	OP_MUL,
	OP_POW,
	OP_SUB,
	PAREN_L,
	PAREN_R,
	INVALID
};

/**
 * union component - union of components of math expressions.
 * @function: pointer to an operator function.
 * @number: pointer to a number.
 */
union component
{
	math_function *function;
	numstr *restrict number;
};

/**
 * struct lexer_token - a token of a math expression.
 * @id: the type of the token.
 * @s_i: pointer to the first character of the token in the original string.
 * @itm: function / number / component of a math expression.
 */
typedef struct lexer_token
{
	enum token_type id;
	const char *restrict s_i;
	union component itm;
} lex_tok;

void token_del(void *freeable_token);
void *token_new(const void *const token);

#endif /* BIGINT_LEXER_H */
