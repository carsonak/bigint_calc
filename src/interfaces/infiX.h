#ifndef INFIX_PROJ_API_H
#define INFIX_PROJ_API_H

#include "macros.h"

#include "xalloc.h"
#include "bignum_math.h"
#include "text_processing.h"

void panic(const char *err_type);
void help_me(const char *which_help);

#endif /*INFIX_PROJ_API_H*/
