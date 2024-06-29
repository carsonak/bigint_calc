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
		perror("Memory allocation error");

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

	if (!ptr && (items * sizeof_item) > 0)
		perror("Memory allocation error");

	return (ptr);
}

/**
 * xrealloc - resize memory with realloc and check for error
 * @nullable_ptr: pointer to an allocated block
 * @size: size in bytes to resize to.
 *
 * Return: pointer to the resized memory area, NULL on failure.
 */
void *xrealloc(void *nullable_ptr, size_t size)
{
	void *ptr = realloc(nullable_ptr, size);

	if (!ptr && size)
		perror("Memory resizing error");

	return (ptr);
}

/**
 * alloc_u4b_array - allocate memory for a u4b_bignum of given length
 * @len: length of the array, length 0 returns the struct with a NULL array
 *
 * Return: a pointer to a u4b_bignum struct, NULL on failure
 */
u4b_bignum *alloc_u4b_array(size_t len)
{
	u4b_bignum *arr = xcalloc(1, sizeof(*arr));

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
 * alloc_numstr_array - allocate memory for a intstr of given length
 * @len: length of the array, length 0 returns the struct with a NULL array
 *
 * Return: a pointer to a intstr struct, NULL on failure.
 */
intstr *alloc_numstr_array(size_t len)
{
	intstr *arr = xcalloc(1, sizeof(*arr));

	if (!arr)
		return (NULL);

	arr->len = len;
	if (len > 0)
	{
		arr->str = xcalloc(len, sizeof(*arr->str));
		if (!arr->str)
			arr = free_n_null(arr);
	}

	return (arr);
}

/**
 * free_n_null - free a pointer, return NULL.
 * @freeable_ptr: a freeable pointer.
 *
 * Return: NULL always.
 */
void *free_n_null(void *freeable_ptr)
{
	free(freeable_ptr);
	return (NULL);
}

/**
 * free_u4b_array - free a u4b_bignum, return NULL.
 * @freeable_arr: a pointer to a u4b_bignum.
 *
 * Return: NULL always.
 */
void *free_u4b_array(u4b_bignum *freeable_arr)
{
	if (freeable_arr)
		free_n_null(freeable_arr->array);

	return (free_n_null(freeable_arr));
}

/**
 * free_numstr_array - free a intstr, return NULL.
 * @freeable_arr: a pointer to a freeable
 *
 * Return: NULL always.
 */
void *free_numstr_array(intstr *freeable_arr)
{
	if (freeable_arr)
		free_n_null(freeable_arr->str);

	return (free_n_null(freeable_arr));
}
