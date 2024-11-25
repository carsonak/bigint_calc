#ifndef STRING_UTILS
#define STRING_UTILS

#include "macros.h"

#include <ctype.h>	/* isdigit */
#include <stddef.h> /* size_t, ptr_diff */
#include <string.h> /* strlen, strcpy */

#include "xalloc.h"

/**
 * mapping_func - a pointer to a function that maps one character to another.
 * @c: the character to be transformed.
 * @args: pointer to more arguments that can be passed to the function.
 *
 * Return: the transformed character.
 */
typedef char (*mapping_func)(const char c, const void *args);

char *filter_str(const char *str, size_t *const processed,
				 const mapping_func f, const void *f_args);
char int_to_char(const unsigned int num);
int char_to_int(const char c);
size_t leading_chars_span(const char *str, const char *ch);

#endif /*STRING_UTILS*/
