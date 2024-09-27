#include "bignum_utils.h"

/**
 * bn_copy - shallow copies a bignum struct (pointers won't be dereferenced).
 * @src: the bignum to be copied.
 * @dest: where to copy to.
 *
 * Return: true on success, false on failure (args are NULL).
 */
bool bn_copy(bignum *src, bignum *dest)
{
	if (!src || !dest)
		return (false);

	*dest = *src;
	return (true);
}

/**
 * bn_deepcopy - deep copies a bignum struct (pointers will be dereferenced).
 * @src: the bignum to be copied.
 * @dest: where to copy to.
 *
 * All pointers in dest should reference memory areas large enough to hold
 * the corresponding data in src.
 * If pointers in src are NULL, nothing will be done.
 *
 * Return: true on success, false on failure.
 */
bool bn_deepcopy(bignum *src, bignum *dest)
{
	if ((!src || !dest) ||
		(src->len > dest->len) ||
		(src->len && src->num && !dest->num))
		return (false);

	dest->len = src->len;
	dest->is_negative = src->is_negative;
	memmove(dest->num, src->num, sizeof(*src->num) * src->len);
	return (true);
}
