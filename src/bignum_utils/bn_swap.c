#include "bignum_utils.h"

/**
 * bn_swap - swaps two bignums.
 * @n1: the first bignum.
 * @n2: the second bignum.
 *
 * Both numbers will be adjusted to the same size in memory after the swap.
 *
 * Return: true on success, false on failure.
 */
bool bn_swap(bignum *n1, bignum *n2)
{
	bignum tmp = {0};
	size_t n1_size = 0, n2_size = 0;

	if (!n1 || !n2)
		return (false);

	n1_size = n1->len;
	n2_size = n2->len;
	if (n1_size > n2_size)
	{
		if (!bn_realloc(n2, n1_size))
			return (false);

		n2->len = n2_size;
	}
	else if (n1_size < n2_size)
	{
		if (!bn_realloc(n1, n2_size))
			return (false);

		n1->len = n1_size;
	}

	tmp = *n1;
	*n1 = *n2;
	*n2 = tmp;
	return (true);
}
