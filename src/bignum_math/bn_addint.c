#include "bignum_math.h"

/**
 * bn_addint - add an int to a bignum.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to the answer on success, NULL on failure.
 */
bool bn_addint(bignum *n1, long long int n2)
{
	bignum *num2 = NULL, *result = NULL;

	if (!n1)
		return (NULL);

	num2 = int_to_bignuma(n2);
	if (!num2)
		return (NULL);

	result = bn_addition(n1, num2);
	bn_free(num2);
	return (result);
}

/**
 * bn_addint_inplace - add an int to a bignum inplace.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: true on success, false on failure.
 */
bool bn_addint_inplace(bignum *n1, long long int n2)
{
	bignum *num2 = NULL;

	if (!n1)
		return (false);

	num2 = int_to_bignuma(n2);
	if (!num2)
		return (false);

	bn_add_inplace(n1, num2);
	bn_free(num2);
	return (true);
}
