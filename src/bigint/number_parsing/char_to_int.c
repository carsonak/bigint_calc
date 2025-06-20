#ifndef BIGINT_CHAR_TO_INT
#define BIGINT_CHAR_TO_INT

#include <ctype.h>  // isalnum, isdigit

static short int char_to_int(const char c);

/*!
 * @brief map an ascii symbol of a base36 number to a decimal number.
 *
 * @param c an alphanumeric character.
 *
 * @return decimal value of the symbol, -1 on error.
 */
static short int char_to_int(const char c)
{
	if (!isalnum(c))
		return (-1);

	if (isdigit(c))
		return (c - '0');

	return (toupper(c) - 'A' + 10);
}

#endif  // BIGINT_CHAR_TO_INT
