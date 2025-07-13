/*!
 * @file
 * @brief parser.
 */

#include "parser.h"
#include "lexer.h"
#include "tokens_Deque.h"
#include "xalloc.h"

static void lexer_token_clean(lexer_token tok) { string_delete(tok.str); }

void parse(reader *const restrict r)
{
	if (!r)
		return;

	Deque_tok tokens = {0};

	while (!feof(r->stream))
	{
		if (!lex_line(&tokens, r))
			return;

		char *restrict tok_str = dq_tok_tostr(&tokens, lexer_token_tostr);

		if (tok_str)
			printf("%s\n", tok_str);

		xfree(tok_str);
		dq_tok_clear(&tokens, lexer_token_clean);
		if (!tok_str)
			return;
	}

	printf("Done!\n");
}
