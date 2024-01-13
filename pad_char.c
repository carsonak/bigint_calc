#include "infix.h"

/**
 * pad_char - calculates length of initial padding characters in a string
 * @str: the string to check
 * @ch: the character
 *
 * Return: number of padding characters
 */
size_t pad_char(char *str, char *ch)
{
	size_t zeros = 0;

	if (str)
	{
		zeros = strspn(str, ch);
		if (zeros && !str[zeros])
			zeros--;
	}

	return (zeros);
}
