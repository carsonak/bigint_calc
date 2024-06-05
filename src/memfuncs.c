#include "infiX.h"

/**
 * malloc_check - allocate memory with malloc and check for failure.
 * @size: size in bytes to allocate
 *
 * Return: a pointer to the memory area, NULL on failure
 */
void *check_malloc(size_t size)
{
	void *ptr = malloc(size);

	if (!ptr)
		perror("Malloc fail");

	return (ptr);
}

/**
 * calloc_check - allocate memory with calloc and check for failure.
 * @items: number of items to allocate memory for
 * @sizeof_item: the size in bytes of each item
 *
 * Return: a pointer to the memory area, NULL on failure
 */
void *check_calloc(size_t items, size_t sizeof_item)
{
	void *ptr = calloc(items, sizeof_item);

	if (!ptr)
		perror("Calloc fail");

	return (ptr);
}

/**
 * free_n_null - frees and nulls a pointer
 * @ptr: a freeable pointer
 */
void free_n_null(void *ptr)
{
	free(ptr);
	ptr = NULL;
}

/**
 * alloc_u4b_array - allocates memory for a u4b_array of a given length
 * @len: length of the array, length 0 returns the struct with a NULL array
 *
 * Return: a pointer to the u4b_array struct, NULL on failure
 */
u4b_array *alloc_u4b_array(size_t len)
{
	u4b_array *arr = check_calloc(1, sizeof(*arr));

	if (!arr)
		return (NULL);

	if (len > 0)
		arr->array = check_calloc(len, sizeof(*arr->array));

	if (len && !arr->array)
	{
		free_n_null(arr);
		return (NULL);
	}

	arr->len = len;
	return (arr);
}

/**
 * free_u4b_array - free a u4b_array
 * @arr: pointer to a u4b_array
 */
void free_u4b_array(u4b_array *arr)
{
	if (arr)
		free_n_null(arr->array);

	free_n_null(arr);
}
