#include "parsing.h"

/**
 * count_digits - calculate how many digits can represent a number.
 * @num: the decimal number.
 *
 * Return: number of digits calculated.
 */
unsigned int count_digits(size_t num)
{
	int d = 0;

	while (num)
	{
		d++;
		num /= 10;
	}

	return (d);
}

/**
 * str_to_numstr - parse a string of digits.
 * @number_str: a string with a number.
 * @base: an int between 2-36 indicating the base of the number.
 * @processed: address to store number of characters processed.
 *
 * Return: pointer to a numstr struct, NULL of failure.
 */
numstr *str_to_numstr(const char *number_str, unsigned int base, size_t *processed)
{
	numstr *arr = NULL;
	size_t str_i = 0, buf_i = 0;
	const unsigned int buf_size = 1024;
	char *buf = NULL;
	int c = 0;

	if (!number_str || base < 2 || base > 36)
		return (NULL);

	arr = alloc_numstr(0);
	buf = xmalloc(buf_size);
	if (!buf || !arr)
		goto cleanup_numstr;

	for (str_i = 0; number_str[str_i] == '-' || number_str[str_i] == '+'; str_i++)
		if (number_str[str_i] == '-')
			arr->is_negative = !arr->is_negative;

	if (number_str[str_i] == '_')
	{
		fprintf(stderr, "ParsingError: Leading underscores not allowed.\n");
		goto cleanup_numstr;
	}

	str_i += leading_chars_len(&number_str[str_i], "0");
	while (number_str[str_i])
	{
		for (buf_i = 0; buf_i < buf_size - 2 && number_str[str_i]; str_i++)
		{
			if (number_str[str_i] == '_')
				continue;

			c = char_to_int(number_str[str_i]);
			if (c < 0 || (unsigned int)c >= base)
				break;

			buf[buf_i] = toupper(number_str[str_i]);
			buf_i++;
		}

		buf[buf_i] = '\0';
		arr->str = xrealloc(arr->str, arr->len + buf_i + sizeof(*arr->str));
		if (!arr->str)
			goto cleanup_numstr;

		strcpy(&arr->str[arr->len], buf);
		arr->len += buf_i;
		if (c < 0 || (unsigned int)c >= base)
			break;
	}

	if (!arr->str || !arr->str[0])
	{
		fprintf(stderr, "ParsingError: string did not contain any valid digits.\n");
		goto cleanup_numstr;
	}

	if (number_str[str_i - 1] == '_')
	{
		fprintf(stderr, "ParsingError: Trailing underscores not allowed.\n");
cleanup_numstr:
		arr = free_numstr(arr);
	}

	if (processed)
		*processed = str_i + 1;

	free_n_null(buf);
	return (arr);
}

/**
 * numstr_to_bignum - convert a numstr to a bignum.
 * @num: the numstr.
 *
 * Return: a pointer to a bignum struct, NULL on failure.
 */
bignum *numstr_to_bignum(numstr *num)
{
	size_t a_i = 0, n_i = 0, tmp = 0;
	unsigned int digits = 0;
	bignum *arr = NULL;
	char num_buf[16], *end = NULL;

	if (!num || !num->len || !num->str || !isalnum(num->str[0]))
		return (NULL);

	/*sizeof(bignum) == */
	/*ceil(numstr.len / no. of digits that can represent BIGNUM_UINT_MAX)*/
	digits = count_digits(BIGNUM_UINT_MAX - 1);
	a_i = (num->len / digits);
	if (num->len % digits)
		a_i++;

	arr = bn_alloc(a_i);
	if (!arr)
		return (NULL);

	for (a_i = 0, n_i = num->len; a_i < arr->len && n_i; a_i++)
	{
		if (n_i >= digits)
			n_i -= digits;
		else
		{
			digits = n_i;
			n_i = 0;
		}

		strncpy(num_buf, &num->str[n_i], digits);
		num_buf[digits] = '\0';
		tmp += strtoull(num_buf, &end, 10);
		if (*end)
		{
			fprintf(stderr, "ParsingError: Invalid character '%c'\n", *end);
			return (bn_free(arr));
		}

		arr->num[a_i] = tmp % BIGNUM_UINT_MAX;
		tmp /= BIGNUM_UINT_MAX;
	}

	while (a_i < arr->len && tmp)
	{
		arr->num[a_i] = tmp % BIGNUM_UINT_MAX;
		tmp /= BIGNUM_UINT_MAX;
		a_i++;
	}

	trim_bignum(arr);
	return (arr);
}

/**
 * bignum_to_numstr - convert a bignum to a numstr.
 * @arr: the bignum.
 * @base: an int between 2-36 indicating the base of the number.
 *
 * Return: a pointer to a numstr, NULL on failure.
 */
numstr *bignum_to_numstr(bignum *arr, unsigned int base)
{
	size_t g = 0, h = 0;
	int bytes_written = 0;
	numstr *num = NULL;

	if (!arr || base < 2 || base > 36)
		return (NULL);

	num = alloc_numstr(arr->len * count_digits(BIGNUM_UINT_MAX - 1) + 1);
	for (g = arr->len, h = 0; num && g > 0 && h < num->len;)
	{
		--g;
		bytes_written = sprintf(&num->str[h], "%u", arr->num[g]);
		h += bytes_written - 1;
	}

	return (num);
}
