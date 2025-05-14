#include "xalloc.h"
#include "string.h"

#include <inttypes.h> /* printf macros for variable stdint types. */

/**
 * xmalloc - allocate memory with malloc and report errors.
 * @size: size in bytes to allocate.
 *
 * Return: a pointer to the memory area, NULL on failure.
 */
void *xmalloc(const len_type size)
{
	if (size < 0)
	{
		fprintf(stderr, "xmalloc error: negative size %" PRIdMAX, size);
		return (NULL);
	}

	void *const ptr = malloc(size);

	if (!ptr && size > 0)
		perror("xmalloc error");

	return (ptr);
}

/**
 * xcalloc - allocate memory with calloc and report errors.
 * @items: number of items to allocate memory for.
 * @sizeof_item: the size in bytes of each item.
 *
 * Return: a pointer to the memory area, NULL on failure.
 */
void *xcalloc(const len_type items, const len_type sizeof_item)
{
	if (items < 0)
	{
		fprintf(stderr, "xcalloc error: negative parameter %" PRIdMAX, items);
		return (NULL);
	}

	if (sizeof_item < 0)
	{
		fprintf(
			stderr, "xcalloc error: negative parameter %" PRIdMAX, sizeof_item
		);
		return (NULL);
	}

	void *const ptr = calloc(items, sizeof_item);

	if (!ptr && (items * sizeof_item) > 0)
		perror("xcalloc error");

	return (ptr);
}

/**
 * xrealloc - resize memory with realloc and report errors.
 * @nullable_ptr: pointer to an allocated block.
 * @size: size in bytes to resize to.
 *
 * Return: pointer to the resized memory area, NULL on failure.
 */
void *xrealloc(void *nullable_ptr, len_type size)
{
	if (size < 0)
	{
		fprintf(stderr, "xrealloc error: negative size %" PRIdMAX, size);
		return (NULL);
	}

	nullable_ptr = realloc(nullable_ptr, size);
	if (!nullable_ptr && size > 0)
		perror("Memory resizing error");

	return (nullable_ptr);
}

/**
 * xrealloc_free_on_fail - resize memory with xrealloc but free original memory on error.
 * @nullable_ptr: pointer to an allocated block.
 * @size: size in bytes to resize to.
 *
 * Return: pointer to the resized memory area, NULL on failure.
 *
 */
void *xrealloc_free_on_fail(void *nullable_ptr, len_type size)
{
	void *const ptr = xrealloc(nullable_ptr, size);

	if (!ptr && size)
		nullable_ptr = xfree(nullable_ptr);

	return (ptr);
}

/**
 * xstrdup - duplicate a string and report errors.
 * @str: the string to duplicate.
 *
 * Return: pointer to the duplicated string, NULL on failure.
 */
char *xstrdup(char const *const str)
{
	char *const s = xmalloc(sizeof(*s) * (strlen(str) + 1));

	if (str)
		strcpy(s, str);

	return (s);
}

/**
 * xfree - free a pointer, return NULL.
 * @freeable_ptr: a freeable pointer.
 *
 * Return: NULL always.
 */
void *xfree(void *freeable_ptr)
{
	free(freeable_ptr);
	return (NULL);
}
