/*!
 * @file
 * @brief String type implementation in C.
 */

#include <stddef.h> /* offsetof */
#include <string.h> /* str* */

#include "String.h"
#include "xalloc.h"

/*!
 * @brief initialise a `StringView` from a `String`.
 * @public @memberof StringView
 *
 * @param dest pointer to the `StringView` to initialise.
 * @param src pointer to the `String`.
 * @return pointer to the `StringView`, NULL on failure.
 */
StringView *stringview_from_string(
	StringView *const restrict dest, const String *const restrict src
)
{
	if (!dest || (src && src->len < 0))
		return (NULL);

	*dest = (StringView){0};
	if (src && src->len > 0)
		*dest = (StringView){.len = src->len, .s = src->s};

	return (dest);
}

/*!
 * @brief initialise a new `String`.
 * @public @memberof String
 *
 * @param s pointer to a string to initialise with.
 * @param s_len number of characters of the string to use for initialisation.
 * @return pointer to the new `String`, NULL on failure.
 */
String *string_new(const char *const restrict s, const len_ty s_len)
{
	if (s_len < 0)
		return (NULL);

	String *const restrict str = xmalloc(offsetof(String, s) + s_len + 1);

	if (!str)
		return (NULL);

	*str = (String){.len = s_len, .p = str->s};
	if (s)
		strncpy(str->s, s, s_len);

	str->s[s_len] = 0;
	return (str);
}

/*!
 * @brief free a `String` from memory.
 * @public @memberof String
 *
 * @param freeable_string pointer to the `String`.
 * @return NULL always.
 */
void *string_delete(String *const restrict freeable_string)
{
	if (freeable_string)
	{
		freeable_string->s[0] = 0;
		*freeable_string = (String){0};
	}

	return (xfree(freeable_string));
}

/*!
 * @brief initialise a `String` from a `StringView`.
 * @public @memberof String
 *
 * @param s the `StringView`.
 * @return pointer to the new `String`, NULL on failure.
 */
String *string_from_stringview(const StringView s)
{
	if (!s.s)
		return (NULL);

	return (string_new(s.s, s.len));
}

/*!
 * @brief concatenate two `String`s.
 * @public @memberof String
 *
 * @param dest first `String`.
 * @param src second `String`.
 * @return pointer to the joined strings, NULL on failure.
 */
String *string_cat(StringView dest, StringView src)
{
	if (dest.len < 0 || src.len < 0 || (!dest.s && !src.s))
		return (NULL);

	src.len = src.s ? src.len : 0;
	dest.len = dest.s ? dest.len : 0;
	String *const restrict str = string_new(dest.s, dest.len + src.len);

	if (!str)
		return (NULL);

	if (src.s)
		strncpy(&str->s[dest.len], src.s, src.len);

	return (str);
}

/*!
 * @brief resize the length of a `String`.
 * @public @memberof String
 *
 * On failure the function frees the old `String`.
 *
 * @param s pointer to the `String`.
 * @param s_len new length of the `String`.
 * @return pointer to the resized `String`, NULL on failure.
 */
String *string_resize(String *const restrict s, const len_ty s_len)
{
	if (s_len < 0)
		return (string_delete(s));

	String *const restrict str = xrealloc(s, offsetof(String, s) + s_len + 1);

	if (!str)
		return (string_delete(s));

	str->len = s_len;
	str->p = str->s;
	str->s[s_len] = 0;
	return (str);
}
