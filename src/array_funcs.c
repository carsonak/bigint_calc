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
			arr->is_negative = 0;
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
	size_t i = 0;

	if (!a1 || !a2)
		return (0);

	trim_u4b_array(a1);
	trim_u4b_array(a2);
	if ((int)a2->is_negative - a1->is_negative)
		return ((int)a2->is_negative - a1->is_negative);

	if (a1->is_negative && a2->is_negative)
	{
		if ((ssize_t)a2->len - a1->len || (!a1->len && !a2->len))
			return ((ssize_t)a2->len - a1->len);

		for (i = a1->len - 1; i > 0; i--)
		{
			if ((ssize_t)a2->array[i] - a1->array[i])
				break;
		}

		return ((ssize_t)a2->array[i] - a1->array[i]);
	}

	if ((ssize_t)a1->len - a2->len || (!a1->len && !a2->len))
		return ((ssize_t)a1->len - a2->len);

	for (i = a1->len - 1; i > 0; i--)
	{
		if ((ssize_t)a1->array[i] - a2->array[i])
			break;
	}

	return ((ssize_t)a1->array[i] - a2->array[i]);
}
