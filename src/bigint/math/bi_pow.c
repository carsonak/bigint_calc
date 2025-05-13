#include "_bigint_internals.h"
#include "bigint.h"

static void ATTR_NONNULL_IDX(1)
	multiply_inplace(bigint **const n1, bigint *const n2);
static void ATTR_NONNULL_IDX(1)
	divide_inplace(bigint **const n1, bigint *const n2);
static void ATTR_NONNULL_IDX(1)
	subtract_inplace(bigint **const n1, bigint *const n2);

/**
 * multiply_inplace - "inplace" `bigint` multiplication.
 * @n1: address of the first `bigint` pointer.
 * @n2: pointer to the second `bigint`.
 */
static void multiply_inplace(bigint **const n1, bigint *const n2)
{
	bigint *cpy = *n1;

	*n1 = bi_multiply(cpy, n2);
	_bi_free(cpy);
}

/**
 * divide_inplace - "inplace" `bigint` division.
 * @n1: address of the first `bigint` pointer.
 * @n2: pointer to the second `bigint`.
 */
static void divide_inplace(bigint **const n1, bigint *const n2)
{
	bigint *cpy = *n1;

	*n1 = bi_divide(cpy, n2);
	_bi_free(cpy);
}

/**
 * subtract_inplace - "inplace" `bigint` subtraction.
 * @n1: address of the first `bigint` pointer.
 * @n2: pointer to the second `bigint`.
 */
static void subtract_inplace(bigint **const n1, bigint *const n2)
{
	if ((*n1)->len < n2->len)
		*n1 = _bi_resize(*n1, n2->len);

	bi_isubtract(*n1, n2);
}

/**
 * bi_power - handle exponentiation of a `bigint`.
 * @base: the base.
 * @exponent: the exponent.
 *
 * Return: pointer to the result, NULL on failure.
 */
bigint *bi_power(bigint *const base, bigint *const exponent)
{
	bigint tmp = {.len = 1, .is_negative = false, .num = (u_int[1]){1}};
	bigint *x = NULL, *y = NULL, *exp = NULL;

	if ((!base || !exponent) || (base->len < 0 || exponent->len < 0))
		return (NULL);

	_bi_trim(base);
	_bi_trim(exponent);
	if (bi_iszero(base) || bi_iszero(exponent))
	{
		x = _bi_alloc(1);
		if (x)
			x->num[0] = 1;

		return (x);
	}

	if (exponent->is_negative)
	{
		fprintf(stderr, "Negative exponents not implemented.\n");
		exit(EXIT_FAILURE);
	}

	exp = bi_dup(exponent);
	x = bi_dup(base);
	y = _bi_alloc(1);
	if (!exp || !x || !y)
		goto cleanup;

	x->is_negative = false;
	y->num[0] = 1;
	tmp.num[0] = 2;
	/* https://en.wikipedia.org/wiki/Exponentiation_by_squaring */
	/* x^n can be written as: */
	/* (x^2) ^ n/2 if n is even, x(x^2) ^ (n-1)/2 if n is odd. */
	/* This can be computed recursively, keeping in mind the special case */
	/* x^0==1 and base case as x^1==x. */
	/* Another form can be written as: */
	/* yx^n = y(x^2) ^ n/2 if n is even, (yx)(x^2) ^ (n-1)/2 if n is odd. */
	/* With y starting at 0. */
	while (x && y && exp && (exp->len > 1 || exp->num[0] > 1))
	{
		if (exp->num[0] % 2)
		{
			multiply_inplace(&y, x);
			tmp.num[0] = 1;
			subtract_inplace(&exp, &tmp);
			tmp.num[0] = 2;
		}

		multiply_inplace(&x, x);
		if (!exp || !x || !y)
			goto cleanup;

		divide_inplace(&exp, &tmp);
	}

	multiply_inplace(&x, y);
	if (exponent->is_negative && x)
	{
		tmp.num[0] = 1;
		divide_inplace(&x, &tmp);
	}

	if (x && base->is_negative && exponent->num[0] % 2)
		x->is_negative = true;

	if (false)
	{
cleanup:
		x = _bi_free(x);
	}

	_bi_free(exp);
	_bi_free(y);
	return (x);
}
