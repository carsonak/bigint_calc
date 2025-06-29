/*!
 * @file
 * @brief String type implementation in C.
 */

#include <string.h>

#include "string_ty.h"
#include "xalloc.h"

/*!
 * @brief initialise a `string_view` from a `string`.
 * @public @memberof string_view
 *
 * @param dest pointer to the `string_view` to initialise.
 * @param src pointer to the `string`.
 * @return pointer to the `string_view`, NULL on failure.
 */
string_view *string_to_string_view(
	string_view *const restrict dest, const string *const restrict src
)
{
	if (!dest || (src && src->len < 0))
		return (NULL);

	*dest = (string_view){0};
	if (src && src->s && src->len > 0)
		*dest = (string_view){.len = src->len, .s = src->s};

	return (dest);
}

/*!
 * @brief initialise a new `string`.
 * @public @memberof string
 *
 * @param s pointer to a string to initialise with.
 * @param s_len number of characters of the string to use for initialisation.
 * @return pointer to the new `string`, NULL on failure.
 */
string *string_new(const char *const restrict s, const len_ty s_len)
{
	if (s_len < 0)
		return (NULL);

	string *const restrict str = xmalloc(sizeof(*str) + s_len + 1);

	if (!str)
		return (NULL);

	*str = (string){.len = s_len};
	if (s)
		strncpy(&str->s[0], s, s_len);

	str->s[s_len] = 0;
	return (str);
}

/*!
 * @brief free a string from memory.
 * @public @memberof string
 *
 * @param freeable_string pointer to the string.
 * @return NULL always.
 */
string *string_delete(string *const restrict freeable_string)
{
	if (freeable_string)
		*freeable_string = (string){0};

	return (xfree(freeable_string));
}

/*!
 * @brief duplicate a `string`.
 * @public @memberof string
 *
 * @param s the string to duplicate.
 * @return pointer to the new `string`, NULL on failure.
 */
string *string_dup(const string_view s)
{
	if (!s.s)
		return (NULL);

	return (string_new(&s.s[0], s.len));
}

/*!
 * @brief concatenate two `string`s.
 * @public @memberof string
 *
 * @param dest first `string`.
 * @param src second `string`.
 * @return pointer to the joined strings, NULL on failure.
 */
string *string_cat(const string_view dest, const string_view src)
{
	if (dest.len < 0 || src.len < 0)
		return (NULL);

	const len_ty s_len = (dest.s ? dest.len : 0) + (src.s ? src.len : 0) + 1;

	if (s_len == 1)
		return (NULL);

	string *const restrict str = string_new(dest.s, dest.len + src.len + 1);

	if (!str)
		return (NULL);

	if (src.s)
		strncpy(&str->s[dest.len], &src.s[0], src.len);

	str->s[str->len] = 0;
	return (str);
}

/*!
 * @brief resize the length of a `string`.
 * @public @memberof string
 *
 * @param s pointer to the `string`.
 * @param s_len new length of the `string`.
 * @return pointer to the resized `string`, NULL on failure.
 */
string *string_resize(string *const restrict s, const len_ty s_len)
{
	if (s_len < 0)
		return (string_delete(s));

	string *const restrict str =
		xrealloc_free_on_fail(s, sizeof(*s) + s_len + 1);

	if (!str)
		return (NULL);

	*str = (string){.len = s_len};
	str->s[s_len] = 0;
	return (str);
}
