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
 * alloc_bignum - allocate memory for a bignum of given length.
 * @len: length of the array, length 0 returns the struct with a NULL array.
 *
 * Return: a pointer to a bignum struct, NULL on failure.
 */
bignum *alloc_bignum(size_t len)
{
	bignum *arr = xcalloc(1, sizeof(*arr));

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
 * bignum_dup - duplicate a bignum.
 * @bn: a pointer to a bignum.
 *
 * Return: a pointer to a copy of bn.
 */
bignum *bignum_dup(bignum *bn)
{
	bignum *dup = NULL;

	if (!bn)
		return (NULL);

	trim_bignum(bn);
	dup = alloc_bignum(0);
	if (!bn->len || !dup)
		return (dup);

	dup->num = xmalloc(bn->len * sizeof(*dup->num));
	if (!dup->num)
		return (free_bignum(dup));

	dup->is_negative = bn->is_negative;
	dup->len = bn->len;
	memcpy(dup->num, bn->num, sizeof(*bn->num) * bn->len);
	return (dup);
}

/**
 * alloc_numstr - allocate memory for a numstr of given length.
 * @len: length of the array, length 0 returns the struct with a NULL array.
 *
 * Return: a pointer to a numstr struct, NULL on failure.
 */
numstr *alloc_numstr(size_t len)
{
	numstr *arr = xcalloc(1, sizeof(*arr));

	if (!arr)
		return (NULL);

	arr->len = len;
	if (len > 0)
	{
		arr->str = xmalloc(len * sizeof(*arr->str));
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
 * free_bignum - free a bignum, return NULL.
 * @freeable_ptr: a pointer to a bignum.
 *
 * Return: NULL always.
 */
void *free_bignum(bignum *freeable_ptr)
{
	if (freeable_ptr)
		free_n_null(freeable_ptr->num);

	return (free_n_null(freeable_ptr));
}

/**
 * free_numstr - free a numstr, return NULL.
 * @freeable_ptr: a pointer to a numstr.
 *
 * Return: NULL always.
 */
void *free_numstr(numstr *freeable_ptr)
{
	if (freeable_ptr)
		free_n_null(freeable_ptr->str);

	return (free_n_null(freeable_ptr));
}
