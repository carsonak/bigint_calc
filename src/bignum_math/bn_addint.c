#include "bignum_math.h"

/**
 * bn_add_int - add an int to a bignum.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to the answer on success, NULL on failure.
 */
bool bn_add_int(bignum * const n1, long long int n2)
{
	bignum num2 = {.len = 4, .is_negative = 0, .num = (u_int[]){0}};

	if (!n1)
		return (NULL);

	int_to_bignum(&num2, n2);
	return (bn_add(n1, &num2));
}

/**
 * bn_addint_inplace - add an int to a bignum inplace.
 * @n1: the first number, must have enough memory allocated to hold the answer.
 * @n2: the second number.
 *
 * The results of the addittion will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * Return: true on success, false on failure.
 */
bool bn_addint_inplace(bignum *const n1, long long int n2)
{
	bignum num2 = {.len = 4, .is_negative = 0, .num = (u_int[]){0}};

	if (!n1)
		return (false);

	int_to_bignum(&num2, n2);
	return (bn_add_inplace(n1, &num2));
}
