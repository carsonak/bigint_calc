#include "numstr.h"

/**
 * bni_to_numstr - convert a bignum to a numstr.
 * @bn_arr: the bignum.
 * @base: an int between 2-36 indicating the base of the number.
 *
 * Return: a pointer to a numstr, NULL on failure.
 */
numstr *bni_to_numstr(bignum_i *bn_arr, const unsigned int base)
{
	numstr *nstr = NULL;
	int bytes_written = 0;
	size_t bn_i = 0, nstr_i = 0;

	if (!bn_arr || base < 2 || base > 36)
		return (NULL);

	nstr = alloc_numstr(bn_arr->len * count_digits(BIGNUM_BASE - 1) + 1);
	for (bn_i = bn_arr->len, nstr_i = 0; nstr && bn_i > 0 && nstr_i < nstr->len;)
	{
		--bn_i;
		bytes_written = sprintf(&nstr->str[nstr_i], "%u", bn_arr->num[bn_i]);
		nstr_i += bytes_written - 1;
	}

	return (nstr);
}
