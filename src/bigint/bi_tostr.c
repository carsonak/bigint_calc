#include "_bigint_internals.h"
#include "bigint.h"

/**
 * count_digits - calculate how many digits can represent a number.
 * @num: the decimal number.
 *
 * Return: number of digits calculated.
 */
static unsigned int count_digits(size_t num)
{
	unsigned int d = 0;

	while (num)
	{
		++d;
		num /= 10;
	}

	return (d);
}

/**
 * bi_tostr - return the number represented by a bigint as a string.
 * @n: pointer to the bigint.
 *
 * Return: a pointer to a str, NULL on failure.
 */
char *bi_tostr(bigint const *const n)
{
	if (!n || !n->len || !n->num)
		return (NULL);

	const size_t str_len =
		(n->len * count_digits(BIGINT_BASE - 1)) + n->is_negative ? 1 : 0;
	char *const str = xmalloc(str_len + 1);

	if (!str)
		return (NULL);

	size_t str_i = 0;
	size_t bi_i = n->len - 1;

	if (n->is_negative)
		str[str_i++] = '-';

	while (bi_i && !n->num[bi_i])
		--bi_i;

	for (; bi_i > 0 && str_i < str_len; --bi_i)
	{
		int bytes_written = sprintf(&str[str_i], "%u", n->num[bi_i]);

		str_i += bytes_written - 1;
	}

	return (str);
}
