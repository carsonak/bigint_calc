#ifndef INFIX_PROJ_XALLOC_H
#define INFIX_PROJ_XALLOC_H

#include "macros.h"

#include <stddef.h> /* size_t */
#include <stdio.h>  /* perror */
#include <stdlib.h> /* *alloc */
#include <string.h> /* strdup */

void *free_n_null(void *freeable_ptr);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_n_null)
ATTR_ALLOC_SIZE(1, 2)
void *xcalloc(size_t items, size_t sizeof_item);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_n_null)
ATTR_ALLOC_SIZE(1)
void *xmalloc(size_t size);
ATTR_MALLOC_FREE(free_n_null)
ATTR_ALLOC_SIZE(2)
void *xrealloc(void *nullable_ptr, size_t size);
void *xrealloc_free_on_fail(void *nullable_ptr, size_t size);
ATTR_MALLOC ATTR_MALLOC_FREE(free_n_null)
char *xstrdup(char const *const str);

#endif /* INFIX_PROJ_XALLOC_H */
