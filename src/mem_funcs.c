#include "infiX.h"

/**
 * xmalloc - allocate memory with malloc and check for failure.
 * @size: size in bytes to allocate.
 *
 * Return: a pointer to the memory area, NULL on failure.
 */
void *xmalloc(size_t size)
{
	void *ptr = malloc(size);

	if (!ptr && size)
		perror("Memory allocation failed.");

	return (ptr);
}

/**
 * xcalloc - allocate memory with calloc and check for failure.
 * @items: number of items to allocate memory for.
 * @sizeof_item: the size in bytes of each item.
 *
 * Return: a pointer to the memory area, NULL on failure.
 */
void *xcalloc(size_t items, size_t sizeof_item)
{
	void *ptr = calloc(items, sizeof_item);

	if (!ptr && items * sizeof_item != 0)
		perror("Memory allocation failed.");

	return (ptr);
}

/**
 * alloc_u4b_array - allocates memory for a u4b_array of a given length
 * @len: length of the array, length 0 returns the struct with a NULL array
 *
 * Return: a pointer to the u4b_array struct, NULL on failure
 */
u4b_array *alloc_u4b_array(size_t len)
{
	u4b_array *arr = xcalloc(1, sizeof(*arr));

	if (!arr)
		return (NULL);

	arr->len = len;
	if (len > 0)
	{
		arr->array = xcalloc(len, sizeof(*arr->array));
		if (!arr->array)
			arr = free_n_null(arr);
	}

	return (arr);
}

/**
 * free_n_null - free a pointer, return NULL.
 * @ptr: a freeable pointer.
 *
 * Return: NULL always.
 */
void *free_n_null(void *ptr)
{
	free(ptr);
	return (NULL);
}

/**
 * free_u4b_array - free a u4b_array, return NULL.
 * @arr: a pointer to a u4b_array.
 *
 * Return: NULL always.
 */
void *free_u4b_array(u4b_array *arr)
{
	if (arr)
		free_n_null(arr->array);

	return (free_n_null(arr));
}
