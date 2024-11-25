#include "bignum_math.h"

/**
 * bni_subtract_int - subtract an int from a bignum.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to the answer on success, NULL on failure.
 */
bignum_i *bni_subtract_int(bignum_i *const n1, long long int n2)
{
	bignum_i num2 = {.len = 4, .is_negative = 0, .num = (u_int[3]){0}};

	if (!n1)
		return (NULL);

	int_to_bni(&num2, n2);
	return (bni_subtract(n1, &num2));
}
