#include "bignum_math.h"

static void ATTR_NONNULL_IDX(1) imultiply(bignum **n1, bignum *n2);
static void ATTR_NONNULL_IDX(1) idivide(bignum **n1, bignum *n2);
static void ATTR_NONNULL_IDX(1) isubtract(bignum **n1, bignum *n2);

/**
 * imultiply - "inplace" bignum multiplication.
 * @n1: address of the first bignum pointer.
 * @n2: pointer to the second bignum.
 */
static void imultiply(bignum **n1, bignum *n2)
{
	bignum *cpy = *n1;

	*n1 = bn_multiplication(cpy, n2);
	free_bignum(cpy);
}

/**
 * idivide - "inplace" bignum division.
 * @n1: address of the first bignum pointer.
 * @n2: pointer to the second bignum.
 */
static void idivide(bignum **n1, bignum *n2)
{
	bignum *cpy = *n1;

	*n1 = bn_division(cpy, n2);
	free_bignum(cpy);
}

/**
 * isubtract - "inplace" bignum subtraction.
 * @n1: address of the first bignum pointer.
 * @n2: pointer to the second bignum.
 */
static void isubtract(bignum **n1, bignum *n2)
{
	if ((*n1)->len < n2->len)
	{
		if (!realloc_bignum(*n1, n2->len))
			*n1 = free_bignum(*n1);
	}

	bn_sub_inplace(*n1, n2);
}

/**
 * bn_power - handle exponentiation of a bignum.
 * @base: the base.
 * @exponent: the exponent.
 *
 * Return: pointer to the result, NULL on failure.
 */
bignum *bn_power(bignum *base, bignum *exponent)
{
	uint one[1] = {1}, two[1] = {2};
	bignum tmp = {.len = 1, .is_negative = false, .num = two};
	bignum *x = NULL, *y = NULL, *exp = NULL;

	if (!base || !exponent)
		return (NULL);

	trim_bignum(base);
	trim_bignum(exponent);
	if (is_zero(base) || is_zero(exponent))
	{
		x = alloc_bignum(1);
		if (x)
			x->num[0] = 1;

		return (x);
	}

	exp = bignum_dup(exponent);
	x = bignum_dup(base);
	y = alloc_bignum(1);
	if (!exp || !x || !y)
		goto clean_up;

	exp->is_negative = false;
	x->is_negative = false;
	y->num[0] = 1;
	tmp.num = two;
	/*https://en.wikipedia.org/wiki/Exponentiation_by_squaring*/
	/*x^n can be written as: */
	/*(x^2) ^ n/2 if n is even, x(x^2) ^ (n-1)/2 if n is odd.*/
	/*This can be computed recursively, keeping in mind the special case */
	/*x^0==1 and base case as x^1==x.*/
	/*Another form can be written as: */
	/*yx^n = y(x^2) ^ n/2 if n is even, (yx)(x^2) ^ (n-1)/2 if n is odd.*/
	/*With y starting at 0.*/
	while (x && y && exp && (exp->len > 1 || exp->num[0] > 1))
	{
		if (exp->num[0] % 2)
		{
			imultiply(&y, x);
			tmp.num = one;
			isubtract(&exp, &tmp);
			tmp.num = two;
		}

		imultiply(&x, x);
		if (!exp || !x || !y)
			goto clean_up;

		idivide(&exp, &tmp);
	}

	imultiply(&x, y);
	if (exponent->is_negative && x)
	{
		tmp.num = one;
		idivide(&x, &tmp);
	}

	if (x && base->is_negative && exponent->num[0] % 2)
		x->is_negative = true;

	if (false)
	{
	clean_up:
		x = free_bignum(x);
	}

	free_bignum(exp);
	free_bignum(y);
	return (x);
}
