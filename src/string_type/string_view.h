#ifndef DS_STRING_VIEW_H
#define DS_STRING_VIEW_H

#include "types.h"

/*!
 * @brief slice of a c string.
 */
typedef struct string_view
{
	/*! @public number of characters in the string excluding the terminating null byte. */
	len_ty len;
	/*! @public pointer to the first character in the string. */
	const char *s;
} string_view;

/*!
 * @brief string type.
 */
typedef struct string
{
	len_ty len;
	len_ty cursor;
	char s[];
};

#endif  // DS_STRING_VIEW_H
