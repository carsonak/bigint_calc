#ifndef BIGINT_CALC_LEXER_TOKEN_H
#define BIGINT_CALC_LEXER_TOKEN_H

#include "string_view.h"

/**
 * enum token_type - tags for all the components of an expression.
 * @ID: an identifier.
 * @KW_EXIT: the exit keyword.
 * @NUMBER: a number.
 * @OP_ADD: '+' operator.
 * @OP_DIV: '/' operator.
 * @OP_MOD: '%' operator.
 * @OP_MUL: '*' operator.
 * @OP_POW: '^' operator.
 * @OP_SUB: '-' operator.
 * @SYM_ASSIGN: '=' symbol.
 * @SYM_BCOMMENT: '/*' symbol.
 * @SYM_LCOMMENT: '#' symbol.
 * @SYM_PAREN_L: '(' symbol.
 * @SYM_PAREN_R: ')' symbol.
 * @SYM_SQUARE_L: '[' symbol.
 * @SYM_SQUARE_R: ']' symbol.
 * @SYM_STATEMENT_END: ';' or '\n' symbol.
 * @INVALID: an invalid character.
 */
enum token_type
{
	ID,
	KW_EXIT,
	NUMBER,
	OP_ADD,
	OP_DEC,
	OP_DIV,
	OP_INC,
	OP_MOD,
	OP_MUL,
	OP_POW,
	OP_SUB,
	SYM_ASSIGN,
	SYM_BCOMMENT,
	SYM_LCOMMENT,
	SYM_PAREN_L,
	SYM_PAREN_R,
	SYM_SQUARE_L,
	SYM_SQUARE_R,
	SYM_STATEMENT_END,
	INVALID,
};

/**
 * struct lexer_token - a token of a math expression.
 * @str: slice of the the string where token was found.
 * @id: the type of the token.
 */
typedef struct lexer_token
{
	string_view str;
	enum token_type id;
} lexer_token;

#endif /* BIGINT_CALC_LEXER_TOKEN_H */
