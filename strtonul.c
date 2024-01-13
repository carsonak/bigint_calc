#include "infix.h"

/**
 * strton64 - converts n elements of a string to size_t int
 * @num: pointer to a string with decimals
 * @nelem: max elements to convert
 *
 * Return: a size_t
 */
size_t strtonul(char *num, size_t nelem)
{
	size_t i = 0, nam = 0;

	for (i = 0; i < nelem; i++)
	{
		if (isdigit(num[i]))
			nam = (nam * 10) + (num[i] - '0');
		else
			break;
	}

	return (nam);
}
