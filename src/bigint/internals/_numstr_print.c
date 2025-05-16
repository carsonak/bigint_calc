#include "_numstr_internals.h"

/**
 * _numstr_print - print a `numstr`.
 * @stream: pointer to the stream to ouput to.
 * @num: pointer to the `numstr`.
 *
 * Return: number of bytes printed.
 */
len_typ
_numstr_print(FILE *const restrict stream, const numstr *const restrict num)
{
	if (!num || !num->str)
	{
		fprintf(stderr, "NULL\n");
		return (0);
	}

	const len_typ bytes_printed =
		(num->is_negative ? fprintf(stream, "-%s\n", num->str)
						  : fprintf(stream, "%s\n", num->str));
	return (bytes_printed);
}
