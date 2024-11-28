#include "numstr.h"

static ATTR_NONNULL bool
check_is_negative(char const *const num_str, size_t *str_i);

/**
 * check_is_negative - determine signedness of number.
 * @num_str: a string with a number.
 * @str_i: pointer to num_str's index.
 *
 * Return: true if the sign is negative, false otherwise.
 */
static bool check_is_negative(char const *const num_str, size_t *str_i)
{
	bool is_neg = false;

	for (; num_str[*str_i] == '-' || num_str[*str_i] == '+'; ++(*str_i))
		if (num_str[*str_i] == '-')
			is_neg = !is_neg;

	return (is_neg);
}

/**
 * map_digits - validates that characters are acceptable digits.
 * @c: the character to validate.
 * @radix: pointer to an int indicating the current working radix.
 *
 * Return: the transformed character, -1 on error.
 */
static char map_digits(const char c, void const *const radix)
{
	int a = char_to_int(c);

	if (!radix)
		return (-1);

	const unsigned int base = *((const unsigned int *)radix);

	if (c == '_')
		return (0); /*underscores should be ignored.*/

	if (a < 0 || (unsigned int)a >= base)
		return (-1); /*invalid character.*/

	return (int_to_char(a));
}

/**
 * str_to_numstr - parse a string of digits.
 * @number_str: a string with a number.
 * @base: an int between 2-36 indicating the base of the number.
 * @processed: address to store number of characters processed.
 *
 * If this function returns NULL, processed will not be changed.
 *
 * Return: pointer to a numstr struct, NULL of failure.
 */
numstr *str_to_numstr(
	char const *const number_str, const unsigned int base, size_t *const processed)
{
	numstr *ns = NULL;
	size_t str_i = 0, p = 0;

	if (!number_str || base < 2 || base > 36)
		return (NULL);

	ns = alloc_numstr(0);
	if (!ns)
		return (NULL);

	ns->is_negative = check_is_negative(number_str, &str_i);
	if (number_str[str_i] == '_')
	{
		fprintf(
			stderr, "ParsingError: Leading underscores not allowed.\n");
		goto cleanup_numstr;
	}

	str_i += leading_chars_span(&number_str[str_i], "0");
	ns->str = filter_str(&number_str[str_i], &p, map_digits, &base);
	if (!ns->str)
		goto critical_failure;

	ns->len = strlen(ns->str);
	if (!ns->len)
	{
		fprintf(
			stderr, "ParsingError: string did not contain any valid digits.\n");
		goto cleanup_numstr;
	}

	str_i += p;
	if (str_i && number_str[str_i - 1] == '_')
	{
		fprintf(
			stderr, "ParsingError: Trailing underscores not allowed.\n");
		--str_i;
cleanup_numstr:
		ns = free_numstr(ns);
	}

	if (processed)
		*processed = str_i;

	return (ns);

critical_failure:
	return (free_numstr(ns));
}
