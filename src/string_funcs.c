#include "infiX.h"
/*#define TESTING_STRINGFUNCTIONS*/

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
 * Return: 0 if both equal, -ve if s1 smaller, +ve if s1 bigger.
 */
int _strcmp(const char *s1, const char *s2)
{
	size_t i = 0;

	if (s1 && !s2)
		return (*s1);
	else if (!s1 && s2)
		return (-*s2);
	else if (s1 && s2)
		for (i = 0; s2[i] || s1[i]; i++)
			if (s1[i] != s2[i])
				return (s1[i] - s2[i]);

	return (0);
}

#ifdef TESTING_STRINGFUNCTIONS

/**
 * main - testing string functions
 *
 * Return: 0
 */
int main(void)
{
	printf("compare: NULL NULL => %d\n", _strcmp(NULL, NULL));
	printf("compare: 'AAA' NULL => %d\n", _strcmp("AAA", NULL));
	printf("compare: NULL 'AAA' => %d\n", _strcmp(NULL, "AAA"));
	printf("compare: 'AAA' 'AAA' => %d\n", _strcmp("AAA", "AAA"));
	printf("compare: 'AcA' 'AAA' => %d\n", _strcmp("AcA", "AAA"));
	printf("compare: 'AAA' 'AcA' => %d\n", _strcmp("AAA", "AcA"));

	return (0);
}
#endif /*TESTING_STRINGFUNCTIONS*/
