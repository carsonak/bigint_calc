#include "xalloc.h"

void *xmalloc(const len_type size)
{
	(void)size;
	return (NULL);
}

void *xcalloc(const len_type items, const len_type sizeof_item)
{
	(void)items;
	(void)sizeof_item;
	return (NULL);
}

void *xrealloc(void *nullable_ptr, const len_type size)
{
	(void)nullable_ptr;
	(void)size;
	return (NULL);
}

void *xrealloc_free_on_fail(void *nullable_ptr, const len_type size)
{
	(void)nullable_ptr;
	(void)size;
	return (NULL);
}

char *xstrdup(char const *const str)
{
	(void)str;
	return (NULL);
}

void *xfree(void *const freeable_ptr)
{
	(void)freeable_ptr;
	return (NULL);
}
