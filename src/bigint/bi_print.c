#include "_bigint_internals.h"
#include "bigint.h"

#include "count_digits.c"

/**
 * bi_print - print a bigint.
 * @stream: pointer to a stream to print to.
 * @n: pointer to the bigint.
 *
 * Return: number of bytes printed, -1 on error.
 */
len_typ bi_print(FILE *const restrict stream, const bigint *const restrict n)
{
	if (!n)
		return (-1);

	len_typ bytes_printed = 0;
	char *const str_arr = _uint_array_to_str(n->num, n->len);
	if (!str_arr)
		return (-1);

	if (n->is_negative)
		bytes_printed = fprintf(stream, "-%s\n", str_arr);
	else
		bytes_printed = fprintf(stream, "%s\n", str_arr);

	xfree(str_arr);
	return (bytes_printed);
}

/**
 * _uint_array_to_str - represent an uint array as a string.
 * @arr: pointer to the array.
 * @len: number of items in the array.
 *
 * Return: pointer to a string, NULL on error.
 */
char *_uint_array_to_str(u_int const *const arr, const len_typ len)
{
	if (!arr || len < 1)
		return (xstrdup("{NULL}"));

	len_typ s_i = 0, n = 0;
	char *const restrict sep = ", ";
	const len_typ len_sep = strlen(sep);
	/* sizeof(str) == (max "digits" in U_INT_MAX * len) + */
	/* total sizeof(separators) + sizeof("{}") + 1 */
	const len_typ len_str =
		((count_digits(U_INT_MAX) - 1) * len) + (len_sep * (len - 1)) + 2 + 1;
	char *const restrict str = xmalloc(len_str * sizeof(*str));

	if (!str)
		return (NULL);

	strcpy(str, "{");
	for (n = 0, s_i = 1; n < len - 1 && s_i < len_str; ++n)
	{
		int bytes_written = sprintf(&str[s_i], "%d", arr[n]);
		if (bytes_written < 0)
			return (xfree(str));

		s_i += bytes_written;
		strcpy(&str[s_i], sep);
		s_i += len_sep;
	}

	if (sprintf(&str[s_i], "%d}", arr[n]) < 0)
		return (xfree(str));

	return (xrealloc_free_on_fail(str, strlen(str) + 1));
}
