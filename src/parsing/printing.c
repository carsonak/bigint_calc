#include "parsing.h"

/**
 * print_bignum - print a bignum.
 * @arr: pointer to the array struct.
 *
 * Return: number of bytes printed, -1 on error.
 */
size_t print_bignum(bignum_i *arr)
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

/*INFO: The following function should ideally be static but that hinders easy testing.*/
/**
 * uint_array_to_str - represent an uint array as a string.
 * @arr: the uint array.
 * @len: number of items in the uint.
 *
 * Return: pointer to a string, NULL on error.
 */
char *uint_array_to_str(u_int const *const arr, size_t len)
{
	size_t s_i = 0, n = 0, len_sep = 0, len_str = 0;
	int bytes_written = 0;
	char *str = NULL, *sep = ", ";

	if (!arr || !len)
		return (xstrdup("{NULL}"));

	len_sep = strlen(sep);
	/*sizeof(str) == (max "digits" in U_INT_MAX * len) + */
	/*total sizeof(separators) + sizeof("{}") + 1*/
	len_str = ((count_digits(U_INT_MAX) - 1) * len) +
			  (len_sep * (len - 1)) + 2 + 1;
	str = xmalloc(len_str * sizeof(*str));
	if (!str)
		return (NULL);

	strcpy(str, "{");
	for (n = 0, s_i = 1; n < len - 1 && s_i < len_str; ++n)
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

	return (xrealloc_free_on_fail(str, strlen(str) + 1));
}

/**
 * print_numstr - print a numstr.
 * @num: the numstr.
 *
 * Return: number of bytes printed.
 */
size_t print_numstr(numstr *num)
{
	size_t bytes_printed = 0;

	if (!num || !num->str)
	{
		fprintf(stderr, "NULL\n");
		return (0);
	}

	if (num->is_negative)
		bytes_printed += printf("-");

	bytes_printed += printf("%s\n", num->str);

	return (bytes_printed);
}
