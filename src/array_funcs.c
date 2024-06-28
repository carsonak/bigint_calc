#include "infiX.h"

/**
 * trim_u4b_array - truncate length of a u4b_array to ignore trailing zeros.
 * @arr: pointer to a u4b_array struct
 *
 * Also sets the is_negative flag to 0 for 0s and NULL arrays.
 */
void trim_u4b_array(u4b_array *arr)
{
	if (!arr)
		return;

	if (!arr->array)
		arr->len = 0;

	while (arr->len > 1 && !arr->array[arr->len - 1])
		--arr->len;

	if (arr->len <= 1)
	{
		if (!arr->array || !arr->array[0])
			arr->is_negative = false;
	}
}

/**
 * cmp_u4barray - compare 2 u4b_arrays.
 * @a1: the first array
 * @a2: the second array
 *
 * Return: +ve number if a1 > a2, -ve number if a1 < a2 else 0.
 */
ssize_t cmp_u4barray(u4b_array *a1, u4b_array *a2)
{
	if (!a1 || !a2)
		return (0);

	trim_u4b_array(a1);
	trim_u4b_array(a2);
	if (a1->is_negative && a2->is_negative)
	{
		if ((ssize_t)a2->len - a1->len)
			return ((ssize_t)a2->len - a1->len);

		return (cmp_rev_uint32array(a2->array, a1->array, a1->len));
	}
	else if (a1->is_negative)
		return (-1);
	else if (a2->is_negative)
		return (1);

	if ((ssize_t)a1->len - a2->len)
		return ((ssize_t)a1->len - a2->len);

	return (cmp_rev_uint32array(a1->array, a2->array, a1->len));
}

/**
 * cmp_rev_uint32array - compare int arrays in reverse
 * @arr1: first array
 * @arr2: second array
 * @len: total items to compare
 *
 * Return: +ve number if arr1 > arr2, -ve number if arr1 < arr2 else 0.
 */
ssize_t cmp_rev_uint32array(uint32_t *arr1, uint32_t *arr2, size_t len)
{
	size_t i = len ? len - 1 : 0;

	if (!arr1 || !arr2)
		return (0);

	for (; i > 0; i--)
	{
		if ((ssize_t)arr1[i] - arr2[i])
			break;
	}

	return ((ssize_t)arr1[i] - arr2[i]);
}
