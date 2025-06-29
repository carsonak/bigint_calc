/*!
 * @file
 * @brief lexer.
 */

#include <ctype.h>  // isdigit
#include <string.h>  // strcmp

#include "lexer.h"

static bool is_id_start(const char c) { return (isalpha(c) || c == '_'); }

static bool is_alnum_(const char c) { return (isalnum(c) || c == '_'); }

static string *get_id(reader *const restrict r)
{
	char buf[128] = {0};
	string_view s1 = {0};
	string *restrict s = NULL;
	unsigned int i = 0;

	while (!feof(r->stream) && is_alnum_(reader_peekc(r)))
	{
		if (i >= sizeof(buf) - 1)
		{
			string *old_s = s;

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

	string *old_s = s;

	s = string_cat(
		*string_to_string_view(&s1, old_s), (string_view){.len = i, .s = buf}
	);
	string_delete(old_s);
	return (s);
}

static string *get_num(reader *const restrict r)
{
	char buf[256] = {0};
	string_view s1 = {0};
	string *restrict s = NULL;
	unsigned int i = 0;

	while (!feof(r->stream) && is_alnum_(reader_peekc(r)))
	{
		if (i >= sizeof(buf) - 1)
		{
			string *old_s = s;

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

	string *old_s = s;

	s = string_cat(
		*string_to_string_view(&s1, old_s), (string_view){.len = i, .s = buf}
	);
	string_delete(old_s);
	return (s);
}

static char skip_block_comment(reader *const restrict r)
{
	for (char c = reader_getc(r);
		 !feof(r->stream) && !(c == '*' && reader_peekc(r) == '/');
		 c = reader_getc(r))
	{
		if (c == '/' && reader_peekc(r) == '*')
		{
			reader_getc(r);
			c = skip_block_comment(r);
			if (c != '/')
				return (c);
		}
	}

	return (reader_getc(r));
}

static char skip_line_comment(reader *const restrict r)
{
	char c = reader_getc(r);

	for (; !feof(r->stream) && c != '\n';)
		c = reader_getc(r);

	return (c);
}

static string *get_string(reader *const restrict r)
{
	char buf[256] = {0};
	string_view s1 = {0};
	string *restrict s = NULL;
	unsigned int i = 0;

	while (!feof(r->stream) && reader_peekc(r) != '"')
	{
		if (i >= sizeof(buf) - 1)
		{
			string *old_s = s;

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

	string *old_s = s;

	s = string_cat(
		*string_to_string_view(&s1, old_s), (string_view){.len = i, .s = buf}
	);
	string_delete(old_s);
	return (s);
}

bool next_token(lexer_token *const restrict tok, reader *const restrict r)
{
	if (!tok)
		return (false);

	*tok = (lexer_token){.id = INVALID};
	if (!r || feof(r->stream))
		return (false);

	char c = reader_peekc(r);

	if (c == EOF)
		return (false);

	while (isspace(c))
	{
		reader_getc(r);
		c = reader_peekc(r);
	}

	tok->line = r->line;
	tok->column = r->column;
	tok->offset = ftell(r->stream);
	if (c == '+')
	{
		reader_getc(r);
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = ASSIGN_ADD;
		}
		else
			tok->id = OP_ADD;
	}
	else if (c == '/')
	{
		reader_getc(r);
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
				return (false);
		}
		else
			tok->id = OP_DIV;
	}
	else if (c == '%')
	{
		reader_getc(r);
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = ASSIGN_MOD;
		}
		else
			tok->id = OP_MOD;
	}
	else if (c == '*')
	{
		reader_getc(r);
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = ASSIGN_MUL;
		}
		else
			tok->id = OP_MUL;
	}
	else if (c == '^')
	{
		reader_getc(r);
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = ASSIGN_POW;
		}
		else
			tok->id = OP_POW;
	}
	else if (c == '-')
	{
		reader_getc(r);
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = ASSIGN_SUB;
		}
		else
			tok->id = OP_SUB;
	}
	else if (c == '=')
	{
		reader_getc(r);
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = OP_EQ;
		}
		else
			tok->id = ASSIGN;
	}
	else if (c == '#')
	{
		reader_getc(r);
		tok->id = COMMENT_LINE;
		if (skip_line_comment(r) != '\n')
			return (false);
	}
	else if (is_id_start(c))
	{
		string *const restrict id = get_id(r);

		if (!id)
			return (false);

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
			return (true);
		}

		string_delete(id);
	}
	else if (isdigit(c))
	{
		string *const restrict num = get_num(r);

		if (!num)
			return (false);

		tok->id = NUM;
		tok->str = num;
	}
	else if (c == '.')
	{
		reader_getc(r);
		tok->id = OP_ACCESS;
	}
	else if (c == '>')
	{
		reader_getc(r);
		tok->id = OP_GT;
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = OP_GE;
		}
	}
	else if (c == '<')
	{
		reader_getc(r);
		tok->id = OP_LT;
		if (reader_peekc(r) == '=')
		{
			reader_getc(r);
			tok->id = OP_LE;
		}
	}
	else if (c == '"')
	{
		reader_getc(r);
		tok->id = STRING;
		tok->str = get_string(r);
		if (!tok->str)
			return (false);

		if (reader_getc(r) != '"')
			return (false);
	}
	else if (c == '\\')
	{
		reader_getc(r);
		tok->id = SYM_BSLASH;
	}
	else if (c == ',')
	{
		reader_getc(r);
		tok->id = SYM_COMMA;
	}
	else if (c == '{')
	{
		reader_getc(r);
		tok->id = SYM_CURLY_L;
	}
	else if (c == '}')
	{
		reader_getc(r);
		tok->id = SYM_CURLY_R;
	}
	else if (c == '(')
	{
		reader_getc(r);
		tok->id = SYM_PAREN_L;
	}
	else if (c == ')')
	{
		reader_getc(r);
		tok->id = SYM_PAREN_R;
	}
	else if (c == ';')
	{
		reader_getc(r);
		tok->id = SYM_SEMICOLON;
	}
	else if (c == '[')
	{
		reader_getc(r);
		tok->id = SYM_SQUARE_L;
	}
	else if (c == ']')
	{
		reader_getc(r);
		tok->id = SYM_SQUARE_R;
	}
	else
		return (false);

	return (true);
}
