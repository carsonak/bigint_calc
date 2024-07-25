#include "text_processing.h"

static char int_to_char(unsigned int num);
static int char_to_int(char c);
static bignum *anybase_to_bignum(numstr *num, unsigned int base) ATTR_NONNULL;

/**
 * parse_number - parse a string of numbers.
 * @number_str: a string with a number.
 * @base: an int between 2-36 indicating the base of the number.
 *
 * Return: pointer to a numstr struct, NULL of failure.
 */
numstr *parse_number(const char *number_str, unsigned int base)
{
	numstr *arr = NULL;
	size_t s_i = 0, b_i = 0;
	const size_t buf_size = 1024;
	char *buf = NULL;

	if (!number_str || base < 2 || base > 36)
		return (NULL);

	arr = alloc_numstr(0);
	buf = xmalloc(buf_size);
	if (!buf || !arr)
		goto cleanup_numstr;

	for (s_i = 0; number_str[s_i] == '-' || number_str[s_i] == '+'; s_i++)
		if (number_str[s_i] == '-')
			arr->is_negative = !arr->is_negative;

	if (number_str[s_i] == '_' || number_str[s_i] == ',')
	{
		fprintf(stderr, "ParsingError: Leading separators not allowed.\n");
		goto cleanup_numstr;
	}

	s_i += leading_chars_len(&number_str[s_i], "0");
	while (number_str[s_i])
	{
		for (b_i = 0; b_i < buf_size - 2 && number_str[s_i]; s_i++)
		{
			if (number_str[s_i] == '_' || number_str[s_i] == ',')
				continue;

			if (number_str[s_i] == '.')
				break;

			buf[b_i] = char_to_int(number_str[s_i]);
			if (buf[b_i] < 0 || (unsigned int)buf[b_i] >= base)
			{
				fprintf(stderr,
						"ParsingError: Invalid character '%c' for base%d.\n",
						number_str[s_i], base);
				goto cleanup_numstr;
			}

			buf[b_i] = int_to_char(buf[b_i]);
			b_i++;
		}

		buf[b_i] = '\0';
		arr->str = xrealloc(arr->str, arr->len + b_i + sizeof(*arr->str));
		if (!arr->str)
			goto cleanup_numstr;

		strcpy(&arr->str[arr->len], buf);
		arr->len += b_i;
		if (number_str[s_i] == '.')
			break;
	}

	if (!arr->str || !arr->str[0])
	{
		fprintf(stderr, "ParsingError: string did not contain any valid digits.\n");
		goto cleanup_numstr;
	}

	if (number_str[s_i - 1] == '_' || number_str[s_i - 1] == ',')
	{
		fprintf(stderr, "ParsingError: Trailing separators not allowed.\n");
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
 * char_to_int - map a base36 ascii symbol to a decimal.
 * @c: an alphanumeric symbol.
 *
 * Return: decimal value of the symbol, -1 if invalid symbol.
 */
int char_to_int(char c)
{
	if (!isalnum(c))
		return (-1);

	if (isdigit(c))
		return (c - '0');

	return (toupper(c) - 'A' + 10);
}

/**
 * int_to_char - map a decimal between 0-35 to a base36 ascii symbol.
 * @num: the number to convert.
 *
 * Return: the ascii symbol, '\0' on error.
 */
char int_to_char(unsigned int num)
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
 * @base: an int between 2-36 indicating the base of the number.
 *
 * Return: a pointer to a bignum struct, NULL on failure.
 */
bignum *numstr_to_bignum(numstr *num, unsigned int base)
{
	size_t a_i = 0, n_i = 0, tmp = 0;
	unsigned int digits = 0;
	bignum *arr = NULL;
	char num_buf[16], *end = NULL;

	if (!num || !num->len || !num->str || !isalnum(num->str[0]) ||
		base < 2 || base > 36)
	{
		return (NULL);
	}

	if (base != 10)
		return (anybase_to_bignum(num, base));

	/*sizeof(bignum) == */
	/*ceil(numstr.len / no. of digits that can represent BIGNUM_UINT_MAX)*/
	digits = count_digits(BIGNUM_UINT_MAX - 1, base);
	a_i = (num->len / digits);
	if (num->len % digits)
		a_i++;

	arr = alloc_bignum(a_i);
	if (!arr)
		return (NULL);

	for (a_i = 0, n_i = num->len; a_i < arr->len && n_i; a_i++)
	{
		if (digits > n_i)
		{
			digits = n_i;
			n_i = 0;
		}
		else
			n_i -= digits;

		strncpy(num_buf, &num->str[n_i], digits);
		num_buf[digits] = '\0';
		tmp += strtoull(num_buf, &end, base);
		if (*end)
		{
			fprintf(
				stderr, "ParsingError: Invalid character '%c' for base%d\n",
				*end, base);
			return (free_bignum(arr));
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
 * anybase_to_bignum - convert a numstr in any base to a bignum.
 * @num: the numstr.
 * @base: an int between 2-36 indicating the base of the number.
 *
 * Return: a pointer to a bignum struct, NULL on failure.
 */
bignum *anybase_to_bignum(numstr *num, unsigned int base)
{
	size_t i = 0;
	int cval = 0;
	uint sarr[1] = {0}, barr[1] = {base};
	bignum *arr = NULL, *tmp = NULL;
	bignum b = {.len = 1, .is_negative = false, .num = barr};
	bignum s = {.len = 1, .is_negative = false, .num = sarr};

	arr = alloc_bignum(1);
	if (!arr)
		return (NULL);

	/* while(str[i]); num = base * num + str[i]; i++; */
	for (i = 0; i < num->len && arr; i++)
	{
		tmp = arr;
		arr = bn_multiplication(&b, arr);
		tmp = free_bignum(tmp);
		cval = char_to_int(num->str[i]);
		if (cval < 0 || (uint)cval >= base)
		{
			fprintf(
				stderr, "ParsingError: Invalid character '%c' for base%u\n",
				num->str[i], base);
			return (free_bignum(arr));
		}

		sarr[0] = cval;
		tmp = arr;
		arr = bn_addition(arr, &s);
		tmp = free_bignum(tmp);
	}

	return (arr);
}

/**
 * count_digits - get how many digits of a given base can represent a number.
 * @num: the decimal number.
 * @base: the number base.
 *
 * Return: digits calculated.
 */
unsigned int count_digits(size_t num, unsigned int base)
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

	num = alloc_numstr(arr->len * count_digits(BIGNUM_UINT_MAX - 1, 10) + 1);
	for (g = arr->len, h = 0; num && g > 0 && h < num->len;)
	{
		--g;
		bytes_written = sprintf(&num->str[h], "%u", arr->num[g]);
		h += bytes_written - 1;
	}

	return (num);
}
