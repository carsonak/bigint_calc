/*!
 * @file
 * @brief methods for creating Bigint types.
 */

#include <string.h>  // memmove

#include "_bi_internals.h"
#include "Bigint.h"
#include "number_parsing.h"

#include "safe_imaxabs.c"

static len_ty uint_to_array(udigit_ty *const dest, uintmax_t num) _nonnull;

/*!
 * @brief convert an unsigned int to a `Bigint` array.
 * @private @memberof Bigint
 *
 * @param[out] dest pointer to the array to store the number.
 * @param[in] num the unsigned int to convert.
 *
 * @return number of used slots of the array.
 */
static len_ty uint_to_array(udigit_ty *const dest, uintmax_t num)
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
 * @brief convert an integer to a `Bigint` representation.
 * @public @memberof Bigint
 *
 * @param[in] n the int to convert.
 *
 * @return a pointer to the `Bigint`, NULL on failure.
 */
Bigint *int_to_new_bi(const intmax_t n)
{
	udigit_ty tmp[6] = {0};
	const len_ty i = (n == 0) ? 1 : uint_to_array(tmp, safe_imaxabs(n));
	Bigint *const num = _bi_alloc(i);

	if (num)
	{
		if (n < 0)
			num->is_negative = true;

		memmove(num->num, tmp, sizeof(*tmp) * i);
	}

	return (num);
}

/*!
 * @brief store an integer into a `Bigint`.
 * @public @memberof Bigint
 *
 * `dest` must be large enough to store the integer, otherwise memory
 * might be corrupted when writing to the `Bigint`.
 *
 * @param[out] dest a pointer to the `Bigint` to store the integer in.
 * @param[in] n the int to convert.
 *
 * @return pointer to the Bigint on success, NULL on failure.
 */
Bigint *int_to_bi(Bigint *const dest, const intmax_t n)
{
	if (!dest || !dest->num)
		return (NULL);

	udigit_ty tmp[6] = {0};
	const len_ty i = (n == 0) ? 1 : uint_to_array(tmp, safe_imaxabs(n));

	if (n < 0)
		dest->is_negative = true;

	dest->len = i;
	memmove(dest->num, tmp, sizeof(*tmp) * i);
	return (dest);
}

/*!
 * @brief returns a pointer to a new `Bigint` type representing a number.
 * @public @memberof Bigint
 *
 * @param[in] number number to initialise the `Bigint` with.
 * @param[in] base radix of the number in `number`.
 * @param[out] processed pointer to an int to store index of the last
 * character of the string to be processed.
 *
 * @return pointer to the `Bigint`, NULL on failure.
 */
Bigint *bi_new(
	char const *const restrict number, const radix_ty base,
	len_ty *const restrict processed
)
{
	numstr *const restrict ns = _numstr_new(number, base, processed);

	if (!ns)
		return (NULL);

	Bigint *const restrict bi =
		(base == 10) ? _numstr_to_bi(ns) : _anybase_to_bi(ns);

	_numstr_free(ns);
	return (bi);
}
