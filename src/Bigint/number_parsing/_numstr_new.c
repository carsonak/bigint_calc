#include <string.h>  // strlen

#include "_bi_internals.h"
#include "number_parsing.h"

static _nonnull bool check_is_negative(
	char const *const restrict number, len_ty *const restrict str_i
);
static _nonnull_pos(2) char map_digits(const char c, void *radix);
static len_ty leading_chars_span(char const *const str, char const *const ch);

#include "char_to_int.c"
#include "int_to_char.c"
#include "is_valid_radix.c"

/*!
 * @brief validates that characters are acceptable digits.
 *
 * @param[in] c the character to validate.
 * @param[in] radix pointer to an int indicating the current working radix.
 *
 * @return the transformed character, -1 on error.
 */
static char map_digits(const char c, void *radix)
{
	short int a = char_to_int(c);
	const radix_ty base = *((const radix_ty *)radix);

	if (c == '_')
		return (0);  // underscores should be ignored.

	if (a < 0 || (radix_ty)a >= base)
		return (-1);  // invalid character.

	return (int_to_char(a));
}

/*! internal buffer size of the filter_str function. */
#define FILTER_STR_BUFFER_SIZE 1024U

/*!
 * @brief filter characters of a string.
 *
 * If the `map` function returns a negative int, processing of the string is
 * finalised and the filtered string returned.
 * If the `map` function returns 0, the character will be will be skipped,
 * effectively deleting the character from the output string.
 *
 * If the function fails due to a memory allocation issue, the original value
 * in `processed` will be unchanged.
 *
 * @param[in] str the string to filter.
 * @param[out] processed address to store number of characters processed in the
 * string.
 * @param[in] map pointer to a function that maps characters onto other characters.
 * @param[in] map_args pointer to arguments that will be passed onto the map
 * function.
 *
 * @return pointer to the filtered string, NULL on failure.
 */
char *filter_str(
	char const *const restrict str, len_ty *const restrict processed,
	const mapping_func map, void *const map_args
)
{
	if (!str || !map)
		return (NULL);

	char *output = NULL, c = 0;
	len_ty str_i = 0, out_len = 0;
	char buffer[FILTER_STR_BUFFER_SIZE];

	while (str[str_i])
	{
		len_ty buf_i = 0;

		for (buf_i = 0; buf_i < FILTER_STR_BUFFER_SIZE - 1 && str[str_i];
			 ++str_i)
		{
			c = map(str[str_i], map_args);
			if (c < 0)
				break;

			if (c == 0)
				continue;

			buffer[buf_i] = c;
			buf_i++;
		}

		buffer[buf_i] = '\0';
		output = xrealloc_free_on_fail(
			output, (out_len + buf_i + 1) * sizeof(*output)
		);
		if (!output)
			break;

		strncpy(&output[out_len], buffer, buf_i + 1);
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

/*!
 * @brief finds the number of leading characters in a string.
 *
 * If `str` consists of only characters in `ch`, then;
 * length of leading characters = length of `str` - 1.
 *
 * @param[in] str the string to check.
 * @param[in] ch pointer to a string of characters to look for.
 *
 * @return number of leading characters.
 */
static len_ty leading_chars_span(char const *const str, char const *const ch)
{
	len_ty count = 0;

	if (str && ch && *ch)
	{
		count = strspn(str, ch);
		if (count && !str[count])
			count--;
	}

	return (count);
}

/*!
 * @brief determine signedness of number in a string.
 *
 * @param[in] number a string with a number.
 * @param[in] str_i pointer to number's index.
 *
 * @return true if the sign is negative, false otherwise.
 */
static bool check_is_negative(
	char const *const restrict number, len_ty *const restrict str_i
)
{
	bool is_neg = false;

	for (; number[*str_i] == '-' || number[*str_i] == '+'; ++(*str_i))
		if (number[*str_i] == '-')
			is_neg = !is_neg;

	return (is_neg);
}

/*!
 * @brief initialise a `numstr` from a string.
 * @public @memberof numstr
 *
 * If the function fails due to a memory allocation issue,
 * the value in `processed` will be unchanged.
 *
 * @param number a string with a number.
 * @param base an int in the range 2-36 indicating the radix of the number.
 * @param processed address to store number of characters of the string
 * processed.
 *
 * @return pointer to a `numstr`, NULL on failure.
 */
numstr *_numstr_new(
	char const *const restrict number, const radix_ty base,
	len_ty *const restrict processed
)
{
	numstr *ns = NULL;
	len_ty str_i = 0, p = 0;

	if (!number || !is_valid_radix(base))
		return (NULL);

	ns = _numstr_alloc(0);
	if (!ns)
		return (NULL);

	ns->is_negative = check_is_negative(number, &str_i);
	ns->base = base;
	if (number[str_i] == '_')
	{
		fprintf(stderr, "ParsingError: Leading underscores not allowed.\n");
		goto parsing_error;
	}

	str_i += leading_chars_span(&number[str_i], "0");
	{
		radix_ty b = base;  // Switch to modifiable type to avoid UB.
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
		goto parsing_error;
	}

	if (str_i && number[str_i - 1] == '_')
	{
		fprintf(stderr, "ParsingError: Trailing underscores not allowed.\n");
		str_i--;
parsing_error:
		ns = _numstr_free(ns);
	}

	if (processed)
		*processed = str_i;

	return (ns);

malloc_error:
	return (_numstr_free(ns));
}
