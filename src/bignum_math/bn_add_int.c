#include "bignum_math.h"

/**
 * bni_add_int - add an int to a bignum_i.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to the answer on success, NULL on failure.
 */
bignum_i *bni_add_int(bignum_i *const n1, long long int n2)
{
	bignum_i num2 = {.len = 4, .is_negative = 0, .num = (u_int[3]){0}};

	if (!n1)
		return (NULL);

	int_to_bni(&num2, n2);
	return (bni_add(n1, &num2));
}
