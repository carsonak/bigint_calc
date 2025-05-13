#include "_bigint_internals.h"
#include "_numstr_internals.h"
#include "bigint.h"
#include "parse_number.h"

#include "char_to_int.c"
#include "count_digits.c"
#include "is_valid_radix.c"

/**
 * numstr_to_bi - convert a numstr to a `bigint`.
 * @nstr: the numstr.
 *
 * Return: a pointer to a bigint_i struct, NULL on failure.
 */
bigint *numstr_to_bi(numstr *nstr)
{
	if (!nstr || nstr->len < 1 || !nstr->str || !isalnum(nstr->str[0]))
		return (NULL);

	len_type nstr_i = 0, bi_i = 0;
	uintmax_t carry = 0;
	/* sizeof(bigint_i) == */
	/* ceil(numstr.len / no. of digits that can represent BIGINT_BASE) */
	unsigned int digits = count_digits(BIGINT_BASE - 1);
	bigint *const bi =
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
		carry += strtoull(num_buf, &end, 10);
		if (*end)
		{
			fprintf(stderr, "ParsingError: Invalid character '%c'\n", *end);
			return (_bi_free(bi));
		}

		bi->num[bi_i] = carry % BIGINT_BASE;
		carry /= BIGINT_BASE;
	}

	while (bi_i < bi->len && carry)
	{
		bi->num[bi_i] = carry % BIGINT_BASE;
		carry /= BIGINT_BASE;
		++bi_i;
	}

	_bi_trim(bi);
	return (bi);
}

/**
 * anybase_to_bi - convert a numstr in any base to a `bigint`.
 * @num: the numstr.
 * @base: an int between 2-36 indicating the base of the number.
 *
 * Return: a pointer to a `bigint` struct, NULL on failure.
 */
bigint *anybase_to_bi(numstr *const num, const unsigned short int base)
{
	if (!num || num->len < 0 || !num->str || !is_valid_radix(base))
		return (NULL);

	len_type i = 0;
	bigint *bigint_final = _bi_alloc(1);

	if (!bigint_final)
		return (NULL);

	/* while str[i]; num = (base * num) + str[i]; ++i; */
	for (i = 0; i < num->len && bigint_final; ++i)
	{
		bigint *tmp = bigint_final;

		bigint_final = bi_multiply_int(bigint_final, base);
		tmp = _bi_free(tmp);
		int cval = char_to_int(num->str[i]);
		if (cval < 0 || (u_int)cval >= base)
		{
			fprintf(
				stderr, "ParsingError: Invalid character '%c' for base%u\n",
				num->str[i], base);
			return (_bi_free(bigint_final));
		}

		tmp = bigint_final;
		bigint_final = bi_add_int(bigint_final, cval);
		tmp = _bi_free(tmp);
	}

	return (bigint_final);
}
