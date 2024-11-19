#include "bignum_utils.h"

/**
 * int_to_bignuma - convert an integer to a bignum representation.
 * @n: the int to convert.
 *
 * Return: a pointer to the bignum, NULL on failure.
 */
bignum *int_to_bignuma(long long int n)
{
	u_int tmp[4] = {0};
	unsigned long long int un = llabs(n);
	unsigned int i = 0;
	bignum *num = NULL;

	while (un)
	{
		tmp[i] = un % BIGNUM_BASE;
		un /= BIGNUM_BASE;
		++i;
	}

	if (!n)
		i = 1;

	num = bn_alloc(i);
	if (num)
	{
		if (n < 0)
			num->is_negative = true;

		memmove(num->num, tmp, sizeof(*tmp) * i);
	}

	return (num);
}

/**
 * int_to_bignum - convert an integer to a bignum representation.
 * @dest: a pointer to a bignum struct to store the number.
 * @n: the int to convert.
 *
 * Return: true on success, false on failure.
 */
bool int_to_bignum(bignum *dest, long long int n)
{
	u_int tmp[4] = {0};
	unsigned long long int un = llabs(n);
	unsigned int i = 0;

	if (!dest || !dest->len || !dest->num)
		return (false);

	while (un)
	{
		tmp[i] = un % BIGNUM_BASE;
		un /= BIGNUM_BASE;
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
