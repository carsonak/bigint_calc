#include "bignum_utils.h"

/**
 * trim_bignum - truncate length of a bignum to ignore trailing zeros.
 * @arr: pointer to a bignum struct.
 *
 * Also sets the is_negative flag to 0 for 0s and NULL arrays.
 */
void trim_bignum(bignum * const arr)
{
	if (!arr)
		return;

	if (!arr->num)
		arr->len = 0;

	while (arr->len > 1 && !arr->num[arr->len - 1])
		--arr->len;

	if (arr->len <= 1)
	{
		if (!arr->num || !arr->num[0])
			arr->is_negative = false;
	}
}
