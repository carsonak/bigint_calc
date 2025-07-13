#ifndef DS_STRING_VIEW_H
#define DS_STRING_VIEW_H

#include "compiler_attributes_macros.h"
#include "types.h"

/*!
 * @brief string type.
 */
typedef struct String
{
	/*! @public number of characters in the string，excluding the terminating null byte. */
	len_ty len;
	/*! @public cursor into the string. */
	char *p;
	/*! @public the string. */
	char s[];
} String;

/*!
 * @brief immutable string type.
 */
typedef struct StringView
{
	/*! @public number of characters in the string, excluding the terminating null byte. */
	len_ty len;
	/*! @public pointer to the string. */
	const char *s;
} StringView;

StringView *stringview_from_string(
	StringView *const restrict dest, const String *const restrict src
) _diagnose_if(!dest, "dest should be non-null.", "warning");

void *string_delete(String *const restrict freeable_string);
/* clang-format off */

String *string_new(const char *const restrict s, const len_ty s_len)
_malloc _malloc_free(string_delete)
_diagnose_if(s_len < 0, "s_len should not be negative.", "warning");
String *string_from_stringview(const StringView s) _malloc _malloc_free(string_delete);
String *string_cat(const StringView dest, const StringView src)
_malloc _malloc_free(string_delete);
String *string_resize(String *const restrict s, const len_ty s_len)
_malloc_free(string_delete)
_diagnose_if(s_len < 0, "s_len should not be negative", "warning");
/* clang-format on */

#endif  // DS_STRING_VIEW_H
