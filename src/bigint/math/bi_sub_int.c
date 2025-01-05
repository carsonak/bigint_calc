#include "_bigint_struct.h"
#include "bigint_math.h"

/**
 * bni_subtract_int - subtract an int from a bigint.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to the answer on success, NULL on failure.
 */
bigint *bi_subtract_int(bigint *const n1, long long int n2)
{
	bigint num2 = {.len = 4, .is_negative = 0, .num = (u_int[3]){0}};

	if (!n1)
		return (NULL);

	int_to_bi(&num2, n2);
	return (bi_subtract(n1, &num2));
}
