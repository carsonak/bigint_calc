#ifndef BIGINT_COUNT_DIGITS_C
#define BIGINT_COUNT_DIGITS_C

#include <stdint.h> /* uintmax_t */

static unsigned int count_digits(uintmax_t num);

/**
 * count_digits - calculate how many digits can represent a number.
 * @num: the decimal number.
 *
 * Return: number of digits calculated.
 */
static unsigned int count_digits(uintmax_t num)
{
	unsigned int d = 0;

	while (num)
	{
		++d;
		num /= 10;
	}

	return (d);
}

#endif /* BIGINT_COUNT_DIGITS_C */
