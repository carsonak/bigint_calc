#include "number_parsing.h"

/*!
 * @brief print a `numstr`.
 * @public @memberof numstr
 *
 * @param[out] stream pointer to the stream to ouput to.
 * @param[in] num pointer to the `numstr`.
 *
 * @return number of bytes printed.
 */
len_ty
_numstr_print(FILE *const restrict stream, const numstr *const restrict num)
{
	if (!num || !num->str)
		return (fprintf(stream, "NULL\n"));

	len_ty bytes_printed = fprintf(stream, "base%u ", num->base);
	if (bytes_printed < 0)
		return (bytes_printed);

	bytes_printed +=
		(num->is_negative ? fprintf(stream, "-%s\n", num->str)
						  : fprintf(stream, "%s\n", num->str));
	return (bytes_printed);
}
