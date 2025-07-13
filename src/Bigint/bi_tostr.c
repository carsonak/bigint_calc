/*!
 * @file
 * @brief methods for stringifying Bigint types.
 */

#include "_bi_internals.h"
#include "Bigint.h"
#include "number_parsing.h"

#include "count_digits.c"

/*!
 * @brief convert the number represented by a `Bigint` into a string of decimals.
 * @public @memberof Bigint
 *
 * @param[in] n pointer to the `Bigint`.
 *
 * @return pointer to a str, NULL on failure.
 */
char *bi_tostr(Bigint const *const restrict n)
{
	if (!n || n->len < 1 || !n->num)
		return (NULL);

	len_ty str_i = 0, bi_i = n->len - 1;
	const unsigned int digits = count_digits(BIGINT_BASE - 1, 10);
	const len_ty str_len = (n->len * digits) + (n->is_negative ? 1 : 0);
	char *const restrict str = xmalloc(str_len + 1);

	if (!str)
		return (NULL);

	if (n->is_negative && !bi_iszero(n))
		str[str_i++] = '-';

	while (bi_i > 0 && n->num[bi_i] == 0)
		bi_i--;

	str_i += sprintf(&str[str_i], "%" PRI_udigit, n->num[bi_i--]);
	for (; bi_i >= 0 && str_i < str_len; --bi_i)
		str_i += sprintf(&str[str_i], "%.*" PRI_udigit, digits, n->num[bi_i]);

	return (str);
}
