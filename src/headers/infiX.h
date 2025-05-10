#ifndef INFIX_PROJ_API_H
#define INFIX_PROJ_API_H

#include "macros.h"

#include "xalloc.h"
#include "bigint_math.h"
#include "parsing.h"

void panic(char const *const err_type);
void help_me(char const *const which_help);

#endif /* INFIX_PROJ_API_H */
