#include "_bi_internals.h"
#include "bigint.h"
#include "number_parsing.h"

#include "safe_imaxabs.c"

static len_type uint_to_array(u_int *const dest, uintmax_t num) ATTR_NONNULL;

/**
 * uint_to_array - convert an unsigned int to a `bigint` array.
 * @dest: pointer to the array to store the number.
 * @num: the unsigned int to convert.
 *
 * Return: number of used slots of the array.
 */
static len_type uint_to_array(u_int *const dest, uintmax_t num)
{
	len_type i = 0;

	while (num > 0)
	{
		dest[i] = num % BIGINT_BASE;
		num /= BIGINT_BASE;
		++i;
	}

	return (i);
}

/**
 * int_to_new_bi - convert an integer to a `bigint` representation.
 * @n: the int to convert.
 *
 * Return: a pointer to the `bigint`, NULL on failure.
 */
bigint *int_to_new_bi(const intmax_t n)
{
	u_int tmp[4] = {0};
	const len_type i = (n == 0) ? 1 : uint_to_array(tmp, safe_imaxabs(n));
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
 * int_to_bigint - store an integer into a `bigint`.
 * @dest: a pointer to the `bigint` to store the integer in.
 * @n: the int to convert.
 *
 * `dest` must be large enough to store the integer, otherwise memory
 * might be corrupted when writing to the `bigint`.
 *
 * Return: pointer to the bigint on success, NULL on failure.
 */
bigint *int_to_bi(bigint *const dest, const intmax_t n)
{
	if (!dest || !dest->num)
		return (NULL);

	u_int tmp[4] = {0};
	const len_type i = (n == 0) ? 1 : uint_to_array(tmp, safe_imaxabs(n));

	if (n < 0)
		dest->is_negative = true;

	dest->len = i;
	memmove(dest->num, tmp, sizeof(*tmp) * i);
	return (dest);
}

/**
 * bi_new - returns a pointer to a new `bigint` type representing a number.
 * @number: number to initialise the `bigint` with.
 * @base: radix of the number in `number`.
 * @processed: pointer to a variable to store index of the last character to be
 * processed.
 *
 * Return: pointer to the `bigint`, NULL on failure.
 */
bigint *bi_new(
	char const *const restrict number, const radix_type base,
	len_type *const restrict processed
)
{
	numstr *const ns = _str_to_numstr(number, base, processed);

	if (!ns)
		return (NULL);

	bigint *const bi =
		(base == 10) ? _numstr_to_bi(ns) : _anybase_to_bi(ns, base);

	_numstr_free(ns);
	return (bi);
}
