#ifndef BIGINT_CHAR_TO_INT
#define BIGINT_CHAR_TO_INT

#include <ctype.h>

static short int char_to_int(const char c);

/**
 * char_to_int - map an ascii symbol for base36 to a decimal number in the range 0-35.
 * @c: an alphanumeric character.
 *
 * Return: decimal value of the symbol, -1 on error.
 */
static short int char_to_int(const char c)
{
	if (!isalnum(c))
		return (-1);

	if (isdigit(c))
		return (c - '0');

	return (toupper(c) - 'A' + 10);
}

#endif /* BIGINT_CHAR_TO_INT */
