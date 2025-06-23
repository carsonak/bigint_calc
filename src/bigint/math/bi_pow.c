/*!
 * @file
 * @brief methods for exponentiating bigint types.
 */

#include <stdio.h>  // fprintf

#include "_bi_internals.h"
#include "bigint.h"

static void multiply_and_replace(
	bigint *restrict *const restrict n1, bigint *const restrict n2
) _nonnull;

/*! Check if an integer is odd. */
#define INT_IS_ODD(num_to_test) (num_to_test % 2)

/*!
 * @brief "inplace" `bigint` multiplication.
 * @private @memberof bigint
 *
 * @param[in out] n1 address of the first `bigint` pointer.
 * @param[in] n2 pointer to the second `bigint`.
 */
static void multiply_and_replace(
	bigint *restrict *const restrict n1, bigint *const restrict n2
)
{
	bigint *restrict cpy = *n1;

	*n1 = bi_multiply(cpy, n2);
	_bi_free(cpy);
}

/*!
 * @brief handle exponentiation of a `bigint`.
 * @public @memberof bigint
 *
 * @param[in] base the base.
 * @param[in] exponent the exponent.
 *
 * @return pointer to the result, NULL on failure.
 */
bigint *bi_power(bigint *const restrict base, const intmax_t exponent)
{
	bigint *restrict x = NULL, *restrict y = NULL;
	intmax_t exp = exponent;

	if (!base || base->len < 0)
		return (NULL);

	if (exponent < 0)
	{
		fprintf(stderr, "Negative exponents not implemented.\n");
		return (NULL);
	}

	if (bi_isNaN(_bi_trim(base)))
		return (_bi_alloc(0));

	if (bi_iszero(base) || exponent == 0)
	{
		/* x^0==1 */
		x = _bi_alloc(1);
		if (x)
			x->num[0] = 1;

		return (x);
	}

	/* https://en.wikipedia.org/wiki/Exponentiation_by_squaring */
	/* x^n = (x^2) ^ n/2 if n is even or x(x^2) ^ (n-1)/2 if n is odd. */
	/* Since x^1==x we can iteratively apply the above formula till n < 2. */
	/* Another form can be written as: */
	/* yx^n = y(x^2) ^ n/2 if n is even or (yx)(x^2) ^ (n-1)/2 if n is odd. */
	/* With y starting at 1. */
	x = _bi_dup(base);
	y = _bi_alloc(1);
	if (!x || !y)
		goto cleanup;

	x->is_negative = false;
	y->num[0] = 1;
	while (x && y && exp > 1)
	{
		if (INT_IS_ODD(exp))
		{
			multiply_and_replace(&y, x);
			exp--;
		}

		multiply_and_replace(&x, x);
		if (!x || !y)
			goto cleanup;

		exp /= 2;
	}

	multiply_and_replace(&x, y);
	if (x && base->is_negative && INT_IS_ODD(exponent))
		x->is_negative = true;

	if (!y)
	{
cleanup:
		x = _bi_free(x);
	}

	y = _bi_free(y);
	return (x);
}
