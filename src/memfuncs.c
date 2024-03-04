#include "infix.h"

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
 * _memcpy - copies a specified about of bytes from one mem to another
 * @dest: the destination
 * @src: the source
 * @n: number of bytes to be copied
 *
 * Return: returns pointer to destination
 */
void *_memcpy(void *dest, void *src, size_t n)
{
	size_t i = 0;

	for (i = 0; i < n; i++)
		((char *)dest)[i] = ((char *)src)[i];

	return (dest);
}
