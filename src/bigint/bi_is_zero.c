#include "bigint_utils.h"

/**
 * bni_is_zero - check if a bigint is equal to 0.
 * @n: the bigint to check.
 *
 * Return: true if bigint is NULL or equal to zero, else false.
 */
bool bi_is_zero(bigint *const n)
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
