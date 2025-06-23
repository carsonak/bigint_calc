/*!
 * @file
 * @brief methods for creating bigint types.
 */

#include <string.h>  // memmove

#include "_bi_internals.h"
#include "bigint.h"
#include "number_parsing.h"

#include "safe_imaxabs.c"

static len_ty uint_to_array(digit_ty *const dest, uintmax_t num) _nonnull;

/*!
 * @brief convert an unsigned int to a `bigint` array.
 * @private @memberof bigint
 *
 * @param[out] dest pointer to the array to store the number.
 * @param[in] num the unsigned int to convert.
 *
 * @return number of used slots of the array.
 */
static len_ty uint_to_array(digit_ty *const dest, uintmax_t num)
{
	len_ty i = 0;

	while (num > 0)
	{
		dest[i] = num % BIGINT_BASE;
		num /= BIGINT_BASE;
		i++;
	}

	return (i);
}

/*!
 * @brief convert an integer to a `bigint` representation.
 * @public @memberof bigint
 *
 * @param[in] n the int to convert.
 *
 * @return a pointer to the `bigint`, NULL on failure.
 */
bigint *int_to_new_bi(const intmax_t n)
{
	digit_ty tmp[6] = {0};
	const len_ty i = (n == 0) ? 1 : uint_to_array(tmp, safe_imaxabs(n));
	bigint *const num = _bi_alloc(i);

	if (num)
	{
		if (n < 0)
			num->is_negative = true;

		memmove(num->num, tmp, sizeof(*tmp) * i);
	}

	return (num);
}

/*!
 * @brief store an integer into a `bigint`.
 * @public @memberof bigint
 *
 * `dest` must be large enough to store the integer, otherwise memory
 * might be corrupted when writing to the `bigint`.
 *
 * @param[out] dest a pointer to the `bigint` to store the integer in.
 * @param[in] n the int to convert.
 *
 * @return pointer to the bigint on success, NULL on failure.
 */
bigint *int_to_bi(bigint *const dest, const intmax_t n)
{
	if (!dest || !dest->num)
		return (NULL);

	digit_ty tmp[6] = {0};
	const len_ty i = (n == 0) ? 1 : uint_to_array(tmp, safe_imaxabs(n));

	if (n < 0)
		dest->is_negative = true;

	dest->len = i;
	memmove(dest->num, tmp, sizeof(*tmp) * i);
	return (dest);
}

/*!
 * @brief returns a pointer to a new `bigint` type representing a number.
 * @public @memberof bigint
 *
 * @param[in] number number to initialise the `bigint` with.
 * @param[in] base radix of the number in `number`.
 * @param[out] processed pointer to an int to store index of the last
 * character of the string to be processed.
 *
 * @return pointer to the `bigint`, NULL on failure.
 */
bigint *bi_new(
	char const *const restrict number, const radix_ty base,
	len_ty *const restrict processed
)
{
	numstr *const restrict ns = _numstr_new(number, base, processed);

	if (!ns)
		return (NULL);

	bigint *const restrict bi =
		(base == 10) ? _numstr_to_bi(ns) : _anybase_to_bi(ns);

	_numstr_free(ns);
	return (bi);
}
