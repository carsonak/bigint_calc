#include "numstr.h"

/**
 * anybase_to_bni - convert a numstr in any base to a bignum.
 * @num: the numstr.
 * @base: an int between 2-36 indicating the base of the number.
 *
 * Return: a pointer to a bignum struct, NULL on failure.
 */
bignum_i *anybase_to_bni(numstr *num, unsigned int base)
{
	size_t i = 0;
	int cval = 0;
	u_int c_arr[1] = {0}, b_arr[1] = {base};
	bignum_i *bigint = NULL, *tmp = NULL;
	bignum_i base_bn = {.len = 1, .is_negative = false, .num = b_arr};
	bignum_i char_bn = {.len = 1, .is_negative = false, .num = c_arr};

	if (!num || !num->str || base < 2 || base > 36)
		return (NULL);

	bigint = bni_alloc(1);
	if (!bigint)
		return (NULL);

	/* while str[i]; num = base * num + str[i]; ++i; */
	for (i = 0; i < num->len && bigint; ++i)
	{
		tmp = bigint;
		bigint = bni_multiply(&base_bn, bigint);
		tmp = bni_free(tmp);
		cval = char_to_int(num->str[i]);
		if (cval < 0 || (u_int)cval >= base)
		{
			fprintf(
				stderr, "ParsingError: Invalid character '%c' for base%u\n",
				num->str[i], base);
			return (bni_free(bigint));
		}

		c_arr[0] = cval;
		tmp = bigint;
		bigint = bni_add(bigint, &char_bn);
		tmp = bni_free(tmp);
	}

	return (bigint);
}
