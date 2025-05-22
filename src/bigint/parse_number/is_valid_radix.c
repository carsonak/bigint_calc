#ifndef BIGINT_IS_VALID_RADIX
#define BIGINT_IS_VALID_RADIX

#include "parse_number.h"

static bool is_valid_radix(const radix_type base);

/**
 * is_valid_radix - check if a number is within the accepted radix range.
 * @base: the number to check.
 *
 * minimum radix: `RADIX_MINIMUM`.
 * maximum radix: `RADIX_MAXIMUM`.
 *
 * Return: true if `base` is within the valid range, false otherwise.
 */
static bool is_valid_radix(const radix_type base)
{
	return (base >= RADIX_MINIMUM && base <= RADIX_MAXIMUM);
}

#endif /* BIGINT_IS_VALID_RADIX */
