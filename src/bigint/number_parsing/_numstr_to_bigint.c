#include <inttypes.h> /* strtoumax */
#include <string.h>   /* strncpy */

#include "_bi_internals.h"
#include "bigint.h"
#include "number_parsing.h"

#include "char_to_int.c"
#include "count_digits.c"
#include "is_valid_radix.c"

/*!
 * @brief convert a `numstr` to a `bigint`.
 * @public @memberof numstr
 *
 * @param[in] nstr pointer to the `numstr`.
 *
 * @return a pointer to the `bigint`, NULL on failure.
 */
bigint *_numstr_to_bi(const numstr *const restrict nstr)
{
	if (!nstr || nstr->len < 1 || !nstr->str || !isalnum(nstr->str[0]))
		return (NULL);

	len_type nstr_i = 0, bi_i = 0;
	/* sizeof(`bigint`) == */
	/* ceil(numstr.len / no. of digits that can represent BIGINT_BASE) */
	unsigned int digits = count_digits(BIGINT_BASE - 1, 10);
	bigint *const restrict bi =
		_bi_alloc((nstr->len / digits) + (nstr->len % digits ? 1 : 0));

	if (!bi)
		return (NULL);

	for (bi_i = 0, nstr_i = nstr->len; bi_i < bi->len && nstr_i > 0; ++bi_i)
	{
		char num_buf[16], *end = NULL;

		if (nstr_i >= digits)
			nstr_i -= digits;
		else
		{
			digits = nstr_i;
			nstr_i = 0;
		}

		strncpy(num_buf, &nstr->str[nstr_i], digits);
		num_buf[digits] = '\0';
		bi->num[bi_i] = strtoumax(num_buf, &end, 10);
		if (*end)
		{
			fprintf(stderr, "ParsingError: Invalid character '%c'\n", *end);
			return (_bi_free(bi));
		}
	}

	bi->is_negative = nstr->is_negative;
	return (_bi_trim(bi));
}

/*!
 * @brief convert a `numstr` in any base to a `bigint`.
 * @public @memberof numstr
 *
 * @param[in] num the numstr.
 *
 * @return a pointer to the `bigint`, NULL on failure.
 */
bigint *_anybase_to_bi(const numstr *const restrict num)
{
	if (!num || num->len < 1 || !num->str || !is_valid_radix(num->base))
		return (NULL);

	len_type i = 0;
	bigint *bigint_final = _bi_alloc(1); /* num = 0 */

	if (!bigint_final)
		return (NULL);

	/* while str[i]; num = (base * num) + str[i]; ++i; */
	for (i = 0; i < num->len && bigint_final; ++i)
	{
		bigint *tmp = bigint_final;

		bigint_final = bi_multiply_int(bigint_final, num->base);
		tmp = _bi_free(tmp);
		int cval = char_to_int(num->str[i]);
		if (cval < 0 || (u_int)cval >= num->base)
		{
			fprintf(
				stderr, "ParsingError: Invalid character '%c' for base%u\n",
				num->str[i], num->base
			);
			return (_bi_free(bigint_final));
		}

		tmp = bigint_final;
		bigint_final = bi_add_int(bigint_final, cval);
		tmp = _bi_free(tmp);
	}

	return (bigint_final);
}
