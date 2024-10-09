#ifndef STRING_UTILS
#define STRING_UTILS

#include "macros.h"

#include <ctype.h>	/* isdigit */
#include <stddef.h> /* size_t, ptr_diff */
#include <string.h> /* strlen, strcpy */

#include "xalloc.h"

/*A pointer to a function that maps one character to another.*/
typedef char (*mapping_func)(char);

char *
filter_str(const char *str, size_t *const processed, const mapping_func f);
char int_to_char(unsigned int num);
int char_to_int(char c);
size_t leading_chars_span(const char *str, char *ch);

#endif /*STRING_UTILS*/
