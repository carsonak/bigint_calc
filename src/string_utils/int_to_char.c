#include "string_utils.h"

/**
 * int_to_char - map a decimal between 0-35 to a base36 ascii symbol.
 * @num: the number to convert.
 *
 * Return: the ascii symbol, '\0' on error.
 */
char int_to_char(unsigned int num)
{
	if (num > 35)
		return ('\0');

	if (num < 10)
		return ('0' + num);

	return ('A' + (num - 10));
}
