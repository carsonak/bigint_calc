#ifndef BIGINT_SAFE_IMAXABS
#define BIGINT_SAFE_IMAXABS

#include <inttypes.h> /* imaxabs */

static uintmax_t safe_imaxabs(intmax_t n);

/**
 * safe_imaxabs - return the absolute value of an intmax_t including the
 * minimum value of the type.
 * @n: the integer to convert.
 *
 * Return: the absolute value of `n`.
 */
static uintmax_t safe_imaxabs(intmax_t n)
{
	return ((n == INTMAX_MIN) ? (uintmax_t)INTMAX_MAX + 1 : imaxabs(n));
}

#endif /* BIGINT_SAFE_IMAXABS */
