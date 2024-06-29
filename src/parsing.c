#include "infiX.h"

/**
 * numstr_to_u4b - convert a intstr to a u4b_bignum.
 * @num_str: the intstr
 *
 * Return: a pointer to a u4b_bignum struct, NULL on failure.
 */
u4b_bignum *numstr_to_u4b(intstr *num_str)
{
	u4b_bignum *arr = NULL;

	if (!num_str || !num_str)
	{
		/* code */
	}

	return (arr);
}

/**
 * u4b_to_numstr - convert a u4b_bignum to a intstr.
 * @arr: the u4b_bignum
 *
 * Return: a pointer to a intstr, NULL on failure.
 */
intstr *u4b_to_numstr(u4b_bignum *arr)
{
	(void)arr;
	return (NULL);
}

/**
 * parse_numstr - parse a string of numbers
 * @numstr: a string with numbers.
 *
 * Return: pointer to a intstr struct.
 */
intstr *parse_numstr(const char *numstr)
{
	intstr *arr = NULL;
	size_t s_i = 0, b_i = 0;
	const unsigned int buf_size = 1024;
	char buf[buf_size];

	if (!numstr || (!isdigit(numstr[0]) && numstr[0] != '-' && numstr[0] != '+'))
		return (NULL);

	arr = alloc_numstr_array(0);
	for (s_i = 0; numstr[s_i] == '-' || numstr[s_i] == '+'; s_i++)
		if (numstr[s_i] == '-')
			arr->is_negative = !arr->is_negative;

	s_i += leading_chars_len(&numstr[s_i], "0");
	while (numstr[s_i])
	{
		for (b_i = 0; b_i < buf_size - 2 && numstr[s_i]; s_i++)
		{
			if (isdigit(numstr[s_i]))
			{
				buf[b_i] = numstr[s_i];
				b_i++;
			}
			else if (numstr[s_i] == '.')
				break;
			else if (numstr[s_i] != '_' || numstr[s_i] != ',')
			{
				fprintf(stderr, "ParsingError: Invalid character: '%c'\n", numstr[s_i]);
				return (free_numstr_array(arr));
			}
		}

		buf[b_i] = '\0';
		arr->str = xrealloc(arr->str, arr->len + b_i + 1);
		if (!arr->str)
			return (free_numstr_array(arr));

		strcpy(&arr->str[arr->len], buf);
		arr->len += b_i;
		if (numstr[s_i] == '.')
			break;
	}

	if (!arr->str || !arr->str[0])
	{
		fprintf(stderr, "ParsingError: 0 decimal characters found\n");
		return (free_numstr_array(arr));
	}

	return (arr);
}

/**
 * uint_array_to_str - represent an unsigned int array as a string
 * @arr: the unsigned int array
 * @len: number of items in the unsigned int
 *
 * Return: pointer to a string, NULL on failure
 */
char *uint_array_to_str(const uint32_t *arr, size_t len)
{
	size_t s_i = 0, n = 0, len_sep = 0, len_str = 0;
	int bytes_written = 0;
	char *numstr = NULL, *sep = ", ";

	if (!arr || !len)
		return (strdup("{NULL}"));

	len_sep = strlen(sep);
	/*sizeof(numstr) == total sizeof(arr) + total sizeof(separators) + */
	/*sizeof("{}") + 1*/
	len_str = (count_digits(MAX_VAL_u4b) * len) + (len_sep * (len - 1)) + 2 + 1;
	numstr = xcalloc(len_str, sizeof(*numstr));
	if (!numstr)
		return (NULL);

	strcpy(numstr, "{");
	for (n = 0, s_i = 1; n < len - 1 && s_i < len_str; n++)
	{
		bytes_written = sprintf(&numstr[s_i], "%d", arr[n]);
		if (bytes_written < 0)
			return (free_n_null(numstr));

		s_i += bytes_written;
		strcpy(&numstr[s_i], sep);
		s_i += len_sep;
	}

	if (sprintf(&numstr[s_i], "%d}", arr[n]) < 0)
		return (free_n_null(numstr));

	return (xrealloc(numstr, strlen(numstr) + 1));
}

/**
 * leading_chars_len - finds the number of leading characters in a string
 * @str: the string to check
 * @ch: the character
 *
 * Return: number of leading characters
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
 * count_digits - counts the number of digits in a number
 * @num: the number
 *
 * Return: digits counted
 */
unsigned int count_digits(ssize_t num)
{
	unsigned int d = 0;

	while (num)
	{
		d++;
		num /= 10;
	}

	return (d);
}

/**
 * print_u4b_array - print a u4b_bignum
 * @arr: pointer to the array struct
 *
 * Return: number of bytes printed, -1 on error.
 */
ssize_t print_u4b_array(u4b_bignum *arr)
{
	char *str_arr = NULL;
	ssize_t bytes_printed = 0;

	if (!arr)
		return (-1);

	str_arr = uint_array_to_str(arr->array, arr->len);
	if (!str_arr)
		return (-1);

	if (arr->is_negative)
		bytes_printed = printf("-%s\n", str_arr);
	else
		bytes_printed = printf("%s\n", str_arr);

	free_n_null(str_arr);
	return (bytes_printed);
}
