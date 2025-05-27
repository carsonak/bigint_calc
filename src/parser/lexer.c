#include <ctype.h>  /* isdigit */
#include <string.h> /* memset */

#include "lexer.h"

/**
 * token_new - allocate and initialise memory for `lex_tok`.
 * @token: data to initialise with.
 *
 * Return: pointer to the token, NULL on failure.
 */
void *token_new(const void *const token)
{
	lex_tok *const t = xcalloc(1, sizeof(*t));

	if (t && token)
		*t = *(const lex_tok *)token;

	return (t);
}

/**
 * token_del - frees a lex_tok.
 * @freeable_token: pointer to a lex_tok struct.
 */
void token_del(void *freeable_token)
{
	lex_tok *const tok = freeable_token;

	if (tok && tok->id == NUMBER)
		tok->itm.number = free_numstr(tok->itm.number);

	tok->id = INVALID;
	xfree(tok);
}

/**
 * lex_str - breaks down a math expression into tokens.
 * @str: a math expression.
 *
 * Return: a deque of tokens on success, NULL otherwise.
 */
deque *lex_str(const char *restrict str)
{
	if (!str)
		return (NULL);

	deque *restrict tokens = dq_new();

	if (!tokens)
		return (NULL);

	for (; *str; ++str)
	{
		lex_tok tok = {0};

		if (isdigit(*str))
		{
			len_type processed = 0;
			bigint *n = _str_to_numstr(str, 10, &processed);

			if (!n)
				break;

			/* `processed` should be greater than 0 unless a malloc error occurred */
			/* in the number parser. */
			str += --processed;
			tok = (lex_tok){.id = NUMBER, .s_i = str, .itm.number = n};
		}
		else if (*str == '+')
			tok = (lex_tok){.id = OP_ADD, .s_i = str, .itm = {bi_add}};
		else if (*str == '/')
			tok = (lex_tok){.id = OP_DIV, .s_i = str, .itm = {bi_divide}};
		else if (*str == '%')
			tok = (lex_tok){.id = OP_MOD, .s_i = str, .itm = {bi_modulo}};
		else if (*str == '*')
			tok = (lex_tok){.id = OP_MUL, .s_i = str, .itm = {bi_multiply}};
		else if (*str == '^')
			tok = (lex_tok){.id = OP_POW, .s_i = str, .itm = {bi_power}};
		else if (*str == '-')
			tok = (lex_tok){.id = OP_SUB, .s_i = str, .itm = {bi_subtract}};
		else if (*str == '(')
			tok = (lex_tok){.id = PAREN_L, .s_i = str, .itm = {NULL}};
		else if (*str == ')')
			tok = (lex_tok){.id = PAREN_R, .s_i = str, .itm = {NULL}};
		else if (*str == ' ' || *str == '\t')
			continue;
		else
		{
			fprintf(stderr, "Parsing Error: Invalid character '%c'.\n", *str);
			break;
		}

		if (!dq_push_tail(tokens, &tok, token_new))
			break;
	}

	if (*str)
		tokens = dq_del(tokens, token_del);

	return (tokens);
}
