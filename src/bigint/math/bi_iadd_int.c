#include "_bigint_struct.h"
#include "bigint_math.h"

/**
 * bni_iadd_int - add an int to a bigint inplace.
 * @n1: the first number, must have enough memory allocated to hold the answer.
 * @n2: the second number.
 *
 * The results of the addittion will be stored in n1. No extra memory
 * will be allocated via calls to *alloc family functions.
 *
 * Return: true on success, false on failure.
 */
bool bi_iadd_int(bigint *const n1, long long int n2)
{
	bigint num2 = {.len = 4, .is_negative = 0, .num = (u_int[3]){0}};

	if (!n1)
		return (false);

	int_to_bi(&num2, n2);
	return (bi_iadd(n1, &num2));
}
