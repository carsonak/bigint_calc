#include "bignum_utils.h"

/**
 * is_zero - check if a bignum is equal to 0.
 * @arr: the bignum to check.
 *
 * Return: true if bignum is NULL or equal to zero, else false.
 */
bool is_zero(bignum * const arr)
{
	if (arr && (!arr->len || !arr->num || (arr->len == 1 && !arr->num[0])))
		return (true);

	return (false);
}
