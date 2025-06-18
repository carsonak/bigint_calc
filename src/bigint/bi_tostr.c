/*!
 * @file
 * @brief methods for stringifying bigint types.
 */

#include "_bi_internals.h"
#include "bigint.h"
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
	const unsigned int digits = count_digits(BIGINT_BASE - 1, 10);
	const len_type str_len = (n->len * digits) + (n->is_negative ? 1 : 0);
	char *const restrict str = xmalloc(str_len + 1);

	if (!str)
		return (NULL);

	if (n->is_negative && !bi_iszero(n))
		str[str_i++] = '-';

	while (bi_i > 0 && n->num[bi_i] == 0)
		--bi_i;

	str_i += sprintf(&str[str_i], "%" PRIu_int, n->num[bi_i--]);
	for (; bi_i >= 0 && str_i < str_len; --bi_i)
		str_i += sprintf(&str[str_i], "%.*" PRIu_int, digits, n->num[bi_i]);

	return (str);
}
