#include "numstr.h"

/**
 * bni_to_numstr - convert a bigint to a numstr.
 * @bi_arr: the bigint.
 * @base: an int between 2-36 indicating the base of the number.
 *
 * Return: a pointer to a numstr, NULL on failure.
 */
numstr *bi_to_numstr(bigint *bi_arr, const unsigned int base)
{
	numstr *nstr = NULL;
	int bytes_written = 0;
	size_t bi_i = 0, nstr_i = 0;

	if (!bi_arr || base < 2 || base > 36)
		return (NULL);

	nstr = alloc_numstr(bi_arr->len * count_digits(BIGINT_BASE - 1) + 1);
	for (bi_i = bi_arr->len, nstr_i = 0; nstr && bi_i > 0 && nstr_i < nstr->len;)
	{
		--bi_i;
		bytes_written = sprintf(&nstr->str[nstr_i], "%u", bi_arr->num[bi_i]);
		nstr_i += bytes_written - 1;
	}

	return (nstr);
}
