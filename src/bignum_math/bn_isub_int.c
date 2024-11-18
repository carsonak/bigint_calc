#include "bignum_math.h"

/**
 * bn_subint_inplace - subtract an int from a bignum inplace.
 * @n1: the first number, must have enough memory allocated to hold the answer.
 * @n2: the second number.
 *
 * The results of the subtraction will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * Return: true on success, false on failure.
 */
bool bn_subint_inplace(bignum * const n1, long long int n2)
{
	bignum num2 = {.len = 4, .is_negative = 0, .num = (u_int[3]) {0}};

	if (!n1)
		return (false);

	int_to_bignum(&num2, n2);
	return (bn_sub_inplace(n1, &num2));
}
