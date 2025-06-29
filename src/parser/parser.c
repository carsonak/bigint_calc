/*!
 * @file
 * @brief parser.
 */

#include "parser.h"
#include "lexer.h"
#include "xalloc.h"

void parse(reader *const restrict r)
{
	if (!r)
		return;

	while (!feof(r->stream))
	{
		lexer_token tok = {0};

		if (!next_token(&tok, r))
			return;

		const char *restrict tok_str = lexer_token_tostr(tok);

		tok.str = string_delete(tok.str);
		if (!tok_str)
			return;

		printf("%s\n", tok_str);
		tok_str = xfree((void *)tok_str);
	}

	printf("Done!\n");
}
