#include "infiX.h"

/**
 * memfill - fills a section of memory with a constant byte
 * @mem: pointer to a memory block
 * @b: character to fill memory with
 * @start: where to start filling
 * @nbytes: how many bytes to fill
 *
 * Return: pointer to the modified memory block
 */
void *memfill(void *mem, char b, size_t start, size_t nbytes)
{
	size_t i = 0;

	for (i = start; i < (start + nbytes); i++)
		((char *)mem)[i] = b;

	return (mem);
}

/**
 * _memcpy - copies a specified about of bytes from one memory area to another
 * @dest: the destination
 * @src: the source
 * @n: number of bytes to be copied
 *
 * Return: returns pointer to destination
 */
void *_memcpy(void *dest, void *src, size_t n)
{
	size_t i = 0;

	if (src < dest && (((char *)src + n) >= (char *)dest))
	{
		for (i = n; i < n && dest != src; i--)
			((char *)dest)[i] = ((char *)src)[i];
	}
	else
	{
		for (i = 0; i < n && dest != src; i++)
			((char *)dest)[i] = ((char *)src)[i];
	}

	return (dest);
}

/**
 * malloc_check - allocate memory with malloc and check for failure.
 * @size: size in bytes to allocate
 *
 * Return: a pointer to the memory area, NULL on failure
 */
void *malloc_check(size_t size)
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
void *calloc_check(size_t items, size_t sizeof_item)
{
	void *ptr = calloc(items, sizeof_item);

	if (!ptr)
		perror("Calloc fail");

	return (ptr);
}
