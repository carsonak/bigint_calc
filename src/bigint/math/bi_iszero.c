#include "_bigint_internals.h"
#include "bigint.h"

/**
 * bi_iszero - check if a `bigint` is equal to 0.
 * @n: pointer to the `bigint` to check.
 *
 * Return: true if the `bigint`:
 * a) has a NULL array.
 * b) has len == 0.
 * c) is equal to 0.
 * else false.
 */
bool bi_iszero(bigint const *const n)
{
	len_type i = 0;

	if (!n || n->len < 0)
		return (false);

	if (!n->num || n->len == 0)
		return (true);

	for (i = n->len - 1; i > 0 && n->num[i] == 0; --i)
		;

	if (n->num[i] == 0)
		return (true);

	return (false);
}
