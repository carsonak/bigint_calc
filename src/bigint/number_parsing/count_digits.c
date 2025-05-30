#ifndef BIGINT_COUNT_DIGITS_C
#define BIGINT_COUNT_DIGITS_C

#include "bigint_types.h"
#include <stdint.h> /* uintmax_t */

static unsigned int count_digits(uintmax_t num, const radix_type base);

/*!
 * @brief calculate how many digits can represent a number in the given base.
 *
 * @param[in] num the number.
 * @param[in] base the radix to calculate for.
 *
 * @return number of digits calculated.
 */
static unsigned int count_digits(uintmax_t num, const radix_type base)
{
	unsigned int d = 0;

	while (num)
	{
		++d;
		num /= base;
	}

	return (d);
}

#endif /* BIGINT_COUNT_DIGITS_C */
