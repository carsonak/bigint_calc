#include "_bi_internals.h"
#include "bigint.h"

/**
 * bi_dup - duplicates a `bigint`.
 * @n: pointer to the `bigint` to duplicate.
 *
 * Return: pointer to the duplicated int, NULL on failure.
 */
bigint *bi_dup(bigint *const n)
{
	if (!n)
		return (NULL);

	_bi_trim(n);
	return (_bi_dup(n));
}
