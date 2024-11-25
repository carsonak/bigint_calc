#include "parsing.h"

/**
 * match_token - tokenises characters in a string.
 * @str: the string.
 * @processed: address to store number of characters processed.
 *
 * Return: pointer to a token, NULL on failure.
 */
static token *match_token(const char *str, size_t *processed)
{
	token *t = NULL;
	numstr *n = NULL;

	if (!str || !*str)
		return (NULL);

	t = xmalloc(sizeof(*t));
	if (!t)
		return (NULL);

	if (processed)
		*processed = 1;

	if (*str == '(')
		*t = (token){.id = PAREN_L, .start = str, .op = {NULL}};
	else if (*str == ')')
		*t = (token){.id = PAREN_R, .start = str, .op = {NULL}};
	else if (*str == '+')
		*t = (token){.id = ADD_OP, .start = str, .op = {bni_add}};
	else if (*str == '-')
		*t = (token){.id = SUB_OP, .start = str, .op = {bni_subtract}};
	else if (*str == '*')
		*t = (token){.id = MUL_OP, .start = str, .op = {bni_multiply}};
	else if (*str == '/')
		*t = (token){.id = DIV_OP, .start = str, .op = {bni_divide}};
	else if (*str == '%')
		*t = (token){.id = MOD_OP, .start = str, .op = {bni_modulo}};
	else if (*str == '^')
		*t = (token){.id = POW_OP, .start = str, .op = {bni_power}};
	else if (isdigit(*str))
	{
		n = str_to_numstr(str, 10, processed);
		if (n)
			*t = (token){.id = NUMBER, .start = str, .op.number = n};
		else
			t = free_n_null(t);
	}
	else
		*t = (token){.id = INVALID, .start = str, .op = {NULL}};

	return (t);
}

/**
 * format_lexing_error_msg - logs a lexing error to standard error.
 * @expression: pointer to whole expression.
 * @idx: index of the first character where error occurred.
 * @msg: message to print.
 *
 * Takes a snapshot of 10 or so characters around the given index, then places
 * a coloured caret underneath the character at index and prints to standard
 * error with the given message.
 *
 * Example:
 *  `format_lexing_error_msg("(123 + 34957,02347) / 2", 12, "invalid character");`
 *  output:
 *  `ParsingError: invalid character:
 *  ...34957,02347...
 *          ^`
 */
static void format_lexing_error_msg(
	const char *expression, size_t idx, const char *msg)
{
	char slice[12] = {0}, highlight[12] = {0};
	int i = 0, upper = 0;

	if (!expression)
		return;

	memset(highlight, ' ', 11);
	upper = idx > 5 ? 5 : (int)idx;
	highlight[upper] = '^';
	for (i = 0; i < upper; ++i)
		slice[i] = expression[idx - upper + i];

	for (i = 0; expression[idx + i] && i < 6; ++i)
		slice[upper + i] = expression[idx + i];

	fprintf(stderr, "ParsingError:");
	if (msg)
		fprintf(stderr, "%s:", msg);

	fprintf(
		stderr, "\n...%s...\n   " ANSI_RED "%s" ANSI_NORMAL, slice, highlight);
}

/**
 * free_token - frees a token.
 * @freeable_token: pointer to a token struct.
 *
 * Return: NULL always.
 */
void *free_token(token *freeable_token)
{
	if (freeable_token && freeable_token->id == NUMBER)
		freeable_token->op.number = free_numstr(freeable_token->op.number);

	freeable_token->id = INVALID;
	return (free_n_null(freeable_token));
}

/**
 * lex_str - breaks down a math expression into tokens.
 * @str: a math expression.
 *
 * Return: a deque of tokens on success, NULL otherwise.
 */
deque *lex_str(const char *str)
{
	deque *tokens = NULL;
	token *t = NULL;
	size_t processed = 0, i = 0;

	if (!str)
		return (NULL);

	tokens = xcalloc(1, sizeof(*tokens));
	if (!tokens)
		return (NULL);

	while (str[i])
	{
		while (str[i] == ' ')
			++i;

		t = match_token(&str[i], &processed);
		if (!t)
			break;

		push_tail(tokens, t);
		if (t->id == INVALID)
		{
			format_lexing_error_msg(str, i, "invalid character");
			break;
		}

		i += processed;
	}

	if (str[i])
	{
		clear_deque(tokens, (void *(*)(void *))free_token);
		tokens = free_n_null(tokens);
	}

	return (tokens);
}
