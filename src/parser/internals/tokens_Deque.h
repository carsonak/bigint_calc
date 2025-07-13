#ifndef CALC_TOKENS_DEQUE_H
#define CALC_TOKENS_DEQUE_H

#include "Deque_struct_macros.h"
#include "lexer_token.h"

DEFINE_DEQUE_STRUCT(tok, lexer_token)

#include "Deque_prototypes_macros.h"

DECLARE_DEQUE_PROTOTYPES(tok, lexer_token)

#endif /* CALC_TOKENS_DEQUE_H */
