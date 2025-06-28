#ifndef DS_STRING_VIEW_H
#define DS_STRING_VIEW_H

#include "attribute_macros.h"
#include "types.h"

#include <string.h>

/*!
 * @brief slice of a c string.
 */
typedef struct string_view
{
	/*! @public number of characters in the string excluding the terminating null byte. */
	len_ty len;
	/*! @public pointer to the string. */
	const char *s;
} string_view;

/*!
 * @brief string type.
 */
typedef struct string
{
	/*! @public number of characters in the string，excluding the terminating null byte. */
	len_ty len;
	/*! @public the string. */
	char s[];
} string;

#endif  // DS_STRING_VIEW_H
