/*!
 * @file
 */

#include <stdio.h>

#include "lexer.h"
#include "xalloc.h"

static const char *const restrict token_names[] = {
	"INVALID",       "ASSIGN_ADD",   "ASSIGN_DIV",  "ASSIGN_MOD",
	"ASSIGN_MUL",    "ASSIGN_POW",   "ASSIGN_SUB",  "ASSIGN",
	"COMMENT_BLOCK", "COMMENT_LINE", "ID",          "KW_BREAK",
	"KW_DEF",        "KW_ELIF",      "KW_ELSE",     "KW_EXIT",
	"KW_FALSE",      "KW_FOR",       "KW_IF",       "KW_IN",
	"KW_RETURN",     "KW_TRUE",      "KW_WHILE",    "NUM",
	"OP_ACCESS",     "OP_ADD",       "OP_DIV",      "OP_EQ",
	"OP_GE",         "OP_GT",        "OP_LE",       "OP_LT",
	"OP_MOD",        "OP_MUL",       "OP_POW",      "OP_SUB",
	"STRING",        "SYM_BSLASH",   "SYM_COMMA",   "SYM_CURLY_L",
	"SYM_CURLY_R",   "SYM_PAREN_L",  "SYM_PAREN_R", "SYM_SEMICOLON",
	"SYM_SQUARE_L",  "SYM_SQUARE_R",
};

char *lexer_token_tostr(lexer_token token)
{
	if (token.id < 0 || token.id >= sizeof(token_names) / sizeof(*token_names))
		return (NULL);

	char *restrict s = NULL;

	if (token.id == ID || token.id == NUM)
	{
		const len_ty str_len = snprintf(
			NULL, 0, "%" PRI_len "%" PRI_len " %s -> %s", token.line,
			token.column, token_names[token.id], token.str->s
		);

		s = xmalloc(str_len + 1);
		if (s)
		{
			sprintf(
				s, "%" PRI_len "%" PRI_len " %s -> %s", token.line,
				token.column, token_names[token.id], token.str->s
			);
		}
	}
	else if (token.id == STRING)
	{
		const len_ty str_len = snprintf(
			NULL, 0, "%" PRI_len "%" PRI_len " %s -> \"%s\"", token.line,
			token.column, token_names[token.id], token.str->s
		);

		s = xmalloc(str_len + 1);
		if (s)
		{
			sprintf(
				s, "%" PRI_len "%" PRI_len " %s -> \"%s\"", token.line,
				token.column, token_names[token.id], token.str->s
			);
		}
	}
	else
	{
		const len_ty str_len = snprintf(
			NULL, 0, "%" PRI_len "%" PRI_len " %s", token.line, token.column,
			token_names[token.id]
		);

		s = xmalloc(str_len + 1);
		if (s)
		{
			sprintf(
				s, "%" PRI_len "%" PRI_len " %s", token.line, token.column,
				token_names[token.id]
			);
		}
	}

	return (NULL);
}
