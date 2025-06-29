#ifndef BIGINT_LEXER_H
#define BIGINT_LEXER_H

#include "lexer_token.h"
#include "reader.h"

char *lexer_token_tostr(lexer_token token) _malloc;

/* clang-format off */
bool next_token(lexer_token *const restrict tok, reader *const restrict r)
_diagnose_if(tok == NULL, "tok should not be a NULL pointer.", "warning")
_diagnose_if(r == NULL, "r should not be a NULL pointer.", "warning");
/* clang-format on */

#endif  // BIGINT_LEXER_H
