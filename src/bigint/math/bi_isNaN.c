#include "_bi_internals.h"
#include "bigint.h"

/**
 * bi_isNaN - check if a bigint is Not a Number.
 * @n: pointer to the bigint.
 *
 * Return: true if the bigint is NaN, false otherwise.
 */
bool bi_isNaN(bigint *const n)
{
	if (!n)
		return (false);

	_bi_trim(n);
	return (_bi_isNaN_const(n));
}

/**
 * _bi_isNaN_const - check if a const bigint is Not a Number.
 * @n: pointer to the bigint.
 *
 * Return: true if the bigint is NaN, false otherwise.
 */
bool _bi_isNaN_const(const bigint *const n)
{
	if (!n)
		return (false);

	return (n->len == 0 && !n->num);
}
