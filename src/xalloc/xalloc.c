/*!
 * @file
 * @brief memory manipulation functions.
 */

#include "xalloc.h"

#include <inttypes.h> /* printf macros for variable stdint types. */
#include <stdio.h>    /* perror */
#include <string.h>   /* strcpy, strlen */

/*!
 * @brief allocate memory with malloc and report errors.
 *
 * @param[in] size size in bytes to allocate.
 *
 * @return a pointer to the memory area, NULL on failure.
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

/*!
 * @brief allocate memory with calloc and report errors.
 *
 * @param[in] items number of items to allocate memory for.
 * @param[in] sizeof_item the size in bytes of each item.
 *
 * @return a pointer to the memory area, NULL on failure.
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

	if (!ptr && ((size_t)items * sizeof_item) > 0)
		perror("xcalloc error");

	return (ptr);
}

/*!
 * @brief resize memory with realloc and report errors.
 *
 * @param[in] nullable_ptr pointer to an allocated block.
 * @param[in] size size in bytes to resize to.
 *
 * @return pointer to the resized memory area, NULL on failure.
 */
void *xrealloc(void *nullable_ptr, const len_type size)
{
	if (size < 0)
	{
		fprintf(stderr, "xrealloc error: negative size %" PRIdMAX, size);
		return (NULL);
	}

	nullable_ptr = realloc(nullable_ptr, size);
	if (!nullable_ptr && size > 0)
		perror("xrealloc error");

	return (nullable_ptr);
}

/*!
 * @brief resize memory with xrealloc but free original memory on error.
 *
 * @param[in] nullable_ptr pointer to an allocated block.
 * @param[in] size size in bytes to resize to.
 *
 * @return pointer to the resized memory area, NULL on failure.
 *
 */
void *xrealloc_free_on_fail(void *nullable_ptr, const len_type size)
{
	void *const ptr = xrealloc(nullable_ptr, size);

	if (!ptr && size)
		free(nullable_ptr);

	return (ptr);
}

/*!
 * @brief duplicate a string and report errors.
 *
 * @param[in] str the string to duplicate.
 *
 * @return pointer to the duplicated string, NULL on failure.
 */
char *xstrdup(char const *const str)
{
	char *const s = xmalloc(sizeof(*s) * (strlen(str) + 1));

	if (str)
		strcpy(s, str);

	return (s);
}

/*!
 * @brief free a pointer, return NULL.
 *
 * @param[in] freeable_ptr a freeable pointer.
 *
 * @return NULL always.
 */
void *xfree(void *const freeable_ptr)
{
	free(freeable_ptr);
	return (NULL);
}
