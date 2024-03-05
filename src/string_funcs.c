#include "infiX.h"

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

/**
 * _strcmp - compares 2 strings
 * @s1: first string.
 * @s2: second string.
 *
 * Return: 0 if both equal, -ve if s1 shorter, +ve if s1 longer.
 */
int _strcmp(const char *s1, const char *s2)
{
	size_t i = 0;

	for (i = 0; s2[i] || s1[i]; i++)
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);

	return (0);
}
