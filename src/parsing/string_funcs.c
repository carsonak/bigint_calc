#include "parsing.h"

/**
 * leading_chars_len - finds the number of leading characters in a string.
 * @str: the string to check.
 * @ch: the character.
 *
 * Return: number of leading characters.
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
 * char_to_int - map a base36 ascii symbol to a decimal.
 * @c: an alphanumeric symbol.
 *
 * Return: decimal value of the symbol, -1 if invalid symbol.
 */
int char_to_int(char c)
{
	if (!isalnum(c))
		return (-1);

	if (isdigit(c))
		return (c - '0');

	return (toupper(c) - 'A' + 10);
}

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
