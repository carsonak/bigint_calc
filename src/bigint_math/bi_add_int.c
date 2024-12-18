#include "bigint_math.h"

/**
 * bni_add_int - add an int to a bigint.
 * @n1: the first number.
 * @n2: the second number.
 *
 * Return: pointer to the answer on success, NULL on failure.
 */
bigint *bi_add_int(bigint *const n1, long long int n2)
{
	bigint num2 = {.len = 4, .is_negative = 0, .num = (u_int[3]){0}};

	if (!n1)
		return (NULL);

	int_to_bni(&num2, n2);
	return (bi_add(n1, &num2));
}
