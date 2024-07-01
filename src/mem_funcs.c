#include "infiX.h"

/**
 * xmalloc - allocate memory with malloc and check for failure.
 * @size: size in bytes to allocate.
 *
 * Return: a pointer to the memory area, NULL on failure.
 */
void *xmalloc(unsigned long int size)
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
void *xcalloc(unsigned long int items, unsigned long int sizeof_item)
{
	void *ptr = calloc(items, sizeof_item);

	if (!ptr && (items * sizeof_item) > 0)
		perror("Memory allocation error");

	return (ptr);
}

/**
 * xrealloc - resize memory with realloc and check for error.
 * @nullable_ptr: pointer to an allocated block.
 * @size: size in bytes to resize to.
 *
 * Return: pointer to the resized memory area, NULL on failure.
 */
void *xrealloc(void *nullable_ptr, unsigned long int size)
{
	void *ptr = realloc(nullable_ptr, size);

	if (!ptr && size)
		perror("Memory resizing error");

	return (ptr);
}

/**
 * xstrdup - duplicate a string and check for error.
 * @str: the string to duplicate.
 *
 * Return: pointer to the duplicated string, NULL on failure.
 */
char *xstrdup(const char *str)
{
	char *s = strdup(str);

	if (!str)
		perror("Memory allocation error");

	return (s);
}

/**
 * alloc_bignum - allocate memory for a BigNum of given length.
 * @len: length of the array, length 0 returns the struct with a NULL array.
 *
 * Return: a pointer to a BigNum struct, NULL on failure.
 */
BigNum *alloc_bignum(unsigned long int len)
{
	BigNum *arr = xcalloc(1, sizeof(*arr));

	if (!arr)
		return (NULL);

	arr->len = len;
	if (len > 0)
	{
		arr->num = xcalloc(len, sizeof(*arr->num));
		if (!arr->num)
			arr = free_n_null(arr);
	}

	return (arr);
}

/**
 * alloc_numstr - allocate memory for a numstr of given length.
 * @len: length of the array, length 0 returns the struct with a NULL array.
 *
 * Return: a pointer to a numstr struct, NULL on failure.
 */
numstr *alloc_numstr(unsigned long int len)
{
	numstr *arr = xcalloc(1, sizeof(*arr));

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
 * free_bignum - free a BigNum, return NULL.
 * @freeable_ptr: a pointer to a BigNum.
 *
 * Return: NULL always.
 */
void *free_bignum(BigNum *freeable_ptr)
{
	if (freeable_ptr)
		free_n_null(freeable_ptr->num);

	return (free_n_null(freeable_ptr));
}

/**
 * free_numstr - free a numstr, return NULL.
 * @freeable_ptr: a pointer to a freeable.
 *
 * Return: NULL always.
 */
void *free_numstr(numstr *freeable_ptr)
{
	if (freeable_ptr)
		free_n_null(freeable_ptr->str);

	return (free_n_null(freeable_ptr));
}
