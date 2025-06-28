/*!
 * @file
 * @brief String type implementation in C.
 */

#include "string_ty.h"
#include "xalloc.h"

/*!
 * @brief initialise a `string_view` from a `string`.
 *
 * @param s pointer to the `string`.
 * @return a string_view.
 */
string_view string_to_string_view(const string *const restrict s)
{
	if (!s || s->len < 0)
		return ((string_view){0});

	return ((string_view){.len = s->len, .s = &s->s[0]});
}

/*!
 * @brief initialise a new `string`.
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

	str->len = s_len;
	if (s)
		strncpy(&str->s[0], s, s_len);

	str->s[s_len] = 0;
	return (str);
}

/*!
 * @brief duplicate a `string`.
 *
 * @param s the string to duplicate.
 * @return pointer to the new `string`, NULL on failure.
 */
string *string_dup(const string_view s)
{
	return (string_new(&s.s[0], s.len));
}

/*!
 * @brief concatenate two `string`s.
 *
 * @param dest first string.
 * @param src second string.
 * @return pointer to the joined strings.
 */
string *string_cat(const string_view dest, const string_view src)
{
	if (dest.len < 0 || src.len < 0 || (dest.len > 0 && !dest.s) ||
		(src.len > 0 && !src.s))
		return (NULL);

	string *const restrict str = string_new(dest.s, dest.len + src.len + 1);

	if (!str)
		return (NULL);

	if (src.s)
		strncpy(&str->s[dest.len], &src.s[0], src.len);

	str->s[str->len] = 0;
	return (str);
}
