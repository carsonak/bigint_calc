/*!
 * @file
 * @brief lexer.
 */

#include <ctype.h>  // isdigit
#include <string.h>  // strcmp

#include "lexer.h"
#include "list_type_structs.h"
#include "macros.h"

static bool is_id_start(const char c) { return (isalpha(c) || c == '_'); }

static bool is_alnum_(const char c) { return (isalnum(c) || c == '_'); }

static char skip_spaces(reader *const restrict r)
{
	char c1 = reader_peekc(r);

	for (; c1 != EOF && c1 != '\n' && isspace(c1); c1 = reader_peekc(r))
		reader_getc(r);

	return (c1);
}

static string *get_id_or_num(reader *const restrict r, const char start)
{
	char buf[256];
	string_view s1 = {0};
	string *restrict s = NULL;
	unsigned int i = 0;

	buf[i++] = start;
	for (char c1 = reader_peekc(r); c1 != EOF && is_alnum_(c1);
		 c1 = reader_peekc(r))
	{
		if (i >= sizeof(buf) - 1)
		{
			string *const old_s = s;

			s = string_cat(
				*string_to_string_view(&s1, old_s),
				(string_view){.len = i, .s = buf}
			);
			string_delete(old_s);
			i = 0;
			if (!s)
				return (NULL);
		}

		buf[i++] = reader_getc(r);
	}

	string *const old_s = s;

	s = string_cat(
		*string_to_string_view(&s1, old_s), (string_view){.len = i, .s = buf}
	);
	string_delete(old_s);
	return (s);
}

static string *get_id(reader *const restrict r, const char start)
{
	if (!is_id_start(start))
		return (NULL);

	return (get_id_or_num(r, start));
}

static string *get_num(reader *const restrict r, const char start)
{
	if (!isdigit(start))
		return (NULL);

	return (get_id_or_num(r, start));
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

static string *get_string(reader *const restrict r, const char start)
{
	char buf[256];
	string_view s1 = {0};
	string *restrict s = NULL;
	unsigned int i = 0;

	if (start != '"')
		return (NULL);

	for (char c1 = reader_peekc(r); c1 != EOF && c1 != '\n' && c1 != '"';
		 c1 = reader_peekc(r))
	{
		if (i >= sizeof(buf) - 1)
		{
			string *const old_s = s;

			buf[i] = 0;
			s = string_cat(
				*string_to_string_view(&s1, old_s),
				(string_view){.len = i, .s = buf}
			);
			string_delete(old_s);
			i = 0;
			if (!s)
				return (NULL);
		}

		buf[i++] = reader_getc(r);
	}

	string *const old_s = s;

	buf[i] = 0;
	s = string_cat(
		*string_to_string_view(&s1, old_s), (string_view){.len = i, .s = buf}
	);
	string_delete(old_s);
	return (s);
}

static void free_lexer_token(void *token_ptr)
{
	lexer_token_delete(token_ptr);
}

bool next_token(lexer_token *const restrict tok, reader *const restrict r)
{
	if (!tok || !r || reader_peekc(r) == EOF)
		return (false);

	const char c = reader_getc(r);

	*tok = (lexer_token){.line = r->line,
						 .column = r->column,
						 .offset = ftell(r->stream),
						 .id = INVALID};
	if (tok->offset < 0)
		perror("ERROR: ftell");

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
		tok->id = OP_DIV;
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = ASSIGN_DIV;
		}
		else if (reader_peekc(r) == '*')
		{
			reader_getc(r);
			tok->id = COMMENT_BLOCK;
			if (skip_block_comment(r) != '/')
				goto error_cleanup;

			reader_getc(r);
		}
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
		tok->id = COMMENT_LINE;
		if (skip_line_comment(r) != '\n')
			goto error_cleanup;
	}
	else if (is_id_start(c))
	{
		string *const restrict id = get_id(r, c);

		if (!id)
			goto error_cleanup;

		if (strcmp(id->s, "break") == 0)
			tok->id = KW_BREAK;
		else if (strcmp(id->s, "def") == 0)
			tok->id = KW_DEF;
		else if (strcmp(id->s, "elif") == 0)
			tok->id = KW_ELIF;
		else if (strcmp(id->s, "else") == 0)
			tok->id = KW_ELSE;
		else if (strcmp(id->s, "exit") == 0)
			tok->id = KW_EXIT;
		else if (strcmp(id->s, "false") == 0)
			tok->id = KW_FALSE;
		else if (strcmp(id->s, "for") == 0)
			tok->id = KW_FOR;
		else if (strcmp(id->s, "if") == 0)
			tok->id = KW_IF;
		else if (strcmp(id->s, "IN") == 0)
			tok->id = KW_IN;
		else if (strcmp(id->s, "return") == 0)
			tok->id = KW_RETURN;
		else if (strcmp(id->s, "true") == 0)
			tok->id = KW_TRUE;
		else if (strcmp(id->s, "while") == 0)
			tok->id = KW_WHILE;
		else
		{
			tok->id = ID;
			tok->str = id;
		}

		if (tok->id != ID)
			string_delete(id);
	}
	else if (isdigit(c))
	{
		string *const restrict num = get_num(r, c);

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
		tok->id = STRING;
		tok->str = get_string(r, c);
		if (!tok->str)
			goto error_cleanup;

		if (reader_getc(r) != '"')
		{
			tok->str = string_delete(tok->str);
			goto error_cleanup;
		}
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

bool lex_line(deque *const restrict dq, reader *const restrict r)
{
	if (!dq)
		return (false);

	*dq = (deque){0};
	if (!r)
		return (false);

	for (char c = skip_spaces(r); c != EOF && c != '\n'; c = skip_spaces(r))
	{
		lexer_token *const restrict tok = lexer_token_new();

		if (!tok || !next_token(tok, r))
		{
			lexer_token_delete(tok);
			goto error_cleanup;
		}

		if (!dq_push_tail(dq, tok, NULL))
			goto error_cleanup;
	}

	reader_getc(r);
	return (true);
error_cleanup:
	dq_clear(dq, free_lexer_token);
	return (false);
}
