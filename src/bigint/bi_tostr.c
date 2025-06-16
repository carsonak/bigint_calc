/*!
 * @file
 * @brief methods for stringifying bigint types.
 */

#include "_bi_internals.h"
#include "number_parsing.h"

#include "count_digits.c"

/*!
 * @brief convert the number represented by a `bigint` into a string of decimals.
 * @public @memberof bigint
 *
 * @param[in] n pointer to the `bigint`.
 *
 * @return pointer to a str, NULL on failure.
 */
char *bi_tostr(bigint const *const restrict n)
{
	if (!n || n->len < 1 || !n->num)
		return (NULL);

	len_type str_i = 0, bi_i = n->len - 1;
	const len_type str_len = (n->len * count_digits(BIGINT_BASE - 1, 10)) +
							 (n->is_negative ? 1 : 0);
	char *const restrict str = xmalloc(str_len + 1);

	if (!str)
		return (NULL);

	if (n->is_negative)
		str[str_i++] = '-';

	while (bi_i && !n->num[bi_i])
		--bi_i;

	for (; bi_i > 0 && str_i < str_len; --bi_i)
	{
		int bytes_written = sprintf(&str[str_i], "%" PRIu_int, n->num[bi_i]);

		str_i += bytes_written - 1;
	}

	return (str);
}
