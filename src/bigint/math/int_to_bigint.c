#include "bigint_utils.h"

/**
 * int_to_new_bi - convert an integer to a bigint representation.
 * @n: the int to convert.
 *
 * Return: a pointer to the bigint, NULL on failure.
 */
bigint *int_to_new_bi(long long int n)
{
	u_int tmp[4] = {0};
	unsigned long long int un = llabs(n);
	unsigned int i = 0;
	bigint *num = NULL;

	while (un)
	{
		tmp[i] = un % BIGINT_BASE;
		un /= BIGINT_BASE;
		++i;
	}

	if (!n)
		i = 1;

	num = _bi_alloc(i);
	if (num)
	{
		if (n < 0)
			num->is_negative = true;

		memmove(num->num, tmp, sizeof(*tmp) * i);
	}

	return (num);
}

/**
 * int_to_bigint - convert an integer to a bigint representation.
 * @dest: a pointer to a bigint struct to store the number.
 * @n: the int to convert.
 *
 * Return: true on success, false on failure.
 */
bool int_to_bi(bigint *dest, long long int n)
{
	u_int tmp[4] = {0};
	unsigned long long int un = llabs(n);
	unsigned int i = 0;

	if (!dest || !dest->len || !dest->num)
		return (false);

	while (un)
	{
		tmp[i] = un % BIGINT_BASE;
		un /= BIGINT_BASE;
		++i;
	}

	if (!n)
		i = 1;

	if (dest->len < i)
		return (false);

	if (n < 0)
		dest->is_negative = true;

	dest->len = i;
	memmove(dest->num, tmp, sizeof(*tmp) * i);
	return (true);
}
