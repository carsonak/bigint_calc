#include "bignum_math.h"

static void ATTR_NONNULL_IDX(1) imultiply(bignum_i **n1, bignum_i *const n2);
static void ATTR_NONNULL_IDX(1) idivide(bignum_i **n1, bignum_i *const n2);
static void ATTR_NONNULL_IDX(1) isubtract(bignum_i **n1, bignum_i *const n2);

/**
 * imultiply - "inplace" bignum_i multiplication.
 * @n1: address of the first bignum_i pointer.
 * @n2: pointer to the second bignum_i.
 */
static void imultiply(bignum_i **n1, bignum_i *const n2)
{
	bignum_i *cpy = *n1;

	*n1 = bni_multiply(cpy, n2);
	bni_free(cpy);
}

/**
 * idivide - "inplace" bignum_i division.
 * @n1: address of the first bignum_i pointer.
 * @n2: pointer to the second bignum_i.
 */
static void idivide(bignum_i **n1, bignum_i *const n2)
{
	bignum_i *cpy = *n1;

	*n1 = bni_divide(cpy, n2);
	bni_free(cpy);
}

/**
 * isubtract - "inplace" bignum_i subtraction.
 * @n1: address of the first bignum_i pointer.
 * @n2: pointer to the second bignum_i.
 */
static void isubtract(bignum_i **n1, bignum_i *const n2)
{
	if ((*n1)->len < n2->len)
	{
		if (!bn_realloc(*n1, n2->len))
			*n1 = bni_free(*n1);
	}

	bni_isubtract(*n1, n2);
}

/**
 * bni_power - handle exponentiation of a bignum_i.
 * @base: the base.
 * @exponent: the exponent.
 *
 * Return: pointer to the result, NULL on failure.
 */
bignum_i *bni_power(bignum_i *base, bignum_i *exponent)
{
	u_int one[1] = {1}, two[1] = {2};
	bignum_i tmp = {.len = 1, .is_negative = false, .num = two};
	bignum_i *x = NULL, *y = NULL, *exp = NULL;

	if (!base || !exponent)
		return (NULL);

	trim_bni(base);
	trim_bni(exponent);
	if (bni_is_zero(base) || bni_is_zero(exponent))
	{
		x = bni_alloc(1);
		if (x)
			x->num[0] = 1;

		return (x);
	}

	exp = bn_dup(exponent);
	x = bn_dup(base);
	y = bni_alloc(1);
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
	x = bni_free(x);
	}

	bni_free(exp);
	bni_free(y);
	return (x);
}
