#include "infiX.h"

/**
 * padding_chars_len - finds the number of leading characters in a string
 * @str: the string to check
 * @ch: the character
 *
 * Return: number of leading characters
 */
size_t padding_chars_len(char *str, char *ch)
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
