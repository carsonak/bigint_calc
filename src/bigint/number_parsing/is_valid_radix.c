#ifndef BIGINT_IS_VALID_RADIX
#define BIGINT_IS_VALID_RADIX

#include "bigint_types.h"

static bool is_valid_radix(const radix_ty base);

/*!
 * @brief check if a number is within the accepted radix range.
 *
 * minimum radix: `RADIX_MINIMUM`.
 * maximum radix: `RADIX_MAXIMUM`.
 *
 * @param base the number to check.
 *
 * @return true if `base` is within the valid range, false otherwise.
 */
static bool is_valid_radix(const radix_ty base)
{
	return (base >= RADIX_MINIMUM && base <= RADIX_MAXIMUM);
}

#endif  // BIGINT_IS_VALID_RADIX
