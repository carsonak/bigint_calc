#ifndef BIGINT_LEXER_H
#define BIGINT_LEXER_H

#include "deque.h"
#include "lexer_token.h"
#include "reader.h"

/* clang-format off */

bool lex_line(deque *const restrict dq, reader *const restrict r)
_diagnose_if(dq == NULL, "dq should not be a NULL pointer.", "warning")
_diagnose_if(r == NULL, "r should not be a NULL pointer.", "warning");
/* clang-format on */

#endif  // BIGINT_LEXER_H
