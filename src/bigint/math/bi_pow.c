#include "_bigint_internals.h"
#include "bigint.h"

static void
multiply_and_replace(bigint **const restrict n1, bigint *const restrict n2);

/**
 * multiply_and_replace - "inplace" `bigint` multiplication.
 * @n1: address of the first `bigint` pointer.
 * @n2: pointer to the second `bigint`.
 */
static void
multiply_and_replace(bigint **const restrict n1, bigint *const restrict n2)
{
	bigint *cpy = *n1;

	*n1 = bi_multiply(cpy, n2);
	_bi_free(cpy);
}

/**
 * bi_power - handle exponentiation of a `bigint`.
 * @base: the base.
 * @exponent: the exponent.
 *
 * Return: pointer to the result, NULL on failure.
 */
bigint *bi_power(bigint *const base, const intmax_t exponent)
{
	bigint *x = NULL, *y = NULL;
	intmax_t exp = exponent;

	if (!base || base->len < 0)
		return (NULL);

	if (exponent < 0)
	{
		fprintf(stderr, "Negative exponents not implemented.\n");
		return (NULL);
	}

	_bi_trim(base);
	if (bi_isNaN(base))
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
			--exp;
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

	_bi_free(y);
	return (x);
}
