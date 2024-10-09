#include "bignum_math.h"

/**
 * bn_add_int - add an int to a bignum.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to the answer on success, NULL on failure.
 */
bignum *bn_add_int(bignum *const n1, long long int n2)
{
	bignum num2 = {.len = 4, .is_negative = 0, .num = (u_int[3]) {0}};

	if (!n1)
		return (NULL);

	int_to_bignum(&num2, n2);
	return (bn_add(n1, &num2));
}
