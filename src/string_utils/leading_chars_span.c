#include "string_utils.h"

/**
 * leading_chars_span - finds the number of leading characters in a string.
 * @str: the string to check.
 * @ch: pointer to a string of characters to look for.
 *
 * Return: number of leading characters.
 */
size_t leading_chars_span(const char *str, char *ch)
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
