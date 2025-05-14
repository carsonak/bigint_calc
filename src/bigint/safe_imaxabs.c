#include <inttypes.h> /* imaxabs */

#include "bigint.h"

/**
 * safe_imaxabs - return the absolute value of an intmax_t including the
 * minimum value of the type.
 * @n: the integer to convert.
 *
 * Return: the absolute value of `n`.
 */
uintmax_t safe_imaxabs(intmax_t n)
{
	return ((n == INTMAX_MIN) ? (uintmax_t)INTMAX_MAX + 1 : imaxabs(n));
}
