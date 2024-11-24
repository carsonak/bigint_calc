#include "parsing.h"

/**
 * numstr_to_bignum - convert a numstr to a bignum.
 * @nstr: the numstr.
 *
 * Return: a pointer to a bignum struct, NULL on failure.
 */
bignum *numstr_to_bignum(numstr *nstr)
{
    size_t bn_i = 0, nstr_i = 0, tmp = 0;
    unsigned int digits = 0;
    bignum *bn_arr = NULL;
    char num_buf[16], *end = NULL;

    if (!nstr || !nstr->len || !nstr->str || !isalnum(nstr->str[0]))
        return (NULL);

    /*sizeof(bignum) == */
    /*ceil(numstr.len / no. of digits that can represent BIGNUM_BASE)*/
    digits = count_digits(BIGNUM_BASE - 1);
    bn_i = (nstr->len / digits);
    if (nstr->len % digits)
        ++bn_i;

    bn_arr = bn_alloc(bn_i);
    if (!bn_arr)
        return (NULL);

    for (bn_i = 0, nstr_i = nstr->len; bn_i < bn_arr->len && nstr_i; ++bn_i)
    {
        if (nstr_i >= digits)
            nstr_i -= digits;
        else
        {
            digits = nstr_i;
            nstr_i = 0;
        }

        strncpy(num_buf, &nstr->str[nstr_i], digits);
        num_buf[digits] = '\0';
        tmp += strtoull(num_buf, &end, 10);
        if (*end)
        {
            fprintf(stderr, "ParsingError: Invalid character '%c'\n", *end);
            return (bn_free(bn_arr));
        }

        bn_arr->num[bn_i] = tmp % BIGNUM_BASE;
        tmp /= BIGNUM_BASE;
    }

    while (bn_i < bn_arr->len && tmp)
    {
        bn_arr->num[bn_i] = tmp % BIGNUM_BASE;
        tmp /= BIGNUM_BASE;
        ++bn_i;
    }

    trim_bignum(bn_arr);
    return (bn_arr);
}
