/*!
 * @file
 */

#include <stdio.h>

#include "lexer.h"
#include "xalloc.h"

/*! Lexer token names. */
static const char *const restrict token_names[] = {
	[INVALID] = "INVALID",
	[ASSIGN_ADD] = "ASSIGN_ADD",
	[ASSIGN_DIV] = "ASSIGN_DIV",
	[ASSIGN_MOD] = "ASSIGN_MOD",
	[ASSIGN_MUL] = "ASSIGN_MUL",
	[ASSIGN_POW] = "ASSIGN_POW",
	[ASSIGN_SUB] = "ASSIGN_SUB",
	[ASSIGN] = "ASSIGN",
	[COMMENT_BLOCK] = "COMMENT_BLOCK",
	[COMMENT_LINE] = "COMMENT_LINE",
	[ID] = "ID",
	[KW_BREAK] = "KW_BREAK",
	[KW_ELIF] = "KW_ELIF",
	[KW_ELSE] = "KW_ELSE",
	[KW_EXIT] = "KW_EXIT",
	[KW_FALSE] = "KW_FALSE",
	[KW_FN] = "KW_FN",
	[KW_FOR] = "KW_FOR",
	[KW_IF] = "KW_IF",
	[KW_IN] = "KW_IN",
	[KW_RETURN] = "KW_RETURN",
	[KW_TRUE] = "KW_TRUE",
	[KW_WHILE] = "KW_WHILE",
	[NUM] = "NUM",
	[OP_ACCESS] = "OP_ACCESS",
	[OP_ADD] = "OP_ADD",
	[OP_DIV] = "OP_DIV",
	[OP_EQ] = "OP_EQ",
	[OP_GE] = "OP_GE",
	[OP_GT] = "OP_GT",
	[OP_LE] = "OP_LE",
	[OP_LT] = "OP_LT",
	[OP_MOD] = "OP_MOD",
	[OP_MUL] = "OP_MUL",
	[OP_POW] = "OP_POW",
	[OP_SUB] = "OP_SUB",
	[STRING] = "STRING",
	[SYM_BSLASH] = "SYM_BSLASH",
	[SYM_COMMA] = "SYM_COMMA",
	[SYM_CURLY_L] = "SYM_CURLY_L",
	[SYM_CURLY_R] = "SYM_CURLY_R",
	[SYM_PAREN_L] = "SYM_PAREN_L",
	[SYM_PAREN_R] = "SYM_PAREN_R",
	[SYM_SEMICOLON] = "SYM_SEMICOLON",
	[SYM_SQUARE_L] = "SYM_SQUARE_L",
	[SYM_SQUARE_R] = "SYM_SQUARE_R",
};

/*!
 * @brief return the string representation of a `lexer_token`.
 * @public @memberof lexer_token
 *
 * @param token the token to stringify.
 * @return pointer to the string, NULL on failure.
 */
char *lexer_token_tostr(const lexer_token token)
{
	if (token.id < 0 || token.id >= sizeof(token_names) / sizeof(*token_names))
		return (NULL);

	char *restrict s = NULL;

	if (token.id == ID || token.id == NUM)
	{
		const len_ty str_len = snprintf(
			NULL, 0, ":%" PRI_len ":%" PRI_len " %s = %s", token.line,
			token.column, token_names[token.id], token.str->s
		);

		s = xmalloc(str_len + 1);
		if (s)
		{
			sprintf(
				s, ":%" PRI_len ":%" PRI_len " %s = %s", token.line,
				token.column, token_names[token.id], token.str->s
			);
		}
	}
	else if (token.id == STRING)
	{
		const len_ty str_len = snprintf(
			NULL, 0, ":%" PRI_len ":%" PRI_len " %s = \"%s\"", token.line,
			token.column, token_names[token.id], token.str->s
		);

		s = xmalloc(str_len + 1);
		if (s)
		{
			sprintf(
				s, ":%" PRI_len ":%" PRI_len " %s = \"%s\"", token.line,
				token.column, token_names[token.id], token.str->s
			);
		}
	}
	else
	{
		const len_ty str_len = snprintf(
			NULL, 0, ":%" PRI_len ":%" PRI_len " %s", token.line, token.column,
			token_names[token.id]
		);

		s = xmalloc(str_len + 1);
		if (s)
		{
			sprintf(
				s, ":%" PRI_len ":%" PRI_len " %s", token.line, token.column,
				token_names[token.id]
			);
		}
	}

	return (s);
}

/*!
 * @brief allocate memory for a `lexer_token`.
 * @public @memberof lexer_token
 *
 * @return pointer to a new `lexer_token`, NULL on failure.
 */
lexer_token *lexer_token_new(void)
{
	return (xcalloc(1, sizeof(lexer_token)));
}

/*!
 * @brief free memory allocated to a `lexer_token`.
 * @public @memberof lexer_token
 *
 * @param freeable_token pointer to the `lexer_token`.
 * @return NULL always.
 */
void *lexer_token_del(lexer_token *const restrict freeable_token)
{
	if (freeable_token)
	{
		string_delete(freeable_token->str);
		*freeable_token = (lexer_token){.id = INVALID};
	}

	return (xfree(freeable_token));
}
