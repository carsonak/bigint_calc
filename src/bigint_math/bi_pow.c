#include "bigint_math.h"

static void ATTR_NONNULL_IDX(1) imultiply(bigint **const n1, bigint *const n2);
static void ATTR_NONNULL_IDX(1) idivide(bigint **const n1, bigint *const n2);
static void ATTR_NONNULL_IDX(1) isubtract(bigint **const n1, bigint *const n2);

/**
 * imultiply - "inplace" bigint multiplication.
 * @n1: address of the first bigint pointer.
 * @n2: pointer to the second bigint.
 */
static void imultiply(bigint **const n1, bigint *const n2)
{
	bigint *cpy = *n1;

	*n1 = bi_multiply(cpy, n2);
	bi_delete(cpy);
}

/**
 * idivide - "inplace" bigint division.
 * @n1: address of the first bigint pointer.
 * @n2: pointer to the second bigint.
 */
static void idivide(bigint **const n1, bigint *const n2)
{
	bigint *cpy = *n1;

	*n1 = bi_divide(cpy, n2);
	bi_delete(cpy);
}

/**
 * isubtract - "inplace" bigint subtraction.
 * @n1: address of the first bigint pointer.
 * @n2: pointer to the second bigint.
 */
static void isubtract(bigint **const n1, bigint *const n2)
{
	if ((*n1)->len < n2->len)
	{
		bigint *tmp = bi_resize(*n1, n2->len);
		if (!tmp)
			*n1 = bi_delete(*n1);

		*n1 = tmp;
	}

	bi_isubtract(*n1, n2);
}

/**
 * bni_power - handle exponentiation of a bigint.
 * @base: the base.
 * @exponent: the exponent.
 *
 * Return: pointer to the result, NULL on failure.
 */
bigint *bi_power(bigint *const base, bigint *const exponent)
{
	u_int one[1] = {1}, two[1] = {2};
	bigint tmp = {.len = 1, .is_negative = false, .num = two};
	bigint *x = NULL, *y = NULL, *exp = NULL;

	if (!base || !exponent)
		return (NULL);

	bi_trim(base);
	bi_trim(exponent);
	if (bi_is_zero(base) || bi_is_zero(exponent))
	{
		x = bi_alloc(1);
		if (x)
			x->num[0] = 1;

		return (x);
	}

	exp = bi_dup(exponent);
	x = bi_dup(base);
	y = bi_alloc(1);
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
		x = bi_delete(x);
	}

	bi_delete(exp);
	bi_delete(y);
	return (x);
}
