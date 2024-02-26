#include "infix.h"

/**
 * pad_char - finds the number of leading characters in a string
 * @str: the string to check
 * @ch: the character
 *
 * Return: number of leading characters
 */
size_t pad_char(char *str, char *ch)
{
	size_t count = 0;

	if (str)
	{
		count = strspn(str, ch);
		if (count && !str[count])
			count--;
	}

	return (count);
}
