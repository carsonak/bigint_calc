#include "bignum_math.h"

/**
 * bni_isubtract_int - subtract an int from a bignum_i inplace.
 * @n1: the first number, must have enough memory allocated to hold the answer.
 * @n2: the second number.
 *
 * The results of the subtraction will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * Return: true on success, false on failure.
 */
bool bni_isubtract_int(bignum_i *const n1, long long int n2)
{
	bignum_i num2 = {.len = 4, .is_negative = 0, .num = (u_int[3]){0}};

	if (!n1)
		return (false);

	int_to_bni(&num2, n2);
	return (bni_isubtract(n1, &num2));
}
