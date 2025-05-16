#ifndef BIGINT_XALLOC_H
#define BIGINT_XALLOC_H

#include "macros.h"

#include <stdio.h>  /* perror */
#include <stdlib.h> /* *alloc */

#include "types.h"

void *xfree(void *freeable_ptr);
ATTR_MALLOC
ATTR_MALLOC_FREE(xfree)
ATTR_ALLOC_SIZE(1, 2)
void *xcalloc(const len_typ items, const len_typ sizeof_item);
ATTR_MALLOC
ATTR_MALLOC_FREE(xfree)
ATTR_ALLOC_SIZE(1)
void *xmalloc(const len_typ size);
ATTR_ALLOC_SIZE(2)
void *xrealloc(void *nullable_ptr, const len_typ size);
ATTR_ALLOC_SIZE(2)
void *xrealloc_free_on_fail(void *nullable_ptr, const len_typ size);
ATTR_MALLOC ATTR_MALLOC_FREE(xfree)
char *xstrdup(char const *const str);

#endif /* BIGINT_XALLOC_H */
