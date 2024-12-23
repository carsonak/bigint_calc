#include "bigint_utils.h"

/**
 * bni_swap - swaps two bigints.
 * @n1: the first bigint.
 * @n2: the second bigint.
 *
 * Both numbers will be adjusted to the same size in memory after the swap.
 *
 * Return: true on success, false on failure.
 */
bool bi_swap(bigint *n1, bigint *n2)
{
	if (!n1 || !n2)
		return (false);

	bigint tmp = {0};
	const size_t n1_size = n1->len, n2_size = n2->len;

	if (n1_size > n2_size)
	{
		bigint *nw = bi_resize(n2, n1_size);

		if (!nw)
			return (false);

		n2 = nw;
		n2->len = n2_size;
	}
	else if (n1_size < n2_size)
	{
		bigint *nw = bi_resize(n1, n2_size);

		if (!nw)
			return (false);

		n1 = nw;
		n1->len = n1_size;
	}

	tmp = *n1;
	*n1 = *n2;
	*n2 = tmp;
	return (true);
}
