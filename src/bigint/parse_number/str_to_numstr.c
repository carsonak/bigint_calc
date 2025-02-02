#include "_bigint_internals.h"
#include "_numstr_internals.h"
#include "parse_number.h"

static ATTR_NONNULL bool
check_is_negative(char const *const number, size_t *str_i);
static int char_to_int(const char c);
static char int_to_char(const unsigned int num);
static ATTR_NONNULL_IDX(2) char map_digits(const char c, void *radix);

/**
 * char_to_int - map a base36 ascii symbol to a decimal.
 * @c: an alphanumeric symbol.
 *
 * Return: decimal value of the symbol, -1 if invalid symbol.
 */
static int char_to_int(const char c)
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
static char int_to_char(const unsigned int num)
{
	if (num > 35)
		return ('\0');

	if (num < 10)
		return ('0' + num);

	return ('A' + (num - 10));
}

/**
 * map_digits - validates that characters are acceptable digits.
 * @c: the character to validate.
 * @radix: pointer to an int indicating the current working radix.
 *
 * Return: the transformed character, -1 on error.
 */
static char map_digits(const char c, void *radix)
{
	int a = char_to_int(c);
	const unsigned int base = *((const unsigned int *)radix);

	if (c == '_')
		return (0); /*underscores should be ignored.*/

	if (a < 0 || (unsigned int)a >= base)
		return (-1); /*invalid character.*/

	return (int_to_char(a));
}

#define FILTER_STR_BUFFER_SIZE 1024U

/**
 * filter_str - filter characters of a string.
 * @str: the string to process.
 * @processed: address to store number of characters processed in the string.
 * @map: pointer to a function that transforms/maps characters onto others.
 * @map_args: pointer to more arguments to pass to the map function.
 *
 * If map returns a negative char, processing is finalised and the processed
 * string returned.
 * If map returns 0, the character will be ignored, essentially deleting the
 * character from the output string.
 *
 * If the function fails due to a memory allocation issue,
 * the value in `processed` will be unchanged.
 *
 * Return: pointer to the filtered string, NULL on failure.
 */
char *filter_str(
	char const *const str, size_t *const processed, const mapping_func map,
	void *const map_args)
{
	char *output = NULL, c = 0;
	size_t buf_i = 0, str_i = 0, out_len = 0;

	if (!str || !map)
		return (NULL);

	char buffer[FILTER_STR_BUFFER_SIZE];

	while (str[str_i])
	{
		for (buf_i = 0; buf_i < FILTER_STR_BUFFER_SIZE - 1 && str[str_i];
		     ++str_i)
		{
			c = map(str[str_i], map_args);
			if (c < 0)
				break;

			if (c == 0)
				continue;

			buffer[buf_i] = c;
			++buf_i;
		}

		buffer[buf_i] = '\0';
		output = xrealloc_free_on_fail(
			output, (out_len + buf_i + 1) * sizeof(*output));
		if (!output)
			break;

		strcpy(&output[out_len], buffer);
		out_len += buf_i;
		if (c < 0)
			break;
	}

	/*processed should not change in case of alloc fail.*/
	if (output && processed)
		*processed = str_i;

	/*In the case `str = "\0"`, output will be NULL, which is undesirable as*/
	/*NULL is an error value in this function.*/
	if (str_i == 0 && !str[str_i])
		return (xcalloc(1, sizeof(*output)));

	return (output);
}

#undef FILTER_STR_BUFFER_SIZE

/**
 * leading_chars_span - finds the number of leading characters in a string.
 * @str: the string to check.
 * @ch: pointer to a string of characters to look for.
 *
 * If `str` consists of only characters in `ch`, then;
 * length of leading characters = length of `str` - 1
 *
 * Return: number of leading characters.
 */
size_t leading_chars_span(char const *const str, char const *const ch)
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
 * check_is_negative - determine signedness of number.
 * @number: a string with a number.
 * @str_i: pointer to number's index.
 *
 * Return: true if the sign is negative, false otherwise.
 */
static bool check_is_negative(char const *const number, size_t *str_i)
{
	bool is_neg = false;

	for (; number[*str_i] == '-' || number[*str_i] == '+'; ++(*str_i))
		if (number[*str_i] == '-')
			is_neg = !is_neg;

	return (is_neg);
}

/**
 * str_to_numstr - parse a string of digits.
 * @number: a string with a number.
 * @base: an int between 2-36 indicating the base of the number.
 * @processed: address to store number of characters processed in the string.
 *
 * If the function fails due to a memory allocation issue,
 * the value in `processed` will be unchanged.
 *
 * Return: pointer to a numstr struct, NULL on failure.
 */
numstr *str_to_numstr(
	char const *const number, const unsigned short int base,
	size_t *const processed)
{
	numstr *ns = NULL;
	size_t str_i = 0, p = 0;

	if (!number || base < 2 || base > 36)
		return (NULL);

	ns = _numstr_alloc(0);
	if (!ns)
		return (NULL);

	ns->is_negative = check_is_negative(number, &str_i);
	if (number[str_i] == '_')
	{
		fprintf(stderr, "ParsingError: Leading underscores not allowed.\n");
		goto cleanup_numstr;
	}

	str_i += leading_chars_span(&number[str_i], "0");
	ns->str = filter_str(&number[str_i], &p, map_digits, &base);
	if (!ns->str)
		goto critical_failure;

	str_i += p;
	ns->len = strlen(ns->str);
	if (!ns->len)
	{
		fprintf(
			stderr, "ParsingError: string did not contain any valid digits.\n");
		goto cleanup_numstr;
	}

	if (str_i && number[str_i - 1] == '_')
	{
		fprintf(stderr, "ParsingError: Trailing underscores not allowed.\n");
		--str_i;
cleanup_numstr:
		ns = _numstr_free(ns);
	}

	if (processed)
		*processed = str_i;

	return (ns);

critical_failure:
	return (_numstr_free(ns));
}
