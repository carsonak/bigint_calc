#include "_bigint_internals.h"
#include "_numstr_internals.h"
#include "parse_number.h"

/**
 * numstr_to_bi - convert a numstr to a bigint.
 * @nstr: the numstr.
 *
 * Return: a pointer to a bigint_i struct, NULL on failure.
 */
bigint *numstr_to_bi(numstr *nstr)
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

	bi_arr = _bi_alloc(bi_i);
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
			return (bi_delete(bi_arr));
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

	_bi_trim(bi_arr);
	return (bi_arr);
}

/**
 * anybase_to_bi - convert a numstr in any base to a bigint.
 * @num: the numstr.
 * @base: an int between 2-36 indicating the base of the number.
 *
 * Return: a pointer to a bigint struct, NULL on failure.
 */
bigint *anybase_to_bi(numstr *num, const unsigned short int base)
{
	size_t i = 0;
	int cval = 0;
	bigint *bigint_final = NULL, *tmp = NULL;
	bigint base_bn = {.len = 1, .is_negative = false, .num = (u_int[1]){base}};
	bigint char_bn = {.len = 1, .is_negative = false, .num = (u_int[1]){0}};

	if (!num || !num->str || base < 2 || base > 36)
		return (NULL);

	bigint_final = _bi_alloc(1);
	if (!bigint_final)
		return (NULL);

	/* while str[i]; num = base * num + str[i]; ++i; */
	for (i = 0; i < num->len && bigint_final; ++i)
	{
		tmp = bigint_final;
		bigint_final = bi_multiply(&base_bn, bigint_final);
		tmp = bi_delete(tmp);
		cval = char_to_int(num->str[i]);
		if (cval < 0 || (u_int)cval >= base)
		{
			fprintf(
				stderr, "ParsingError: Invalid character '%c' for base%u\n",
				num->str[i], base);
			return (bi_delete(bigint_final));
		}

		char_bn.num[0] = cval;
		tmp = bigint_final;
		bigint_final = bi_add(bigint_final, &char_bn);
		tmp = bi_delete(tmp);
	}

	return (bigint_final);
}
