#include "xalloc.h"

/**
 * free_n_null - dummy
 * @freeable_ptr: unused.
 *
 * Return: NULL always.
 */
void *free_n_null(void *freeable_ptr)
{
	(void)freeable_ptr;
	return (NULL);
}

/**
 * xcalloc - dummy.
 * @items: unused.
 * @sizeof_item: unused.
 *
 * Return: NULL always.
 */
void *xcalloc(size_t items, size_t sizeof_item)
{
	(void)items;
	(void)sizeof_item;
	return (NULL);
}

/**
 * xmalloc - dummy.
 * @size: unused.
 *
 * Return: NULL always.
 */
void *xmalloc(size_t size)
{
	(void)size;
	return (NULL);
}

/**
 * xrealloc - dummy.
 * @nullable_ptr: unused.
 * @size: unused.
 *
 * Return: NULL always.
 */
void *xrealloc(void *nullable_ptr, size_t size)
{
	(void)nullable_ptr;
	(void)size;
	return (NULL);
}

/**
 * xstrdup - dummy.
 * @str: unused.
 *
 * Return: NULL always.
 */
char *xstrdup(const char *str)
{
	(void)str;
	return (NULL);
}
