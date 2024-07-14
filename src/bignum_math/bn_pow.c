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
    uint a[1] = {0};
    bignum tmp = {.len = 1, .is_negative = 0, .num = a};
    bignum *res = NULL, *res_cp = NULL, *exp = NULL, *exp_cp = NULL;

    if (!base || !exponent)
        return (NULL);

    trim_bignum(base);
    trim_bignum(exponent);
    if (!cmp_bignum(base, &tmp) || !cmp_bignum(exponent, &tmp))
    {
        res = alloc_bignum(1);
        if (res && cmp_bignum(base, &tmp))
            res->num[0] = 1;

        return (res);
    }

    exp = bignum_dup(exponent);
    res = bignum_dup(base);
    if (res)
        res->is_negative = false;

    if (res && exp && exp->num[0] % 2)
    {
        exp_cp = exp;
        res_cp = res;
        res = bn_multiplication(res, res);
        a[0] = 1;
        exp = bn_division(exp, &tmp);
        free_bignum(exp_cp);
        free_bignum(res_cp);
    }

    a[0] = 2;
    while (res && exp && (exp->len > 1 || exp->num[0] > 1))
    {
        exp_cp = exp;
        res_cp = res;
        res = bn_multiplication(res, res);
        exp = bn_division(exp, &tmp);
        free_bignum(exp_cp);
        free_bignum(res_cp);
    }

    if (exponent->is_negative)
    {
        res_cp = res;
        a[0] = 1;
        res = bn_division(&tmp, res);
        free_bignum(res_cp);
    }

    free_bignum(exp);
    return (res);
}
