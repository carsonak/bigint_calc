#ifndef INFIX_PROJ_XALLOC_H
#define INFIX_PROJ_XALLOC_H

#include "macros.h"

#include <stddef.h> /* size_t */
#include <stdio.h>  /* perror */
#include <stdlib.h> /* *alloc */

void *xfree(void *freeable_ptr);
ATTR_MALLOC
ATTR_MALLOC_FREE(xfree)
ATTR_ALLOC_SIZE(1, 2)
void *xcalloc(const size_t items, const size_t sizeof_item);
ATTR_MALLOC
ATTR_MALLOC_FREE(xfree)
ATTR_ALLOC_SIZE(1)
void *xmalloc(const size_t size);
ATTR_MALLOC_FREE(xfree)
ATTR_ALLOC_SIZE(2)
void *xrealloc(void *nullable_ptr, const size_t size);
void *xrealloc_free_on_fail(void *nullable_ptr, const size_t size);
ATTR_MALLOC ATTR_MALLOC_FREE(xfree)
char *xstrdup(char const *const str);

#endif /* INFIX_PROJ_XALLOC_H */
