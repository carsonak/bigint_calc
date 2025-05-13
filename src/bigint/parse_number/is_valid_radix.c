#ifndef BIGINT_IS_VALID_RADIX
#define BIGINT_IS_VALID_RADIX

#include "bigint_types.h"

static bool is_valid_radix(const unsigned short int base);

/**
 * is_valid_radix - check if a number is within the accepted radix range.
 * @base: the number to check.
 *
 * minimum radix: `MINIMUM_BIGINT_RADIX`.
 * maximum radix: `MAXIMUM_BIGINT_RADIX`.
 *
 * Return: true if `base` is within the valid range, false otherwise.
 */
static bool is_valid_radix(const unsigned short int base)
{
	return (base >= MINIMUM_BIGINT_RADIX && base <= MAXIMUM_BIGINT_RADIX);
}

#endif /* BIGINT_IS_VALID_RADIX */
