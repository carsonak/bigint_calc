/*!
 * @file
 * @brief parser.
 */

#include "parser.h"
#include "lexer.h"
#include "list_type_structs.h"
#include "xalloc.h"

static char *stringify_lexer_token(const void *const token_ptr)
{
	if (!token_ptr)
		return (NULL);

	const lexer_token t = *(const lexer_token *)token_ptr;
	return (lexer_token_tostr(t));
}

static void free_lexer_token(void *token_ptr)
{
	lexer_token_delete(token_ptr);
}

void parse(reader *const restrict r)
{
	if (!r)
		return;

	deque tokens = {0};

	while (!feof(r->stream))
	{
		if (!lex_line(&tokens, r))
			return;

		const char *restrict tok_str =
			dq_tostr(&tokens, stringify_lexer_token);

		if (tok_str)
			printf("%s\n", tok_str);

		xfree((void *)tok_str);
		dq_clear(&tokens, free_lexer_token);
		if (!tok_str)
			return;
	}

	printf("Done!\n");
}
