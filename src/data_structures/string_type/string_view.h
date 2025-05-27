#ifndef DS_STRING_VIEW_H
#define DS_STRING_VIEW_H

#include "types.h"

/**
 * struct string_view - slice of a c string.
 * @len: number of characters in the string excluding the terminating null byte.
 * @s: pointer to the first character in the string.
 */
typedef struct string_view
{
	len_type len;
	const char *s;
} string_view;

#endif /* DS_STRING_VIEW_H */
