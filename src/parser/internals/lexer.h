#ifndef BIGINT_LEXER_H
#define BIGINT_LEXER_H

#include "deque.h"
#include "lexer_token.h"

void token_del(void *freeable_token);
void *token_new(const void *const token);
deque *lex_str(const char *restrict str);

#endif  // BIGINT_LEXER_H
