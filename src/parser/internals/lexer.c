/*!
 * @file
 * @brief lexer.
 */

#include <assert.h>  // assert
#include <ctype.h>   // isdigit
#include <string.h>  // strcmp

#include "lexer.h"
#include "macros.h"
#include "tokens_Deque.h"

static char skip_spaces(reader *const restrict r) _nonnull;
static char skip_block_comment(reader *const restrict r) _nonnull;
static char skip_line_comment(reader *const restrict r) _nonnull;
static String *
get_id_or_num(reader *const restrict r, const char start) _nonnull;
static String *
get_id(reader *const restrict r, const char start) _nonnull _malloc;
static String *
get_num(reader *const restrict r, const char start) _nonnull _malloc;
static enum lexer_token_type
identifier_type(const char *const restrict id, const len_ty len) _nonnull;
static String *
get_string(reader *const restrict r, const char start) _nonnull _malloc;

static char skip_spaces(reader *const restrict r)
{
	char c1 = reader_peekc(r);

	for (; c1 != EOF && c1 != '\n' && isspace(c1); c1 = reader_peekc(r))
		reader_getc(r);

	return (c1);
}

static char skip_block_comment(reader *const restrict r)
{
	const char *const old_prompt = r->prompt;

	r->prompt = PROMPT_PENDING;
	char c0 = reader_getc(r), c1 = reader_peekc(r);

	for (; c0 != EOF && c1 != EOF && !(c0 == '*' && c1 == '/');
		 c0 = reader_getc(r), c1 = reader_peekc(r))
	{
		if (c0 == '/' && c1 == '*')
		{
			c0 = reader_getc(r);
			c0 = skip_block_comment(r);
			if (c0 != '/')
				return (c0);

			c0 = reader_getc(r);
		}
	}

	r->prompt = old_prompt;
	return (c1);
}

static char skip_line_comment(reader *const restrict r)
{
	char c1 = reader_peekc(r);

	for (; c1 != EOF && c1 != '\n'; c1 = reader_peekc(r))
		reader_getc(r);

	return (c1);
}

static bool is_id_start(const char c) { return (isalpha(c) || c == '_'); }

static bool is_alnum_(const char c) { return (isalnum(c) || c == '_'); }

static String *get_id_or_num(reader *const restrict r, const char start)
{
	char buf[256];
	StringView s1 = {0};
	String *restrict s = NULL;
	unsigned int i = 0;

	buf[i++] = start;
	for (char c1 = reader_peekc(r); c1 != EOF && is_alnum_(c1);
		 c1 = reader_peekc(r))
	{
		if (i >= sizeof(buf) - 1)
		{
			String *const old_s = s;

			s = string_cat(
				*stringview_from_string(&s1, old_s),
				(StringView){.len = i, .s = buf}
			);
			string_delete(old_s);
			i = 0;
			if (!s)
				return (NULL);
		}

		buf[i++] = reader_getc(r);
	}

	String *const old_s = s;

	s = string_cat(
		*stringview_from_string(&s1, old_s), (StringView){.len = i, .s = buf}
	);
	string_delete(old_s);
	return (s);
}

static String *get_id(reader *const restrict r, const char start)
{
	if (!is_id_start(start))
		return (NULL);

	return (get_id_or_num(r, start));
}

static String *get_num(reader *const restrict r, const char start)
{
	if (!isdigit(start))
		return (NULL);

	return (get_id_or_num(r, start));
}

/*!
 * @brief determine the type of identifier in the given string.
 *
 * @param id non-null pointer to the string.
 * @param len number of characters in the string.
 * @return the identifier type.
 */
static enum lexer_token_type
identifier_type(const char *restrict id, const len_ty len)
{
	char c = *id++;

	if (c == 'b')
	{
		if (len == sizeof("break") - 1 && strcmp("reak", id) == 0)
			return (KW_BREAK);
	}
	else if (c == 'e')
	{
		if (len == 4)
		{
			c = *id++;
			if (c == 'l')
			{
				if (strcmp("if", id) == 0)
					return (KW_ELIF);

				if (strcmp("se", id) == 0)
					return (KW_ELSE);
			}
			else if (c == 'x')
			{
				if (strcmp("it", id) == 0)
					return (KW_EXIT);
			}
		}
	}
	else if (c == 'f')
	{
		if (len == sizeof("false") - 1 && strcmp("alse", id) == 0)
			return (KW_FALSE);
		else if (len == sizeof("fn") - 1 && *id == 'n')
			return (KW_FN);
		else if (len == sizeof("for") - 1 && strcmp("or", id) == 0)
			return (KW_FOR);
	}
	else if (c == 'i')
	{
		if (len == 2)
		{
			if (*id == 'f')
				return (KW_IF);
			else if (*id == 'n')
				return (KW_IN);
		}
	}
	else if (c == 'r')
	{
		if (len == sizeof("return") - 1 && strcmp("eturn", id) == 0)
			return (KW_RETURN);
	}
	else if (c == 't')
	{
		if (len == sizeof("true") - 1 && strcmp("rue", id) == 0)
			return (KW_TRUE);
	}
	else if (c == 'w')
	{
		if (len == sizeof("while") - 1 && strcmp("hile", id) == 0)
			return (KW_WHILE);
	}

	return (ID);
}

static String *get_string(reader *const restrict r, const char start)
{
	char buf[256];
	StringView s1 = {0};
	String *restrict s = NULL;
	unsigned int i = 0;

	if (start != '"')
		return (NULL);

	for (char c1 = reader_peekc(r); c1 != EOF && c1 != '\n' && c1 != '"';
		 c1 = reader_peekc(r))
	{
		if (i >= sizeof(buf) - 1)
		{
			String *const old_s = s;

			buf[i] = 0;
			s = string_cat(
				*stringview_from_string(&s1, old_s),
				(StringView){.len = i, .s = buf}
			);
			string_delete(old_s);
			i = 0;
			if (!s)
				return (NULL);
		}

		buf[i++] = reader_getc(r);
	}

	String *const old_s = s;

	buf[i] = 0;
	s = string_cat(
		*stringview_from_string(&s1, old_s), (StringView){.len = i, .s = buf}
	);
	string_delete(old_s);
	return (s);
}

/*!
 * @brief retrieve the next token in the stream.
 *
 * @param tok address to store the token.
 * @param r pointer to a stream reader context struct.
 * @return true on success, false on error.
 */
bool next_token(lexer_token *const restrict tok, reader *const restrict r)
{
	if (!tok || !r || reader_peekc(r) == EOF)
		return (false);

	const char c = reader_getc(r);

	*tok = (lexer_token){.line = r->line, .column = r->column, .id = INVALID};
	if (c == '+')
	{
		tok->id = OP_ADD;
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = ASSIGN_ADD;
		}
	}
	else if (c == '/')
	{
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = ASSIGN_DIV;
		}
		else if (reader_peekc(r) == '*')
		{
			reader_getc(r);
			if (skip_block_comment(r) == '/')
			{
				tok->id = COMMENT_BLOCK;
				reader_getc(r);
			}
		}
		else
			tok->id = OP_DIV;
	}
	else if (c == '%')
	{
		tok->id = OP_MOD;
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = ASSIGN_MOD;
		}
	}
	else if (c == '*')
	{
		tok->id = OP_MUL;
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = ASSIGN_MUL;
		}
	}
	else if (c == '^')
	{
		tok->id = OP_POW;
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = ASSIGN_POW;
		}
	}
	else if (c == '-')
	{
		tok->id = OP_SUB;
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = ASSIGN_SUB;
		}
	}
	else if (c == '=')
	{
		tok->id = ASSIGN;
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = OP_EQ;
		}
	}
	else if (c == '#')
	{
		if (skip_line_comment(r) == '\n')
			tok->id = COMMENT_LINE;
	}
	else if (is_id_start(c))
	{
		String *const restrict identifier = get_id(r, c);

		if (!identifier)
			goto error_cleanup;

		tok->id = identifier_type(identifier->s, identifier->len);
		tok->str = identifier;
		if (tok->id != ID)
			tok->str = string_delete(identifier);
	}
	else if (isdigit(c))
	{
		String *const restrict num = get_num(r, c);

		if (!num)
			goto error_cleanup;

		tok->id = NUM;
		tok->str = num;
	}
	else if (c == '.')
	{
		tok->id = OP_ACCESS;
	}
	else if (c == '>')
	{
		tok->id = OP_GT;
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = OP_GE;
		}
	}
	else if (c == '<')
	{
		tok->id = OP_LT;
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = OP_LE;
		}
	}
	else if (c == '"')
	{
		tok->str = get_string(r, c);
		if (!tok->str)
			goto error_cleanup;

		if (reader_getc(r) != '"')
			tok->str = string_delete(tok->str);
		else
			tok->id = STRING;
	}
	else if (c == '\\')
	{
		tok->id = SYM_BSLASH;
	}
	else if (c == ',')
	{
		tok->id = SYM_COMMA;
	}
	else if (c == '{')
	{
		tok->id = SYM_CURLY_L;
	}
	else if (c == '}')
	{
		tok->id = SYM_CURLY_R;
	}
	else if (c == '(')
	{
		tok->id = SYM_PAREN_L;
	}
	else if (c == ')')
	{
		tok->id = SYM_PAREN_R;
	}
	else if (c == ';')
	{
		tok->id = SYM_SEMICOLON;
	}
	else if (c == '[')
	{
		tok->id = SYM_SQUARE_L;
	}
	else if (c == ']')
	{
		tok->id = SYM_SQUARE_R;
	}
	else
	{
		tok->str = string_new((const char[2]){c}, 1);
		if (!tok->str)
			goto error_cleanup;
	}

	return (true);
error_cleanup:
	return (false);
}

bool lex_line(Deque_tok *const restrict dq, reader *const restrict r)
{
	if (!dq)
		return (false);

	*dq = (Deque_tok){0};
	if (!r)
		return (false);

	for (char c = skip_spaces(r); c != EOF && c != '\n'; c = skip_spaces(r))
	{
		lexer_token tok;

		if (!next_token(&tok, r))
			goto error_cleanup;

		if (!dq_tok_push_tail(dq, tok, NULL))
			goto error_cleanup;
	}

	reader_getc(r);
	return (true);
error_cleanup:
	dq_tok_clear(dq, NULL);
	return (false);
}
