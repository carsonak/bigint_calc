#include "string_utils.h"

/**
 * char_to_int - map a base36 ascii symbol to a decimal.
 * @c: an alphanumeric symbol.
 *
 * Return: decimal value of the symbol, -1 if invalid symbol.
 */
int char_to_int(const char c)
{
	if (!isalnum(c))
		return (-1);

	if (isdigit(c))
		return (c - '0');

	return (toupper(c) - 'A' + 10);
}
