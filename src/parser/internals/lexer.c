/*!
 * @file
 * @brief lexer.
 */

#include <ctype.h>  // isdigit

#include "lexer.h"

static len_ty block_comment_len(const char *const str) _nonnull;
static len_ty identifier_len(const char *const str) _nonnull;
static len_ty line_comment_len(const char *const str) _nonnull;
static len_ty number_len(const char *const str) _nonnull;
static len_ty sign_len(const char *const str) _nonnull;

/*!
 * @brief allocate and initialise memory for `lexer_token`.
 * @public @memberof lexer_token
 *
 * @param[in] token data to initialise with.
 *
 * @return pointer to the token, NULL on failure.
 */
void *token_new(const void *const token)
{
	lexer_token *const restrict t = xcalloc(1, sizeof(*t));

	if (t && token)
		*t = *(const lexer_token *)token;

	return (t);
}

/*!
 * @brief frees a `lexer_token`.
 * @public @memberof lexer_token
 *
 * @param[in] freeable_token pointer to a `lexer_token`.
 */
void token_del(void *freeable_token)
{
	lexer_token *const t = freeable_token;

	t->id = INVALID;
	t->str = (string_view){0};
	xfree(t);
}

/*!
 * @brief count the characters in a string representing a number.
 *
 * @param[in] str pointer to the first character in the number.
 *
 * @return number of characters in the number, 0 if none.
 */
static len_ty number_len(const char *const str)
{
	len_ty i = 0;

	if (!isdigit(*str))
		return (0);

	for (; isalnum(str[i]) || str[i] == '_'; ++i)
		;

	return (i);
}

/*!
 * @brief count the characters in a string representing an identifier.
 *
 * @param[in] str pointer to the first character in the identifier.
 *
 * @return number of characters in the identifier, 0 if none.
 */
static len_ty identifier_len(const char *const str)
{
	len_ty i = 0;

	if (!isalpha(*str) || *str != '_')
		return (0);

	for (; isalnum(str[i]) || str[i] == '_'; ++i)
		;

	return (i);
}

/*!
 * @brief count number of consecutive '+' and '-' in a string.
 *
 * @param[in] str pointer to the first sign character.
 *
 * @return number of consecutive sign characters, 0 if none.
 */
static len_ty sign_len(const char *const str)
{
	len_ty i = 0;

	for (; str[i] == '+' || str[i] == '-'; ++i)
		;

	return (i);
}

/*!
 * @brief count the characters that make up a block comment in a string.
 *
 * @param[in] str pointer to the starting symbols of a block comment.
 *
 * @return number of characters in a the block comment, 0 if none.
 */
static len_ty block_comment_len(const char *const str)
{
	len_ty i = 2;

	if (!(str[0] == '/' && str[1] == '*'))
		return (0);

	for (; str[i] && !(str[0] == '*' && str[1] == '/'); ++i)
		;

	if (str[0] == '*' && str[1] == '/')
		i += 2;

	return (i);
}

/*!
 * @brief count the characters that make up a line comment in a string.
 *
 * @param[in] str pointer to the starting symbol of a line comment.
 *
 * @return number of characters in the line comment, 0 if none.
 */
static len_ty line_comment_len(const char *const str)
{
	len_ty i = 1;

	if (*str != '#')
		return (0);

	for (; str[i] && str[i] != '\n'; ++i)
		;

	return (i);
}

/*!
 * @brief break down a string into tokens.
 * @relatesalso lexer_token
 *
 * @param[in] str pointer to the string.
 *
 * @return a `deque` of tokens on success, NULL otherwise.
 */
lexer_token next_token(FILE *const stream)
{
	lexer_token tok = {0};

	if (!str)
		return ((lexer_token){0});

	const char KEYWORD_EXIT[] = "exit";

	for (; *str; ++str)
	{
		string_view slice = {.len = 1, .s = str};

		if (strncmp(str, KEYWORD_EXIT, sizeof(KEYWORD_EXIT) - 1) == 0 &&
			!isalnum(str[sizeof(KEYWORD_EXIT) - 1]) &&
			str[sizeof(KEYWORD_EXIT) - 1] != '_')
		{
			t = (lexer_token){.id = KW_EXIT, .str = slice};
			t.str.len = sizeof(KEYWORD_EXIT) - 1;
			str += sizeof(KEYWORD_EXIT) - 2;
			goto early_exit;
		}
		else if (isalpha(*str) || *str == '_')
		{
			t = (lexer_token){.id = ID, .str = slice};
			t.str.len = identifier_len(str);
			str += (t.str.len - 1);
		}
		else if (isdigit(*str))
		{
			t = (lexer_token){.id = NUMBER, .str = slice};
			t.str.len = number_len(str);
			str += (t.str.len - 1);
		}
		else if (*str == '+')
		{
			t = (lexer_token){.id = OP_ADD, .str = slice};
			t.str.len = sign_len(str);
			str += (t.str.len - 1);
			if (isalpha(str[1]) || str[1] == '_')
			{
				const len_ty id_len = identifier_len(++str);

				t.id = ID;
				t.str.len += id_len;
				str += (id_len - 1);
			}
			else if (isdigit(str[1]))
			{
				const len_ty num_len = number_len(++str);

				t.id = NUMBER;
				t.str.len += num_len;
				str += (num_len - 1);
			}
			else if (t.str.len == 2 && *str == '+')
				t.id = OP_INC;
			else
			{
				t.id = INVALID;
				goto early_exit;
			}
		}
		else if (*str == '/')
			t = (lexer_token){.id = OP_DIV, .str = slice};
		else if (*str == '%')
			t = (lexer_token){.id = OP_MOD, .str = slice};
		else if (*str == '*')
			t = (lexer_token){.id = OP_MUL, .str = slice};
		else if (*str == '^')
			t = (lexer_token){.id = OP_POW, .str = slice};
		else if (*str == '-')
		{
			t = (lexer_token){.id = OP_SUB, .str = slice};
			t.str.len = sign_len(str);
			str += (t.str.len - 1);
			if (isalpha(str[1]) || str[1] == '_')
			{
				const len_ty id_len = identifier_len(++str);

				t.id = ID;
				t.str.len += id_len;
				str += (id_len - 1);
			}
			else if (isdigit(str[1]))
			{
				const len_ty num_len = number_len(++str);

				t.id = NUMBER;
				t.str.len += num_len;
				str += (num_len - 1);
			}
			else if (t.str.len == 2 && *str == '-')
				t.id = OP_DEC;
			else
			{
				t.id = INVALID;
				goto early_exit;
			}
		}
		else if (*str == '=')
			t = (lexer_token){.id = SYM_ASSIGN, .str = slice};
		else if (str[0] == '/' && str[1] == '*')
		{
			t = (lexer_token){.id = SYM_COMMENT_B, .str = slice};
			t.str.len = block_comment_len(str);
			str += (t.str.len - 1);
			if (!(*(str - 1) == '*' && *str == '/'))
			{
				t.id = INVALID;
				goto early_exit;
			}
		}
		else if (*str == '#')
		{
			t = (lexer_token){.id = SYM_LCOMMENT, .str = slice};
			t.str.len = line_comment_len(str);
			str += (t.str.len - 1);
		}
		else if (*str == '(')
			t = (lexer_token){.id = SYM_PAREN_L, .str = slice};
		else if (*str == ')')
			t = (lexer_token){.id = SYM_PAREN_R, .str = slice};
		else if (*str == '[')
			t = (lexer_token){.id = SYM_SQUARE_L, .str = slice};
		else if (*str == ']')
			t = (lexer_token){.id = SYM_SQUARE_R, .str = slice};
		else if (*str == ';')
			t = (lexer_token){.id = SYM_SEMICOLON, .str = slice};
		else if (*str == '\n')
			t = (lexer_token){.id = SYM_NEWLINE, .str = slice};
		else if (isspace(*str))
			continue;
		else
		{
			t = (lexer_token){.id = INVALID, .str = slice};
			goto early_exit;
		}

		if (!dq_push_tail(tokens, &t, token_new))
			break;
	}

	if (*str)
	{
alloc_fail:
		tokens = dq_del(tokens, token_del);
	}

	if (0)
	{
early_exit:
		if (!dq_push_tail(tokens, &t, token_new))
			goto alloc_fail;
	}

	return (tokens);
}
