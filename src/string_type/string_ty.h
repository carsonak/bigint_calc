#ifndef DS_STRING_VIEW_H
#define DS_STRING_VIEW_H

#include "attribute_macros.h"
#include "types.h"

#include <string.h>

/*!
 * @brief string type.
 */
typedef struct string
{
	/*! @public number of characters in the string，excluding the terminating null byte. */
	len_ty len;
	/*! @public cursor into the string. */
	len_ty i;
	/*! @public the string. */
	char s[];
} string;

/*!
 * @brief immutable string type.
 */
typedef struct string_view
{
	/*! @public number of characters in the string, excluding the terminating null byte. */
	len_ty len;
	/*! @public pointer to the string. */
	const char *s;
} string_view;

string_view *string_to_string_view(
	string_view *const restrict dest, const string *const restrict src
);

string *string_delete(string *const restrict freeable_string);
/* clang-format off */
string *string_new(const char *const restrict s, const len_ty s_len)
_malloc _malloc_free(string_delete)
_diagnose_if(s_len < 0, "Negative lengths are not allowed", "warning");
string *string_dup(const string_view s) _malloc _malloc_free(string_delete);
string *string_cat(const string_view dest, const string_view src)
_malloc _malloc_free(string_delete);
string *string_resize(string *const restrict s, const len_ty s_len)
_malloc_free(string_delete)
_diagnose_if(s_len < 0, "Negative lengths are not allowed", "warning");
/* clang-format on */

#endif  // DS_STRING_VIEW_H
