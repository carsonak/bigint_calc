#include "_bi_internals.h"
#include "number_parsing.h"

static ATTR_NONNULL bool check_is_negative(
	char const *const restrict number, len_type *const restrict str_i
);
static ATTR_NONNULL_IDX(2) char map_digits(const char c, void *radix);
static len_type
leading_chars_span(char const *const str, char const *const ch);

#include "char_to_int.c"
#include "int_to_char.c"
#include "is_valid_radix.c"

/**
 * map_digits - validates that characters are acceptable digits.
 * @c: the character to validate.
 * @radix: pointer to an int indicating the current working radix.
 *
 * Return: the transformed character, -1 on error.
 */
static char map_digits(const char c, void *radix)
{
	short int a = char_to_int(c);
	const radix_type base = *((const radix_type *)radix);

	if (c == '_')
		return (0); /* underscores should be ignored. */

	if (a < 0 || (radix_type)a >= base)
		return (-1); /* invalid character. */

	return (int_to_char(a));
}

#define FILTER_STR_BUFFER_SIZE 1024U

/**
 * filter_str - filter characters of a string.
 * @str: the string to filter.
 * @processed: address to store number of characters processed in the string.
 * @map: pointer to a function that maps characters onto other characters.
 * @map_args: pointer to arguments that will be passed onto the map function.
 *
 * If the `map` function returns a negative int, processing of the string is
 * finalised and the filtered string returned.
 * If the `map` function returns 0, the character will be will be skipped,
 * effectively deleting the character from the output string.
 *
 * If the function fails due to a memory allocation issue, the original value
 * in `processed` will be unchanged.
 *
 * Return: pointer to the filtered string, NULL on failure.
 */
char *filter_str(
	char const *const restrict str, len_type *const restrict processed,
	const mapping_func map, void *const map_args
)
{
	if (!str || !map)
		return (NULL);

	char *output = NULL, c = 0;
	len_type str_i = 0, out_len = 0;
	char buffer[FILTER_STR_BUFFER_SIZE];

	while (str[str_i])
	{
		len_type buf_i = 0;

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
			output, (out_len + buf_i + 1) * sizeof(*output)
		);
		if (!output)
			break;

		strcpy(&output[out_len], buffer);
		out_len += buf_i;
		if (c < 0)
			break;
	}

	/* processed should not change in case of alloc fail. */
	if (output && processed)
		*processed = str_i;

	/* In the case `str = "\0"`, output will be NULL, which is undesirable as */
	/* NULL is an error value in this function. */
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
static len_type leading_chars_span(char const *const str, char const *const ch)
{
	len_type count = 0;

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
static bool check_is_negative(
	char const *const restrict number, len_type *const restrict str_i
)
{
	bool is_neg = false;

	for (; number[*str_i] == '-' || number[*str_i] == '+'; ++(*str_i))
		if (number[*str_i] == '-')
			is_neg = !is_neg;

	return (is_neg);
}

/**
 * _str_to_numstr - parse a string of digits.
 * @number: a string with a number.
 * @base: an int between 2-36 indicating the radix of the number.
 * @processed: address to store number of characters processed in the string.
 *
 * If the function fails due to a memory allocation issue,
 * the value in `processed` will be unchanged.
 *
 * Return: pointer to a numstr struct, NULL on failure.
 */
numstr *_str_to_numstr(
	char const *const restrict number, const radix_type base,
	len_type *const restrict processed
)
{
	numstr *ns = NULL;
	len_type str_i = 0, p = 0;

	if (!number || !is_valid_radix(base))
		return (NULL);

	ns = _numstr_alloc(0);
	if (!ns)
		return (NULL);

	ns->is_negative = check_is_negative(number, &str_i);
	if (number[str_i] == '_')
	{
		fprintf(stderr, "ParsingError: Leading underscores not allowed.\n");
		goto input_error;
	}

	str_i += leading_chars_span(&number[str_i], "0");
	{
		radix_type b = base;  // Avoiding Undefined Behaviour.
		ns->str = filter_str(&number[str_i], &p, map_digits, &b);
	}

	if (!ns->str)
		goto malloc_error;

	str_i += p;
	ns->len = strlen(ns->str);
	if (ns->len < 1)
	{
		fprintf(
			stderr, "ParsingError: string did not contain any valid digits.\n"
		);
		goto input_error;
	}

	if (str_i && number[str_i - 1] == '_')
	{
		fprintf(stderr, "ParsingError: Trailing underscores not allowed.\n");
		--str_i;
input_error:
		ns = _numstr_free(ns);
	}

	if (processed)
		*processed = str_i;

	return (ns);

malloc_error:
	return (_numstr_free(ns));
}
