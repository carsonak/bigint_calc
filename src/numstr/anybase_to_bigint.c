#include "numstr.h"

/**
 * anybase_to_bni - convert a numstr in any base to a bigint.
 * @num: the numstr.
 * @base: an int between 2-36 indicating the base of the number.
 *
 * Return: a pointer to a bigint struct, NULL on failure.
 */
bigint *anybase_to_bni(numstr *num, unsigned int base)
{
	size_t i = 0;
	int cval = 0;
	u_int c_arr[1] = {0}, b_arr[1] = {base};
	bigint *bigint_final = NULL, *tmp = NULL;
	bigint base_bn = {.len = 1, .is_negative = false, .num = b_arr};
	bigint char_bn = {.len = 1, .is_negative = false, .num = c_arr};

	if (!num || !num->str || base < 2 || base > 36)
		return (NULL);

	bigint_final = bi_alloc(1);
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

		c_arr[0] = cval;
		tmp = bigint_final;
		bigint_final = bi_add(bigint_final, &char_bn);
		tmp = bi_delete(tmp);
	}

	return (bigint_final);
}
