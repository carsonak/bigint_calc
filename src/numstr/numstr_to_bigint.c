#include "numstr.h"

/**
 * numstr_to_bni - convert a numstr to a bigint_i.
 * @nstr: the numstr.
 *
 * Return: a pointer to a bigint_i struct, NULL on failure.
 */
bigint *numstr_to_bni(numstr *nstr)
{
    size_t bi_i = 0, nstr_i = 0, tmp = 0;
    unsigned int digits = 0;
    bigint *bi_arr = NULL;
    char num_buf[16], *end = NULL;

    if (!nstr || !nstr->len || !nstr->str || !isalnum(nstr->str[0]))
        return (NULL);

    /*sizeof(bigint_i) == */
    /*ceil(numstr.len / no. of digits that can represent BIGINT_BASE)*/
    digits = count_digits(BIGINT_BASE - 1);
    bi_i = (nstr->len / digits);
    if (nstr->len % digits)
        ++bi_i;

    bi_arr = bi_alloc(bi_i);
    if (!bi_arr)
        return (NULL);

    for (bi_i = 0, nstr_i = nstr->len; bi_i < bi_arr->len && nstr_i; ++bi_i)
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
            return (bi_free(bi_arr));
        }

        bi_arr->num[bi_i] = tmp % BIGINT_BASE;
        tmp /= BIGINT_BASE;
    }

    while (bi_i < bi_arr->len && tmp)
    {
        bi_arr->num[bi_i] = tmp % BIGINT_BASE;
        tmp /= BIGINT_BASE;
        ++bi_i;
    }

    bi_trim(bi_arr);
    return (bi_arr);
}
