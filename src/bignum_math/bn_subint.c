#include "bignum_math.h"

/**
 * bn_subint - subtract an int from a bignum.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to the answer on success, NULL on failure.
 */
bool bn_subint(bignum *n1, long long int n2)
{
	bignum *num2 = NULL, *result = NULL;

	if (!n1)
		return (NULL);

	num2 = int_to_bignuma(n2);
	if (!num2)
		return (NULL);

	result = bn_subtraction(n1, num2);
	bn_free(num2);
	return (result);
}

/**
 * bn_subint_inplace - subtract an int from a bignum inplace.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: true on success, false on failure.
 */
bool bn_subint_inplace(bignum *n1, long long int n2)
{
	bignum *num2 = NULL;

	if (!n1)
		return (false);

	num2 = int_to_bignuma(n2);
	if (!num2)
		return (false);

	bn_sub_inplace(n1, num2);
	bn_free(num2);
	return (true);
}
