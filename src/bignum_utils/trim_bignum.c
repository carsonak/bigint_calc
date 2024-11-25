#include "bignum_utils.h"

/**
 * trim_bni - truncate length of a bignum to ignore trailing zeros.
 * @n: pointer to a bignum struct.
 *
 * Also sets the is_negative flag to 0 for 0s and NULL arrays.
 */
void trim_bni(bignum_i *const n)
{
	if (!n)
		return;

	if (!n->num)
		n->len = 0;

	while (n->len > 1 && !n->num[n->len - 1])
		--n->len;

	if (!n->num || (n->len == 1 && !n->num[0]))
		n->is_negative = false;
}
