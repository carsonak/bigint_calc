#include "parsing.h"

/**
 * count_digits - calculate how many digits can represent a number.
 * @num: the decimal number.
 *
 * Return: number of digits calculated.
 */
unsigned int count_digits(size_t num)
{
	int d = 0;

	while (num)
	{
		d++;
		num /= 10;
	}

	return (d);
}
