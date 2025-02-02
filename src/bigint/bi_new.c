#include "_bigint_internals.h"
#include "_numstr_internals.h"
#include "bigint.h"
#include "parse_number.h"

/**
 * int_to_new_bi - convert an integer to a bigint representation.
 * @n: the int to convert.
 *
 * Return: a pointer to the bigint, NULL on failure.
 */
bigint *int_to_new_bi(const long long int n)
{
	u_int tmp[4] = {0};
	unsigned int i = 0;

	if (!n)
		i = 1;
	else
	{
		unsigned long long int un = llabs(n);

		while (un)
		{
			tmp[i] = un % BIGINT_BASE;
			un /= BIGINT_BASE;
			++i;
		}
	}

	bigint *const num = _bi_alloc(i);

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
 * `dest` must be large enough to store the number, otherwise memory
 * might be corrupted when writing to the bigint.
 *
 * Return: true on success, false on failure.
 */
bool int_to_bi(bigint *const dest, const long long int n)
{
	u_int tmp[4] = {0};
	unsigned int i = 0;

	if (!dest || !dest->num)
		return (false);

	if (!n)
		i = 1;
	else
	{
		unsigned long long int un = llabs(n);

		while (un)
		{
			tmp[i] = un % BIGINT_BASE;
			un /= BIGINT_BASE;
			++i;
		}
	}

	if (n < 0)
		dest->is_negative = true;

	dest->len = i;
	memmove(dest->num, tmp, sizeof(*tmp) * i);
	return (true);
}

/**
 * bi_new - returns a pointer to a new bigint type representing a number.
 * @number: number to initialise the bigint with.
 * @base: radix of the number in `number`.
 * @processed: pointer to a variable to store index of the last character to be
 * processed.
 *
 * Return: pointer to the bigint, NULL on failure.
 */
bigint *bi_new(
	char const *const number, const unsigned short int base,
	size_t *const processed)
{
	numstr *const ns = str_to_numstr(number, base, processed);

	if (!ns)
		return (NULL);

	bigint *const bi =
		(base == 10) ? numstr_to_bi(ns) : anybase_to_bi(ns, base);

	_numstr_free(ns);
	return (bi);
}
