#include "xalloc.h"

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
 * xrealloc - resize memory with realloc and check for error.
 * @nullable_ptr: pointer to an allocated block.
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
 * xrealloc_free_on_fail - resize memory with xrealloc but free original memory on error.
 * @nullable_ptr: pointer to an allocated block.
 * @size: size in bytes to resize to.
 *
 * Return: pointer to the resized memory area, NULL on failure.
 *
 */
void *xrealloc_free_on_fail(void *nullable_ptr, size_t size)
{
	void *ptr = xrealloc(nullable_ptr, size);

	if (!ptr && size)
		nullable_ptr = free_n_null(nullable_ptr);

	return (ptr);
}

/**
 * xstrdup - duplicate a string and check for error.
 * @str: the string to duplicate.
 *
 * Return: pointer to the duplicated string, NULL on failure.
 */
char *xstrdup(char const *const str)
{
	char *s = strdup(str);

	if (!str)
		perror("Memory allocation error");

	return (s);
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
