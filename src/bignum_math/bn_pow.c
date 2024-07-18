#include "infiX.h"

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
    bignum *x = NULL, *y = NULL, *exp = NULL, *cpy = NULL;

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
    {
        free_bignum(exp);
        free_bignum(y);
        return (free_bignum(x));
    }

    exp->is_negative = false;
    x->is_negative = false;
    y->num[0] = 1;
    tmp.num = two;
    while (x && y && exp && (exp->len > 1 || exp->num[0] > 1))
    {
        if (exp->num[0] % 2)
        {
            cpy = y;
            y = bn_multiplication(y, x);
            free_bignum(cpy);
            tmp.num = one;
            cpy = exp;
            exp = bn_subtraction(exp, &tmp);
            free_bignum(cpy);
            tmp.num = two;
        }

        cpy = x;
        x = bn_multiplication(x, x);
        free_bignum(cpy);
        cpy = exp;
        exp = bn_division(exp, &tmp);
        free_bignum(cpy);
    }

    cpy = x;
    x = bn_multiplication(x, y);
    free_bignum(cpy);
    if (exponent->is_negative)
    {
        tmp.num = one;
        cpy = x;
        x = bn_division(&tmp, x);
        free_bignum(cpy);
    }

    if (x && base->is_negative && exponent->num[0] % 2)
        x->is_negative = true;

    free_bignum(exp);
    free_bignum(y);
    return (x);
}
