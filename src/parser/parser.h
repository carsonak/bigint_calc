#ifndef PARSING_H
#define PARSING_H

#include <stdio.h> /* fprintf */

#include "bigint.h"
#include "deque.h"
#include "linked_node.h"
#include "number_parsing.h"
#include "xalloc.h"

#define ANSI_RED "\x1b[31m"
#define ANSI_NORMAL "\x1b[m"

deque *parse_tokens(deque *const restrict tokens);

#endif /* PARSING_H */
