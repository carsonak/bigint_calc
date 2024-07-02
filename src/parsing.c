#include "infiX.h"

static char to_base36(uint num);
static int from_base36(char c);

/*Number base to be used for conversions.*/
uint NUMBASE = 10;

/**
 * parse_str - parse a string of numbers.
 * @str: a string with numbers.
 *
 * Return: pointer to a numstr struct, NULL of failure.
 */
numstr *parse_str(const char *str)
{
	numstr *arr = NULL;
	size_t s_i = 0, b_i = 0;
	const uint buf_size = 2048;
	char *buf = NULL;

	if (!str || (!isdigit(str[0]) && str[0] != '-' && str[0] != '+'))
		return (NULL);

	arr = alloc_numstr(0);
	buf = xmalloc(buf_size);
	if (!buf || !arr)
		goto cleanup_numstr;

	for (s_i = 0; str[s_i] == '-' || str[s_i] == '+'; s_i++)
		if (str[s_i] == '-')
			arr->is_negative = !arr->is_negative;

	s_i += leading_chars_len(&str[s_i], "0");
	while (str[s_i])
	{
		for (b_i = 0; b_i < buf_size - 2 && str[s_i]; s_i++)
		{
			if (str[s_i] == '_' || str[s_i] == ',')
				continue;

			if (str[s_i] == '.')
				break;

			buf[b_i] = from_base36(str[s_i]);
			if (buf[b_i] < 0 || (uint)buf[b_i] >= NUMBASE)
			{
				fprintf(stderr,
						"ParsingError: Invalid character '%c' for base%d\n",
						str[s_i], NUMBASE);
				goto cleanup_numstr;
			}

			buf[b_i] = to_base36(buf[b_i]);
			b_i++;
		}

		buf[b_i] = '\0';
		arr->str = xrealloc(arr->str, arr->len + b_i + 1);
		if (!arr->str)
			goto cleanup_numstr;

		strcpy(&arr->str[arr->len], buf);
		arr->len += b_i;
		if (str[s_i] == '.')
			break;
	}

	if (!arr->str || !arr->str[0])
	{
		fprintf(stderr, "ParsingError: 0 decimal characters found\n");
cleanup_numstr:
		arr = free_numstr(arr);
	}

	free_n_null(buf);
	return (arr);
}

/**
 * leading_chars_len - finds the number of leading characters in a string.
 * @str: the string to check.
 * @ch: the character.
 *
 * Return: number of leading characters.
 */
size_t leading_chars_len(const char *str, char *ch)
{
	size_t count = 0;

	if (str && ch && *ch)
	{
		count = strspn(str, ch);
		if (count && !str[count])
			count--;
	}

	return (count);
}

/**
 * from_base36 - map a base36 ascii symbol to a decimal.
 * @c: an alphanumeric symbol.
 *
 * Return: decimal value of the symbol, -1 if invalid symbols.
 */
int from_base36(char c)
{
	if (!isalnum(c))
		return (-1);

	if (isdigit(c))
		return (c - '0');

	return (toupper(c) - 'A' + 10);
}

/**
 * to_base36 - map a decimal between 0-35 to a base36 ascii symbol.
 * @num: the number to convert.
 *
 * Return: the ascii symbol, '\0' on error.
 */
char to_base36(uint num)
{
	if (num > 35)
		return ('\0');

	if (num < 10)
		return ('0' + num);

	return ('A' + (num - 10));
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
	uint max_digits = 0;
	bignum *arr = NULL;
	char num_buf[36] = {0}, *end = NULL;

	if (!num || !num->len || !num->str || !isalnum(num->str[0]))
		return (NULL);

	/*sizeof(bignum) == ceil(numstr.len / max_digits of base in BIGNUM_UINT_MAX)*/
	max_digits = count_digits(BIGNUM_UINT_MAX, NUMBASE);
	a_i = (num->len / max_digits);
	if (num->len % max_digits)
		a_i++;

	arr = alloc_bignum(a_i);
	if (!arr)
		return (NULL);

	/*Ensure atleast BIGNUM_UINT_MAX is captured on each iteration.*/
	max_digits++;
	for (a_i = 0, n_i = 0; a_i < arr->len && n_i < num->len; a_i++)
	{
		strncpy(num_buf, &num->str[n_i], max_digits);
		num_buf[max_digits] = '\0';
		tmp += strtol(num_buf, &end, NUMBASE);
		if (*end)
		{
			fprintf(stderr,
					"ParsingError: Invalid character '%c' for base%d\n",
					*end, NUMBASE);
			return (free_bignum(arr));
		}

		arr->num[a_i] = tmp % BIGNUM_UINT_MAX;
		tmp /= BIGNUM_UINT_MAX;
		n_i += max_digits;
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
 *
 * Return: a pointer to a numstr, NULL on failure.
 */
numstr *bignum_to_numstr(bignum *arr)
{
	(void)arr;
	return (NULL);
}

/**
 * print_bignum - print a bignum.
 * @arr: pointer to the array struct.
 *
 * Return: number of bytes printed, -1 on error.
 */
size_t print_bignum(bignum *arr)
{
	char *str_arr = NULL;
	size_t bytes_printed = 0;

	if (!arr)
		return (-1);

	str_arr = uint_array_to_str(arr->num, arr->len);
	if (!str_arr)
		return (-1);

	if (arr->is_negative)
		bytes_printed = printf("-%s\n", str_arr);
	else
		bytes_printed = printf("%s\n", str_arr);

	free_n_null(str_arr);
	return (bytes_printed);
}

/**
 * uint_array_to_str - represent an uint array as a string.
 * @arr: the uint array.
 * @len: number of items in the uint.
 *
 * Return: pointer to a string, NULL on error.
 */
char *uint_array_to_str(const uint *arr, size_t len)
{
	size_t s_i = 0, n = 0, len_sep = 0, len_str = 0;
	int bytes_written = 0;
	char *str = NULL, *sep = ", ";

	if (!arr || !len)
		return (xstrdup("{NULL}"));

	len_sep = strlen(sep);
	/*sizeof(str) == (max digits in 4 bytes * len) + */
	/*total sizeof(separators) + sizeof("{}") + 1*/
	len_str = (count_digits(UINT_MAX, 10) * len) +
			  (len_sep * (len - 1)) + 2 + 1;
	str = xmalloc(len_str * sizeof(*str));
	if (!str)
		return (NULL);

	strcpy(str, "{");
	for (n = 0, s_i = 1; n < len - 1 && s_i < len_str; n++)
	{
		bytes_written = sprintf(&str[s_i], "%d", arr[n]);
		if (bytes_written < 0)
			return (free_n_null(str));

		s_i += bytes_written;
		strcpy(&str[s_i], sep);
		s_i += len_sep;
	}

	if (sprintf(&str[s_i], "%d}", arr[n]) < 0)
		return (free_n_null(str));

	return (xrealloc(str, strlen(str) + 1));
}

/**
 * count_digits - calculate how many digits of the given base can represent the given decimal.
 * @num: the decimal number.
 * @base: the number base.
 *
 * Return: digits calculated.
 */
uint count_digits(size_t num, uint base)
{
	int d = 0;

	while (num)
	{
		d++;
		num /= base;
	}

	return (d);
}

/**
 * get_base - return the current number base.
 *
 * Return: the current base.
 */
uint get_base(void) { return (NUMBASE); }

/**
 * set_base - update the number base.
 * @base: a value between 2-36.
 *
 * Return: the updated base on success, 0 on failure.
 */
uint set_base(uint base)
{
	if (!(base >= 2 && base <= 36))
		return (0);

	NUMBASE = base;
	return (NUMBASE);
}
