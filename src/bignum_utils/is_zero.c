#include "bignum_utils.h"

/**
 * is_zero - check if a bignum is equal to 0.
 * @n: the bignum to check.
 *
 * Return: true if bignum is NULL or equal to zero, else false.
 */
bool is_zero(bignum * const n)
{
	size_t i = 0;

	if (!n)
		return (false);

	if (n->len)
		i = n->len - 1;

	while (n->num && i && !n->num[i])
		i--;

	if (!n->len || !n->num || !n->num[i])
		return (true);

	return (false);
}
