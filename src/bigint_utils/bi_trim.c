#include "bigint_utils.h"

/**
 * bi_trim - truncate length of a bigint to ignore trailing zeros.
 * @n: pointer to a bigint struct.
 *
 * Also sets the is_negative flag to 0 for 0s and NULL arrays.
 */
void bi_trim(bigint *const n)
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
