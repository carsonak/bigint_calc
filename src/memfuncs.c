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
 * free_u4b_array - free a u4b_array
 * @arr: pointer to a u4b_array
 */
void free_u4b_array(u4b_array *arr)
{
	if (arr)
		free_n_null(arr->array);

	free_n_null(arr);
}
