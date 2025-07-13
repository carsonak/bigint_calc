#ifndef CALC_LEXER_H
#define CALC_LEXER_H

#include "lexer_token.h"
#include "reader.h"
#include "tokens_Deque.h"

/* clang-format off */

bool lex_line(Deque_tok *const restrict dq, reader *const restrict r)
_diagnose_if(!dq, "dq should not be a NULL pointer.", "warning")
_diagnose_if(!r, "r should not be a NULL pointer.", "warning");
/* clang-format on */

#endif  // CALC_LEXER_H
