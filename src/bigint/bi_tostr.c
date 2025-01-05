#include "_bigint_struct.h"
#include "bigint.h"

/**
 * count_digits - calculate how many digits can represent a number.
 * @num: the decimal number.
 *
 * Return: number of digits calculated.
 */
unsigned int count_digits(size_t num)
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
 * bi_tostr - convert a bigint to a numstr.
 * @bi: the bigint.
 * @base: an int between 2-36 indicating the base of the number.
 *
 * Return: a pointer to a numstr, NULL on failure.
 */
char *bi_tostr(bigint *bi, const unsigned int base)
{
	if (!bi || base < 2 || base > 36)
		return (NULL);

	const size_t str_len =
		(bi->len * count_digits(BIGINT_BASE - 1)) + bi->is_negative ? 1 : 0;
	char *const str = xmalloc(str_len + 1);
	if (!str)
		return (NULL);

	size_t str_i = 0;
	if (bi->is_negative)
		str[str_i++] = '-';

	for (size_t bi_i = bi->len; bi_i > 0 && str_i < str_len;)
	{
		--bi_i;
		int bytes_written = sprintf(&str[str_i], "%u", bi->num[bi_i]);
		str_i += bytes_written - 1;
	}

	return (str);
}
